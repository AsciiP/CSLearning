import java.io.*;
import java.util.Scanner;

public class IOFileXCV {
    public static void main(String[] args) {
        try (Scanner sc = new Scanner(System.in)) {
            System.out.println("请输入完整的文件路径（包含文件扩展名）");
            String srcPath = sc.nextLine().trim();

            System.out.println("请选择以下任意操作...");
            System.out.println("A 复制-粘贴图片");
            System.out.println("B 剪切-粘贴文本文件");
            System.out.println("请输入操作对应的字母（请注意大小写）...");
            String choice = sc.nextLine().trim();

            if (!"A".equals(choice) && !"B".equals(choice)) {
                System.out.println("无效的操作字母，程序结束。");
                return;
            }

            System.out.println("请输入要保存的文件路径（包含完整文件名）...");
            String destPath = sc.nextLine().trim();

            File src = new File(srcPath);
            if (!src.exists() || !src.isFile()) {
                System.out.println("源文件不存在或不是文件，程序结束。");
                return;
            }

            try {
                if ("A".equals(choice)) {
                    try (BufferedInputStream bis = new BufferedInputStream(new FileInputStream(src));
                         BufferedOutputStream bos = new BufferedOutputStream(new FileOutputStream(destPath))) {
                        byte[] buf = new byte[8192];
                        int len;
                        while ((len = bis.read(buf)) != -1) {
                            bos.write(buf, 0, len);
                        }
                        bos.flush();
                    }
                    System.out.println("图片复制完成。");
                } else {
                    // 要求：利用字节输入输出流实现文本文件相关操作（剪切）
                    try (FileInputStream fis = new FileInputStream(src);
                         FileOutputStream fos = new FileOutputStream(destPath)) {
                        byte[] buf = new byte[8192];
                        int len;
                        while ((len = fis.read(buf)) != -1) {
                            fos.write(buf, 0, len);
                        }
                    }
                    // 删除源文件以实现剪切
                    if (src.delete()) {
                        System.out.println("文本文件剪切并粘贴完成。");
                    } else {
                        System.out.println("文件复制成功，但删除源文件失败。");
                    }
                }
            } catch (IOException e) {
                System.out.println("发生错误: " + e.getMessage());
            }
        }
    }
}