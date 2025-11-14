# 成绩计算器示例

该示例包含一个通用接口 `GradeCalculator`，并提供两种实现：唱歌比赛评分和学生成绩按权重计算。示例位于 `src/com/example/grades`。

编译与运行（在项目根目录）：

```powershell
javac -d bin -sourcepath src src/com/example/grades/*.java
java -cp bin com.example.grades.Main
```

接口契约简述：
- `double calculate()` 返回最终分数（通常 0 - 100）。实现应在接收到非法输入时抛出 `IllegalArgumentException`。

扩展建议：
- 为 `GradeCalculator` 添加通用的异常类型或结果对象，包含元数据（例如是否合格、分位数）。
- 为 `SingingContestCalculator` 添加裁剪 N 个极端值、四舍五入策略等配置。
- 为 `StudentGradeCalculator` 添加缺失项处理和基于最大分数的归一化选项。
## Getting Started

Welcome to the VS Code Java world. Here is a guideline to help you get started to write Java code in Visual Studio Code.

## Folder Structure

The workspace contains two folders by default, where:

- `src`: the folder to maintain sources
- `lib`: the folder to maintain dependencies

Meanwhile, the compiled output files will be generated in the `bin` folder by default.

> If you want to customize the folder structure, open `.vscode/settings.json` and update the related settings there.

## Dependency Management

The `JAVA PROJECTS` view allows you to manage your dependencies. More details can be found [here](https://github.com/microsoft/vscode-java-dependency#manage-dependencies).
