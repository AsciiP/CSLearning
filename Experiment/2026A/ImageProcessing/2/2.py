from pathlib import Path

import cv2
import numpy as np


COLOR_IMAGE_PATH = Path(__file__).with_name("bird.jpg")
GRAY_IMAGE_PATH = Path(__file__).with_name("coffee.jpg")
OUTPUT_DIR = Path(__file__).with_name("results")

# 加权灰度化系数（ITU-R BT.601 亮度公式）：人眼对绿色最敏感、对蓝色最不敏感
WEIGHT_R, WEIGHT_G, WEIGHT_B = 0.299, 0.587, 0.114
# 强度分层伪彩色的分层数
SLICING_LEVELS = 8


def gray_by_max(image: np.ndarray) -> np.ndarray:
    """最大值灰度化：取每个像素 R、G、B 三通道中的最大值作为灰度值。"""
    return np.max(image, axis=2).astype(np.uint8)


def gray_by_mean(image: np.ndarray) -> np.ndarray:
    """平均值灰度化：取每个像素三通道值的算术平均作为灰度值。"""
    return np.rint(np.mean(image, axis=2)).astype(np.uint8)


def gray_by_weighted(image: np.ndarray) -> np.ndarray:
    """加权平均灰度化：按亮度权重组合三通道，结果最接近人眼的明暗感知。"""
    # OpenCV 读入的通道顺序为 B、G、R，权重需按该顺序对应
    weighted = (
        image[:, :, 0] * WEIGHT_B
        + image[:, :, 1] * WEIGHT_G
        + image[:, :, 2] * WEIGHT_R
    )
    return np.rint(weighted).astype(np.uint8)


