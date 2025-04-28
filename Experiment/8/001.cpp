// 第8个实验：文件格式解析
// 目的：练习对已知文件格式文件的读取和写入。
// 要求：
// 1）编写一个程序，将一个文本文件拷贝到另外一个文件中。
// 2）创建一个文本文件，在文件中输出一个九九乘法表。
// 3）解析一个bmp文件的文件格式信息。并且拷贝到另外一个文件中。将图片中心的9*9像素区域设置为红绿蓝间隔快。

#include <iostream>
#include <fstream>
#include <vector>

void copyTextFile(const std::string &source, const std::string &destination)
{
    std::ifstream inFile(source);
    std::ofstream outFile(destination);

    if (!inFile || !outFile)
    {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inFile, line))
    {
        outFile << line << '\n';
    }

    inFile.close();
    outFile.close();
}

void createMultiplicationTable(const std::string &filename)
{
    std::ofstream outFile(filename);

    if (!outFile)
    {
        std::cerr << "Error creating file!" << std::endl;
        return;
    }

    for (int i = 1; i <= 9; ++i)
    {
        for (int j = 1; j <= 9; ++j)
        {
            outFile << i << " * " << j << " = " << i * j << '\t';
        }
        outFile << '\n';
    }

    outFile.close();
}

void modifyBmpFile(const std::string &source, const std::string &destination)
{
    std::ifstream inFile(source, std::ios::binary);
    std::ofstream outFile(destination, std::ios::binary);

    if (!inFile || !outFile)
    {
        std::cerr << "Error opening BMP file!" << std::endl;
        return;
    }

    std::vector<char> header(54);
    inFile.read(header.data(), header.size());
    outFile.write(header.data(), header.size());

    int width = *reinterpret_cast<int *>(&header[18]);
    int height = *reinterpret_cast<int *>(&header[22]);
    int rowSize = ((width * 3 + 3) / 4) * 4;

    std::vector<char> pixelData(rowSize * height);
    inFile.read(pixelData.data(), pixelData.size());

    int centerX = width / 2;
    int centerY = height / 2;

    for (int y = centerY - 4; y <= centerY + 4; ++y)
    {
        for (int x = centerX - 4; x <= centerX + 4; ++x)
        {
            int index = y * rowSize + x * 3;
            if (x % 3 == 0)
            {
                pixelData[index] = 255;
                pixelData[index + 1] = 0;
                pixelData[index + 2] = 0;
            }
            else if (x % 3 == 1)
            {
                pixelData[index] = 0;
                pixelData[index + 1] = 255;
                pixelData[index + 2] = 0;
            }
            else if (x % 3 == 2)
            {
                pixelData[index] = 0;
                pixelData[index + 1] = 0;
                pixelData[index + 2] = 255;
            }
        }
    }

    outFile.write(pixelData.data(), pixelData.size());

    inFile.close();
    outFile.close();
}

int main()
{
    copyTextFile("source.txt", "destination.txt");
    createMultiplicationTable("multiplication_table.txt");
    modifyBmpFile("source.bmp", "destination.bmp");

    return 0;
}