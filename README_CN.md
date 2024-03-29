# <img src="vc/res/Make24.ico"/> 计算24点

## 介绍

计算24点是一种在年轻人中比较流行的单人或双人益智游戏。玩家随机抽出4张扑克牌，设法把牌上的4个数通过加减乘除得到24点。每张牌必须且只能使用一次。A视作1点，大小王也视作1点，或不使用，J、Q、K或视为1点，或分别视为11、12、13，这由玩家约定。市面上已经有很多提供计算24点功能的软件或APP。作为计算24点的爱好者，开发此项目纯属自娱自乐，无意与其他软件竞争。

本项目包含3个版本的计算24点程序。一个是很多年前用用Visual
C++开发，另外两个分别是最近用Python和SQL开发的。3个版本之间没有任何关联，其中只有C++版的是游戏，另外两个只能算工具。

欢迎提出改进建议和参与改进。除了提交issue外，也可通过邮箱31416\@msn.cn和我交流。

## Python版和SQL版

Python版在Python3.6下面开发，以命令行方式使用。如给定1、2、3、4这4个数进行计算，则执行命令"Python make24.py 1 2 3 4"，控制台输出计算24点的答案。

SQL版为SQL脚本，执行SQL可输出计算24所有的答案，约有1500多行。脚本基于MySQL的语法，稍作修改可移植到Oracle、SQL Server等其他数据库。

这两个版本的特点：

1. 代码简洁，共计100行左右代码。

2. 列出所有答案，并且不重复。

3. 答案中包含尽可能少的括号。

4. 支持必要时用分数进行计算。

## VC版

在Visual C++
2005下面开发，使用WTL开发界面，扑克图片借用Windows操作系统自带的纸牌游戏中的资源组件。使用时，电脑出题，用户通过鼠标点击和拖放进行计算。详见[帮助文档](vc/help_cn.md)。

VC版的特点：

1. 不需要键盘输入，支持通过鼠标点击和拖放的方式计算24点，最多只需要拖放3次。

2. 可查看答案。

3. 支持不同的难度等级。

4. 支持用分数进行计算。

5. 提供中英两种界面。