def pseudo_color_by_slicing(
    gray: np.ndarray, levels: int = SLICING_LEVELS
) -> np.ndarray:
    """强度分层伪彩色：把灰度范围均分为 levels 个区间，每个区间赋予一种固定颜色。"""
    # 在 jet 彩色条上均匀采样 levels 种颜色，作为各灰度区间的颜色表
    samples = (np.arange(levels) * 255 // (levels - 1)).astype(np.uint8).reshape(-1, 1)
    color_table = cv2.applyColorMap(samples, cv2.COLORMAP_JET).reshape(levels, 3)
    # 计算每个像素所属的区间编号，再查颜色表完成着色
    index = np.clip(gray.astype(np.int32) * levels // 256, 0, levels - 1)
    return color_table[index]


def pseudo_color_by_transform(gray: np.ndarray) -> np.ndarray:
    """灰度值到彩色变换伪彩色：用 R、G、B 三个分段线性变换把灰度映射为彩色。"""
    g = gray.astype(np.float64)
    # 各通道的（灰度值, 通道值）控制点，整体形成蓝-青-绿-黄-红的连续过渡
    red = np.interp(g, [0, 127, 191, 255], [0, 0, 255, 255])
    green = np.interp(g, [0, 64, 127, 191, 255], [0, 0, 255, 255, 0])
    blue = np.interp(g, [0, 64, 127, 255], [255, 255, 0, 0])
    bgr = np.stack([blue, green, red], axis=-1)
    return np.clip(bgr, 0, 255).astype(np.uint8)


def bgr_to_his(image: np.ndarray) -> np.ndarray:
    """将 BGR 彩色图像转换到 HIS 颜色空间（H 色调、I 亮度、S 饱和度）。"""
    # 各分量归一化到 [0, 1]
    normalized = image.astype(np.float64) / 255.0
    blue, green, red = normalized[:, :, 0], normalized[:, :, 1], normalized[:, :, 2]

    # 亮度 I：三分量的算术平均
    intensity = (red + green + blue) / 3.0
    # 饱和度 S = 1 - min(R,G,B)/I；I 为 0（纯黑）时定义 S 为 0，避免除零
    min_channel = np.minimum(np.minimum(red, green), blue)
    saturation = np.zeros_like(intensity)
    positive = intensity > 0
    saturation[positive] = 1.0 - min_channel[positive] / intensity[positive]

    # 色调 H：标准反余弦公式；分母加极小量并裁剪 arccos 入参，避免数值错误
    numerator = ((red - green) + (red - blue)) / 2.0
    denominator = np.sqrt((red - green) ** 2 + (red - blue) * (green - blue)) + 1e-12
    theta = np.arccos(np.clip(numerator / denominator, -1.0, 1.0))
    hue = np.where(blue <= green, theta, 2.0 * np.pi - theta)

    # 与 OpenCV 存储约定一致：H 压缩到 [0,179]，S、I 拉伸到 [0,255]
    his = np.stack(
        [hue / (2.0 * np.pi) * 179.0, saturation * 255.0, intensity * 255.0],
        axis=-1,
    )
    return np.clip(his, 0, 255).astype(np.uint8)


def his_to_bgr(his: np.ndarray) -> np.ndarray:
    """将 HIS 图像还原回 BGR 颜色空间，验证转换的可逆性。"""
    # 还原到 [0, 1]：H 由 [0,179] 恢复为弧度，S、I 由 [0,255] 恢复
    hue = his[:, :, 0].astype(np.float64) / 179.0 * 2.0 * np.pi
    saturation = his[:, :, 1].astype(np.float64) / 255.0
    intensity = his[:, :, 2].astype(np.float64) / 255.0

    red = np.empty_like(hue)
    green = np.empty_like(hue)
    blue = np.empty_like(hue)

    # 按色调所处扇区（每 2π/3 一段）套用 HIS→RGB 的标准反变换公式
    sector0 = (hue >= 0) & (hue < 2.0 * np.pi / 3.0)
    sector1 = (hue >= 2.0 * np.pi / 3.0) & (hue < 4.0 * np.pi / 3.0)
    sector2 = hue >= 4.0 * np.pi / 3.0

    def sector_to_rgb(
        hue_s: np.ndarray, sat_s: np.ndarray, int_s: np.ndarray
    ) -> tuple[np.ndarray, np.ndarray, np.ndarray]:
        """在局部扇区坐标下计算 (低通道, 高通道, 第三通道) 的分量值。"""
        low = int_s * (1.0 - sat_s)  # 该扇区的最小通道
        denom = np.maximum(np.cos(np.pi / 3.0 - hue_s), 1e-12)
        high = int_s * (1.0 + sat_s * np.cos(hue_s) / denom)
        third = 3.0 * int_s - low - high
        return low, high, third

    # 扇区0（0~120°）：R 为高通道、B 为低通道、G 为第三通道
    low, high, third = sector_to_rgb(
        hue[sector0], saturation[sector0], intensity[sector0]
    )
    red[sector0], green[sector0], blue[sector0] = high, third, low
    # 扇区1（120°~240°）：G 为高通道、R 为低通道、B 为第三通道
    low, high, third = sector_to_rgb(
        hue[sector1] - 2.0 * np.pi / 3.0, saturation[sector1], intensity[sector1]
    )
    red[sector1], green[sector1], blue[sector1] = low, high, third
    # 扇区2（240°~360°）：B 为高通道、G 为低通道、R 为第三通道
    low, high, third = sector_to_rgb(
        hue[sector2] - 4.0 * np.pi / 3.0, saturation[sector2], intensity[sector2]
    )
    red[sector2], green[sector2], blue[sector2] = third, low, high

    bgr = np.stack([blue, green, red], axis=-1) * 255.0
    return np.clip(bgr, 0, 255).astype(np.uint8)


def make_comparison(
    images: list[np.ndarray], panel_height: int = 240, gap: int = 4
) -> np.ndarray:
    """将各图像缩放到相同高度后横向拼接，中间插入白色分隔线，便于对比观察。"""
    panels: list[np.ndarray] = []
    for image in images:
        height, width = image.shape[:2]
        new_width = max(1, int(width * panel_height / height))
        panel = cv2.resize(
            image, (new_width, panel_height), interpolation=cv2.INTER_LINEAR
        )
        if panel.ndim == 2:  # 灰度图统一转为三通道后再拼接
            panel = cv2.cvtColor(panel, cv2.COLOR_GRAY2BGR)
        panels.append(panel)
        panels.append(np.full((panel_height, gap, 3), 255, np.uint8))
    panels.pop()  # 去掉末尾多余的分隔线
    return np.hstack(panels)


def save_results(
    color_image: np.ndarray, gray_image: np.ndarray, output_dir: Path
) -> list[Path]:
    """执行实验二的各项处理，保存结果图像与对比拼图。"""
    output_dir.mkdir(parents=True, exist_ok=True)

    gray_max = gray_by_max(color_image)
    gray_mean = gray_by_mean(color_image)
    gray_weighted = gray_by_weighted(color_image)
    pseudo_slicing = pseudo_color_by_slicing(gray_image)
    pseudo_transform = pseudo_color_by_transform(gray_image)

    # 实验目的(1)：RGB(BGR) 与 HSV、HIS 颜色空间的相互转换
    hsv_image = cv2.cvtColor(color_image, cv2.COLOR_BGR2HSV)
    his_image = bgr_to_his(color_image)
    # HIS 还原回 BGR：验证颜色空间转换的可逆性
    his_restored = his_to_bgr(his_image)

    # 量化 HIS→BGR 还原误差（uint8 量化本身会引入 ±1~2 的舍入误差）
    restore_error = np.abs(
        his_restored.astype(np.int16) - color_image.astype(np.int16)
    ).mean()
    print(f"HIS 还原回 BGR 的平均绝对误差：{restore_error:.3f}（灰阶 0~255）")


    # 记录三种灰度化结果的平均灰度，供报告中的定量对比使用
    print("三种灰度化结果的平均灰度：")
    for name, result in (
        ("最大值灰度化", gray_max),
        ("平均值灰度化", gray_mean),
        ("加权平均灰度化", gray_weighted),
    ):
        print(f"  {name}: {result.mean():.2f}")

    results = {
        "original.jpg": color_image,
        "gray_max.jpg": gray_max,
        "gray_mean.jpg": gray_mean,
        "gray_weighted.jpg": gray_weighted,
        "gray_compare.jpg": make_comparison(
            [color_image, gray_max, gray_mean, gray_weighted]
        ),
        # HSV 三个分量：H 色调、S 饱和度、V 明度
        "hsv_h.jpg": hsv_image[:, :, 0],
        "hsv_s.jpg": hsv_image[:, :, 1],
        "hsv_v.jpg": hsv_image[:, :, 2],
        # HIS 三个分量：H 色调、I 强度、S 饱和度，以及 HIS 还原回 BGR 的结果
        "his_h.jpg": his_image[:, :, 0],
        "his_i.jpg": his_image[:, :, 2],
        "his_s.jpg": his_image[:, :, 1],
        "his_restored.jpg": his_restored,
        "colorspace_compare.jpg": make_comparison(
            [
                color_image,
                hsv_image[:, :, 0],
                hsv_image[:, :, 1],
                hsv_image[:, :, 2],
                his_image[:, :, 0],
                his_image[:, :, 2],
                his_image[:, :, 1],
            ],
            panel_height=160,
        ),
        "pseudo_original.jpg": gray_image,
        "pseudo_slicing.jpg": pseudo_slicing,
        "pseudo_transform.jpg": pseudo_transform,
        "pseudo_compare.jpg": make_comparison(
            [gray_image, pseudo_slicing, pseudo_transform]
        ),
    }
    saved_paths = []
    for filename, result in results.items():
        path = output_dir / filename
        if not cv2.imwrite(str(path), result):
            raise OSError(f"无法写入结果图像：{path}")
        saved_paths.append(path)
    return saved_paths


def main() -> None:
    color_image = cv2.imread(str(COLOR_IMAGE_PATH), cv2.IMREAD_COLOR)
    if color_image is None:
        raise FileNotFoundError(f"无法读取彩色图像：{COLOR_IMAGE_PATH}")
    gray_image = cv2.imread(str(GRAY_IMAGE_PATH), cv2.IMREAD_GRAYSCALE)
    if gray_image is None:
        raise FileNotFoundError(f"无法读取灰度图像：{GRAY_IMAGE_PATH}")

    saved_paths = save_results(color_image, gray_image, OUTPUT_DIR)
    print(f"彩色图像：{COLOR_IMAGE_PATH}")
    print(f"灰度图像：{GRAY_IMAGE_PATH}")
    print(f"结果目录：{OUTPUT_DIR}")
    print("已生成：")
    for path in saved_paths:
        print(f"  {path.name}")


if __name__ == "__main__":
    main()
