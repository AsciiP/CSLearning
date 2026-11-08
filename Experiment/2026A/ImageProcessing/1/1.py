from pathlib import Path

import cv2
import numpy as np


INPUT_IMAGE_PATH = Path(__file__).with_name("lotus.jpg")
OUTPUT_DIR = Path(__file__).with_name("results")


def translate_image(image: np.ndarray, x_shift: int, y_shift: int) -> np.ndarray:
    """按照给定的水平、垂直位移平移图像，空白区域填充黑色。"""
    height, width = image.shape[:2]
    translation_matrix = np.array(
        [[1, 0, x_shift], [0, 1, y_shift]], dtype=np.float32
    )
    return cv2.warpAffine(
        image,
        translation_matrix,
        (width, height),
        borderMode=cv2.BORDER_CONSTANT,
        borderValue=(0, 0, 0),
    )


def resize_image(
    image: np.ndarray, scale: float, interpolation: int
) -> np.ndarray:
    """按照比例缩放图像。"""
    height, width = image.shape[:2]
    new_size = (max(1, int(width * scale)), max(1, int(height * scale)))
    return cv2.resize(image, new_size, interpolation=interpolation)


def rotate_image(
    image: np.ndarray, angle: float, center: tuple[float, float] | None = None
) -> np.ndarray:
    """绕指定中心旋转图像，输出尺寸与输入相同。"""
    height, width = image.shape[:2]
    if center is None:
        center = (width / 2, height / 2)
    rotation_matrix = cv2.getRotationMatrix2D(center, angle, 1.0)
    return cv2.warpAffine(
        image,
        rotation_matrix,
        (width, height),
        borderMode=cv2.BORDER_CONSTANT,
        borderValue=(0, 0, 0),
    )


def save_results(image: np.ndarray, output_dir: Path) -> list[Path]:
    """执行实验中的各项变换，并保存结果图像。"""
    output_dir.mkdir(parents=True, exist_ok=True)
    height, width = image.shape[:2]
    center = (width / 2, height / 2)

    gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    translated_image = translate_image(image, x_shift=80, y_shift=50)
    nearest_image = resize_image(
        image, scale=1.5, interpolation=cv2.INTER_NEAREST
    )
    bilinear_image = resize_image(
        image, scale=1.5, interpolation=cv2.INTER_LINEAR
    )
    rotated_image = rotate_image(image, angle=30, center=center)

    results = {
        "original.jpg": image,
        "gray.jpg": gray_image,
        "translated.jpg": translated_image,
        "scaled_nearest.jpg": nearest_image,
        "scaled_bilinear.jpg": bilinear_image,
        "rotated.jpg": rotated_image,
    }
    saved_paths = []
    for filename, result in results.items():
        path = output_dir / filename
        if not cv2.imwrite(str(path), result):
            raise OSError(f"无法写入结果图像：{path}")
        saved_paths.append(path)
    return saved_paths


def main() -> None:
    image = cv2.imread(str(INPUT_IMAGE_PATH), cv2.IMREAD_COLOR)
    if image is None:
        raise FileNotFoundError(f"无法读取输入图像：{INPUT_IMAGE_PATH}")

    saved_paths = save_results(image, OUTPUT_DIR)
    print(f"输入图像：{INPUT_IMAGE_PATH}")
    print(f"结果目录：{OUTPUT_DIR}")
    print("已生成：")
    for path in saved_paths:
        print(f"  {path.name}")


if __name__ == "__main__":
    main()