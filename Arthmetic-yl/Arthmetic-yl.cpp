#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<iostream>
#include <iomanip>
#include <math.h>
#include <windows.h>
using namespace std;
//保留一位小数
double ro(double r) {
	double a = 10*r, b;
	b = (a > 0.0) ? floor(a + 0.5) : ceil(a - 0.5);
	return b/10 ;
}
//用结构体存放操作数 
struct fraction {
	int a;
	int b;
};
char ope[4] = { '+','-','x','/' }; //操作符
//返回操作符的优先级
int Priority(char op) {
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
struct fraction Ca(struct fraction n1, struct fraction n2, char op) {
	struct fraction gb;
	int f;
	switch (op) {
	case '+': {
		gb.a = n1.a*n2.b + n1.b*n2.a;
		gb.b = n1.b*n2.b;
		break;
	}
	case '-': {
		gb.a = n1.a*n2.b - n1.b*n2.a;
		gb.b = n1.b*n2.b;
		break;
	}
	case 'x': {
		gb.a = n1.a*n2.a;
		gb.b = n1.b*n2.b;
		break;
	}
	case '/': {
		gb.a = n1.a*n2.b;
		gb.b = n1.b*n2.a;
		break;
	}
	default: {
		gb.a = 0;
		gb.b = 1;
	}
	}
	f = gcd(gb.a, gb.b);
	gb.a = gb.a / f;
	gb.b = gb.b / f;
    //如果分母为负数，取反
	if (gb.b<0) {
		gb.a = -gb.a;
		gb.b = -gb.b;
	}
	return gb;
}
//随机生成1个操作数并打印 
struct fraction getN()
{
	int i, j, f;
	struct fraction gn;
	if (rand() % 3 == 1)
	{
		i = rand() % 9 + 1;
		j = rand() % 9 + 1;
		f = gcd(j, i);
		i = i / f;
		j = j / f;
		cout << i << "/" << j;
	}
	else {
		i = rand() % 99 + 1;
		j = 1;
		cout << i;
	}
	gn.a = i;
	gn.b = j;
	return gn;
}
//随机生成一个操作符并打印
char getop() {
	char op = ope[rand() % 4];
	if (op == '/')
		cout << "÷";
	else if (op == 'x')
		cout << "×";
	else cout << op;
	return op;
}
int main()
{
	srand((unsigned)time(NULL));      //每次运行进行初始化 									
	int n, op_N = 0, r_op_N = 0;      //控制生成题目的个数 
	double score = 100, cost;               //题目得分 
	time_t start, end;
	cout << "请输入题目个数(例如输入:-n 5,将生成5个题目)：";
	cin >> n;
	while (n<1) {
		cout << "请输入正确的出题数目！" << endl;
		cin >> n;
	}
	cout << "本次共" << n << "题，满分100分" << endl;
	time(&start);
	//第一个for循环，每次生成一个题目 
	for (int j = 0;j<n;j++) {
		cout << "第" << j + 1 << "题" << endl;
		int t = 0, a = 0, b = 0, top = 0;
		struct fraction opN[11], numStack[11];
		char op, operatorStack[20], expQueue[30];
		opN[a++] = getN();
		expQueue[b++] = a - 1 + '0';//将整型转化为字符型存于字符数组中，以此记录运算符与操作数的顺序。
		//加入附加功能，运算符个数随机生成
		int op_Num =rand() %9 + 1 ;	
		for (t = 0;t<op_Num;t++)
		{
			op = getop();  //生成运算符  			
			if (t == 0) {
				operatorStack[top++] = op;
				opN[a++] = getN();
				expQueue[b++] = a - 1 + '0';
				continue;        //跳出循环
			}
			//根据运算符优先级排序，控制出栈顺序
			while (Priority(op) <= Priority(operatorStack[top - 1]) && top>0) {
				top--;
				expQueue[b++] = operatorStack[top];
			} 
			operatorStack[top++] = op;
			opN[a++] = getN();//产生一个随机数 
			expQueue[b++] = a - 1 + '0';//将整型转化为字符型存于字符数组中，记录操作数坐标
		}
		//将剩余的低优先级的依次取出放入expqueue中
		while (top>0) {
			top--;
			expQueue[b++] = operatorStack[top];
		}
		//根据后缀表达式计算结果 		
		top = 0;
		for (int i = 0;i<b;i++) {
			if (expQueue[i] >= '0'&&expQueue[i] <= ':')//
			{
				int ch = expQueue[i] - '0';//将存入的字符转化为整型的数，其值就是相应操作数在opNum中的坐标
				numStack[top].a = opN[ch].a;
				numStack[top].b = opN[ch].b;
				top++;
			}
			else {
				top--;
				numStack[top - 1] = Ca(numStack[top - 1], numStack[top], expQueue[i]);
			}
		}
		cout << "=";
		//用户输入计算结果
		char  rightAns[100],userAns[100];
		//将储存在numStack中的结果赋值到字符数组中，方便判断用户的答案是否正确
		if (numStack[top - 1].b != 1) {
			sprintf_s(rightAns, "%d%s", numStack[top - 1].a, "/");
			sprintf_s(rightAns, "%s%d", rightAns, numStack[top - 1].b);
		}
		else
			sprintf_s(rightAns, "%d", numStack[top - 1].a);
        cin >> userAns;
		//将结果存入字符数组
		
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
	time(&end);
	cost = difftime(end, start);
	//printf("%lf/n", cost);
	cout << "本次答题用时" << cost << "秒" << endl;
	score = ro(100 * r_op_N / op_N);
	cout << "本次得分" << score;
	return 0;
}
