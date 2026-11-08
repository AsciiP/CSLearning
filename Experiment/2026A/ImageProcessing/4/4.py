import cv2
import numpy as np
import matplotlib.pyplot as plt

IMAGE_PATH = r"D:\CSLearning\Experiment\2026A\ImageProcessing\4\peppers.jpg"


def centered_distance(shape):
	rows, cols = shape
	y, x = np.ogrid[:rows, :cols]
	return np.sqrt((y - rows // 2) ** 2 + (x - cols // 2) ** 2)


def main():
	image = cv2.imread(IMAGE_PATH)
	if image is None:
		raise FileNotFoundError(f"无法读取图像：{IMAGE_PATH}")

	gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

	# (1) DFT及频谱显示
	gray_float = gray.astype(np.float32)
	dft = cv2.dft(gray_float, flags=cv2.DFT_COMPLEX_OUTPUT)
	dft_shift = np.fft.fftshift(dft)
	spectrum = 20 * np.log(cv2.magnitude(dft_shift[:, :, 0], dft_shift[:, :, 1]) + 1)

	# 构造含高斯噪声的图像，并在频域进行理想低通滤波
	rng = np.random.default_rng(0)
	noisy = np.clip(gray.astype(np.float32) + rng.normal(0, 25, gray.shape), 0, 255).astype(np.uint8)
	noisy_dft = np.fft.fftshift(np.fft.fft2(noisy))
	distance = centered_distance(gray.shape)
	low_pass = (distance <= 35).astype(np.float32)
	smooth = np.real(np.fft.ifft2(np.fft.ifftshift(noisy_dft * low_pass)))
	smooth = np.clip(smooth, 0, 255).astype(np.uint8)

	# (3) 频域高通滤波，并将高频细节叠加到原图实现锐化
	image_dft = np.fft.fftshift(np.fft.fft2(gray))
	high_pass = (distance >= 12).astype(np.float32)
	high_frequency = np.real(np.fft.ifft2(np.fft.ifftshift(image_dft * high_pass)))
	sharpened = np.clip(gray.astype(np.float32) + 1.5 * high_frequency, 0, 255).astype(np.uint8)

	plt.figure(figsize=(12, 8))
	plt.subplot(2, 3, 1)
	plt.imshow(gray, cmap="gray")
	plt.title("Original image")
	plt.axis("off")

	plt.subplot(2, 3, 2)
	plt.imshow(spectrum, cmap="gray")
	plt.title("DFT spectrum")
	plt.axis("off")

	plt.subplot(2, 3, 3)
	plt.imshow(noisy, cmap="gray")
	plt.title("Noisy image")
	plt.axis("off")

	plt.subplot(2, 3, 4)
	plt.imshow(smooth, cmap="gray")
	plt.title("Low-pass smoothing")
	plt.axis("off")

	plt.subplot(2, 3, 5)
	plt.imshow(high_frequency, cmap="gray")
	plt.title("High-pass component")
	plt.axis("off")

	plt.subplot(2, 3, 6)
	plt.imshow(sharpened, cmap="gray")
	plt.title("High-pass sharpening")
	plt.axis("off")

	plt.tight_layout()
	plt.show()


if __name__ == "__main__":
	main()

