#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<iostream>
#include <iomanip>
#include <math.h>
using namespace std;
//保留一位小数
double ro(double r) {
	double a = 10*r, b;
	b = (a > 0.0) ? floor(a + 0.5) : ceil(a - 0.5);
	return b/10 ;
}
//用结构体存放操作数 
struct Digit {
	int x;
	int y;
};
char ss[4] = { '+','-','x','/' }; //操作符
								  //返回操作符的优先级
int mPriority(char op) {
	if (op == '+') return 1;
	else if (op == '-') return 1;
	else if (op == 'x') return 2;
	else  return 2;
}
//求最大公约数
int gcd(int a, int b)
{
	return a == 0 ? b : gcd(b%a, a);
}
//根据操作符计算两个操作数
Digit Calculate(Digit num1, Digit num2, char op) {
	Digit res;
	int f;
	switch (op) {
	case '+': {
		res.x = num1.x*num2.y + num1.y*num2.x;
		res.y = num1.y*num2.y;
		break;
	}
	case '-': {
		res.x = num1.x*num2.y - num1.y*num2.x;
		res.y = num1.y*num2.y;
		break;
	}
	case 'x': {
		res.x = num1.x*num2.x;
		res.y = num1.y*num2.y;
		break;
	}
	case '/': {
		res.x = num1.x*num2.y;
		res.y = num1.y*num2.x;
		break;
	}
	default: {
		res.x = 0;
		res.y = 1;
	}
	}
	f = gcd(res.y, res.x);
	res.x = res.x / f;res.y = res.y / f;
	//如果分母为负数，取反
	if (res.y<0) {
		res.x = -res.x;
		res.y = -res.y;
	}
	return res;
}
//随机生成1个操作数并打印 
Digit getNum()
{
	int i, j, f;
	Digit res;
	if (rand() % 3 == 1)
	{
		i = rand() % 9 + 1;
		j = rand() % 9 + 1;
		if (i>j) { int temp = i;i = j;j = temp; }
		f = gcd(j, i);
		i = i / f;j = j / f;
		cout << i << "/" << j;
	}
	else {
		i = rand() % 99 + 1;
		j = 1;
		cout << i;
	}
	res.x = i;
	res.y = j;
	return res;
}
//随机生成一个操作符并打印
char getOperator() {
	char op = ss[rand() % 4];
	if (op == '/')
		cout << "÷";
	else if (op == 'x')
		cout << "×";
	else cout << op;
	return op;
}
int main()
{
	srand((unsigned)time(NULL));     //每次运行进行初始化 
									 //srand(time(0));
	int n, op_N = 0, r_op_N = 0;           //控制生成题目的个数 
	double score = 100;               //题目得分 
	cout << "请输入题目个数(例如输入:-n 5,将生成5个题目)：";
	cin >> n;
	while (n<1) {
		cout << "请输入正确的出题数目！" << endl;
		cin >> n;
	}
	cout << "本次共" << n << "题，满分100分" << endl;
	//第一个for循环，每次生成一个题目 
	for (int j = 0;j<n;j++) {
		cout << "第" << j + 1 << "题" << endl;
		int t = 0, q = 0, p = 0, top = 0;
		Digit opNum[11], numStack[10];
		char op, operatorStack[20], expQueue[30];
		opNum[q++] = getNum();
		expQueue[p++] = q - 1 + '0';//将整型转化为字符型存于字符数组中，以此记录运算符与操作数的顺序。
		//加入附加功能，运算符个数随机生成
		int op_Num =rand() %9 + 1 ;	
		for (t = 0;t<op_Num;t++)
		{
			op = getOperator();  //生成运算符  			
			if (t == 0) {
				operatorStack[top++] = op;
				opNum[q++] = getNum();
				expQueue[p++] = q - 1 + '0';
				continue;        //跳出循环
			}
			//根据运算符优先级排序，控制出栈顺序
			while (mPriority(op) <= mPriority(operatorStack[top - 1]) && top>0) {
				top--;
				expQueue[p++] = operatorStack[top];
			}
			operatorStack[top++] = op;
			opNum[q++] = getNum();//产生一个随机数 
			expQueue[p++] = q - 1 + '0';//将整型转化为字符型存于字符数组中
		}
		//将剩余的低优先级的依次取出放入expqueue中
		while (top>0) {
			top--;
			expQueue[p++] = operatorStack[top];
		}
		//根据后缀表达式计算结果 		
		top = 0;
		for (int i = 0;i<p;i++) {
			if (expQueue[i] >= '0'&&expQueue[i] <= ':')//
			{
				int ch = expQueue[i] - '0';//将存入的字符转化为整型的数，其值就是相应操作数在opNum中的坐标
				numStack[top].x = opNum[ch].x;
				numStack[top].y = opNum[ch].y;
				top++;
			}
			else {
				top--;
				numStack[top - 1] = Calculate(numStack[top - 1], numStack[top], expQueue[i]);
			}
		}
		cout << "=";
		//用户输入计算结果
		char  rightAns[100],userAns[100];
        cin >> userAns;
		//将结果存入字符数组
		if (numStack[top - 1].y != 1) {
			sprintf_s(rightAns, "%d%s", numStack[top - 1].x, "/");
			sprintf_s(rightAns, "%s%d", rightAns, numStack[top - 1].y);
		}
		else
			sprintf_s(rightAns, "%d", numStack[top - 1].x);
		//判断对错 
		if (strcmp(userAns, rightAns) == 0)
		{
			r_op_N += op_Num;
			cout << "正确！" << endl;
		}
		else {
			cout << "不正确！正确答案= " << rightAns << endl;
		}
		op_N += op_Num;
	}
	score = ro(100 * r_op_N / op_N);
	cout << "本次得分" << score;
	return 0;
}
