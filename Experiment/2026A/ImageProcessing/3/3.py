import cv2
import numpy as np
import matplotlib.pyplot as plt

# --- 常量定义 ---
# 请将此处修改为你实际使用的图片文件名
IMAGE_PATH = r"D:\CSLearning\Experiment\2026A\ImageProcessing\3\lotus.jpg"

def add_gaussian_noise(img, mean=0, sigma=25):
    """
    为图像添加高斯噪声
    :param img: 输入图像
    :param mean: 均值
    :param sigma: 标准差
    :return: 噪声图像 (uint8类型)
    """
    gauss = np.random.normal(mean, sigma, img.shape)
    noisy = img + gauss
    # 将像素值截断在 0-255 之间并转为 uint8
    noisy = np.clip(noisy, 0, 255).astype(np.uint8)
    return noisy

def add_salt_pepper_noise(img, s_vs_p=0.5, amount=0.05):
    """
    为图像添加椒盐噪声（脉冲噪声）
    :param img: 输入图像
    :param s_vs_p: 盐与椒的比例
    :param amount: 噪声密度
    :return: 噪声图像
    """
    out = np.copy(img)
    num_salt = np.ceil(amount * img.size * s_vs_p)
    coords = [np.random.randint(0, i - 1, int(num_salt)) for i in img.shape]
    out[coords[0], coords[1]] = 255 # 盐噪声 (白点)

    num_pepper = np.ceil(amount * img.size * (1. - s_vs_p))
    coords = [np.random.randint(0, i - 1, int(num_pepper)) for i in img.shape]
    out[coords[0], coords[1]] = 0   # 椒噪声 (黑点)
    return out

def avg_filter(img, ksize=3):
    """
    自行实现均值滤波器
    :param img: 输入灰度图像
    :param ksize: 卷积核大小 (如 3x3)
    :return: 滤波后的图像
    """
    h, w = img.shape
    # 创建填充后的图像 (padding)，这里使用零填充或复制边缘
    pad = ksize // 2
    img_padded = cv2.copyMakeBorder(img, pad, pad, pad, pad, cv2.BORDER_REPLICATE)
    
    result = np.zeros_like(img)
    
    # 遍历每一个像素
    for i in range(h):
        for j in range(w):
            # 提取 ksize x ksize 的邻域窗口
            region = img_padded[i:i+ksize, j:j+ksize]
            # 计算该窗口内像素的平均值
            mean_val = np.mean(region)
            result[i, j] = mean_val
            
    return result.astype(np.uint8)

def mid_filter(img, ksize=3):
    """
    自行实现中值滤波器
    :param img: 输入灰度图像
    :param ksize: 卷积核大小 (如 3x3)
    :return: 滤波后的图像
    """
    h, w = img.shape
    pad = ksize // 2
    img_padded = cv2.copyMakeBorder(img, pad, pad, pad, pad, cv2.BORDER_REPLICATE)
    
    result = np.zeros_like(img)
    
    # 遍历每一个像素
    for i in range(h):
        for j in range(w):
            # 提取邻域窗口
            region = img_padded[i:i+ksize, j:j+ksize]
            # 将窗口内的像素展平并排序，取中值
            median_val = np.median(region)
            result[i, j] = median_val
            
    return result.astype(np.uint8)

def laplacian_sharpen(img):
    """
    使用拉普拉斯算子实现锐化滤波
    :param img: 输入灰度图像
    :return: 锐化后的图像
    """
    # 拉普拉斯核
    kernel = np.array([[0, -1, 0],
                       [-1, 4, -1],
                       [0, -1, 0]])
    
    # 使用 filter2D 进行卷积操作
    laplacian = cv2.filter2D(img, -1, kernel)
    
    # 锐化 = 原图 + 拉普拉斯结果 (增强边缘)
    sharpened = cv2.add(img, laplacian)
    return sharpened

def main():
    # 1. 读取图像
    # 以灰度模式读取，方便处理
    img = cv2.imread(IMAGE_PATH, cv2.IMREAD_GRAYSCALE)
    
    if img is None:
        print(f"错误：无法找到图片 {IMAGE_PATH}，请检查路径。")
        return

    # 2. 添加噪声
    # 这里演示添加高斯噪声，你也可以调用 add_salt_pepper_noise 测试脉冲噪声
    img_gaussian = add_gaussian_noise(img)
    # img_salt_pepper = add_salt_pepper_noise(img) # 如需测试椒盐噪声取消注释

    # 3. 应用滤波器
    # 使用自定义的均值滤波和中值滤波
    img_avg = avg_filter(img_gaussian, ksize=3)
    img_mid = mid_filter(img_gaussian, ksize=3)
    
    # 锐化滤波通常作用于原图或去噪后的图
    img_sharp = laplacian_sharpen(img)

    # 4. 显示结果
    titles = ['Original Image', 'Gaussian Noise', 'Avg Filter (3x3)', 'Median Filter (3x3)', 'Laplacian Sharpen']
    images = [img, img_gaussian, img_avg, img_mid, img_sharp]

    plt.figure(figsize=(12, 8))
    for i in range(5):
        plt.subplot(2, 3, i+1)
        plt.imshow(images[i], cmap='gray')
        plt.title(titles[i])
        plt.xticks([]), plt.yticks([])
    
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    main()