# Make 24

[中文文档](README_CN.md)

## Introduction

Make 24 is a popular game among young people, with one or two players. 4 cards are randomly choosen, each of which represents 1 number.
 A is regarded as 1 point, and J, Q, K are all regarded as 1 point, or regarded as 11 point, 12 point, 13 point, respectively. The player tries to get 24 points on these 4 numbers by adding, subtracting, multiplying and dividing. Each number must be used and can only be used once.
 There are already a lot of software or apps available that provide make 24 calculation. As a fan of this game, I develop this project purely for self-entertainment and have no intention to compete with other software.

This project contains two versions of programs. One is developed by Python recently, the other is developed by Visual
C++ many years ago. These two versions have significant differences, and have nothing to do with each other.

Suggestions are welcome. In addition to submitting an issue, you can email me as well. My email address is 31416\@msn.cn.

## Python Edition

Developed under Python 3.6 and used as a command line. For example, to calculate 1, 2, 3, and 4, execute the command "Python make24.py 1 2 3 4". Then the console outputs expressions that get 24 points.

Python edition features：

1. The code is concise, with less than 100 lines.

2. All answers are listed with no duplication.

3. Expressions contain as few parentheses as possible.

4. Fraction can be used if necessary.

## Visual C++ Edition

Developed under Visual C++2005. WTL is used for developing UI. Card pictures are borrowed from the official card game in Windows operating system.

In each round, the computer deals cards, and the user tries to calculate by mouse. Please see [Help document](vc/help_cn.md)。Currently, only chinese help document is provided.

Visual C++ edition features：

1. No keyboard input is required, and calculatation can be done by very few mouse clicks and drags and drops.

2. Answers are provided.

3. Different difficulty levels are supported.

4. Fraction can be used if necessary.

5. Both Chinese and English UI are available.
