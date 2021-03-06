#include "pch.h"
#pragma warning(disable:4996)
#include<iostream>
#include<cstring>
#include<string>
#include<vector>
#include<ctime>
#include<cmath>
#include<fstream>
#include<algorithm>
using namespace std;

int map[9][9] = { {2} };

int move_up[2] = { 3,6 };
int move_mid[3] = { 2,5,8 };
int move_down[3] = { 1,4,7 };

int first_row[9] = { 1,2,3,4,5,6,7,8,9 };

int num;

char OutputData[200000000];
int p = 0;

int vis[3][10][10];

void Creat_Sudoku(int count)
{
	//下面是对每一行向右移动的列数进行排列和对第一行数进行排列
	if (count >= 1)
	{
		next_permutation(move_down, move_down + 2);
	}

	if (count % 6 == 0 && count != 0)
	{
		next_permutation(move_mid, move_mid + 2);
	}

	if (count % 36 == 0 && count != 0)
	{
		next_permutation(move_up + 1, move_up + 2);
	}

	if (count % 72 == 0 && count != 0)
	{
		next_permutation(first_row + 1, first_row + 8);
	}

	int col;

	for (col = 0; col < 9; col++)
	{
		map[0][col] = first_row[col];
	}

	for (col = 0; col < 9; col++)   //用第一行生成其他行
	{
		map[1][(col + move_up[0]) % 9] = map[0][col];
		map[2][(col + move_up[1]) % 9] = map[0][col];
		map[3][(col + move_mid[0]) % 9] = map[0][col];
		map[4][(col + move_mid[1]) % 9] = map[0][col];
		map[5][(col + move_mid[2]) % 9] = map[0][col];
		map[6][(col + move_down[0]) % 9] = map[0][col];
		map[7][(col + move_down[1]) % 9] = map[0][col];
		map[8][(col + move_down[2]) % 9] = map[0][col];
	}

	int i, j; 
	//将结果读入超大数组内
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			if (j != 8)
			{
				OutputData[p++] = map[i][j] + '0';
				OutputData[p++] = ' ';
			}
			else
			{
				OutputData[p++] = map[i][j] + '0';
				if (count == num - 1 && i == 8)
				{

				}
				else
				{
					OutputData[p++] = '\n';
				}
			}
		}

	}

	if (count != num - 1)
	{
		OutputData[p++] = '\n';
	}
}


int res[9][9];
int suc;

void SetVis(int r, int c, int num)  //第r行，第i列和对应九宫格中已经有数num，则使用该函数
{
	vis[0][r][num] = 1;
	vis[1][c][num] = 1;
	vis[2][r / 3 * 3 + c / 3][num] = 1;
}

void ResetVis(int r, int c, int num) //第r行，第i列和对应九宫格中没有数num，则使用该函数
{
	vis[0][r][num] = 0;
	vis[1][c][num] = 0;
	vis[2][r / 3 * 3 + c / 3][num] = 0;
}

bool CheckCanVis(int r, int c, int num)  //检查第r行，第i列是否能放数num
{
	if (vis[0][r][num] == 0 && vis[1][c][num] == 0 && vis[2][r / 3 * 3 + c / 3][num] == 0)
		return true;
	else
		return false;
}

void prt()    //将结果输入大数组中
{
	int i, j;
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			if (j != 8)
			{
				OutputData[p++] = res[i][j] + '0';
				OutputData[p++] = ' ';
			}
			else
			{
				OutputData[p++] = res[i][j] + '0';
				if(i!=8)
				OutputData[p++] = '\n';
			}
		}

	}
}

void TraceBack(int n)
{
	if (suc == 1)
		return;

	if (n > 80)  //代表解完当前数独
	{
		prt();
		suc = 1;
		return;
	}

	if (res[n / 9][n % 9] != 0)   //当前格子有数字，跳到下一格
	{
		TraceBack(n + 1);
	}
	else
	{
		for (int i = 1; i <= 9; i++)
		{
			int f = CheckCanVis(n / 9, n % 9, i);

			if (f == 0)
				continue;
			else
			{
				res[n / 9][n % 9] = i;
				SetVis(n / 9, n % 9, res[n / 9][n % 9]);

				TraceBack(n + 1);

				ResetVis(n / 9, n % 9, res[n / 9][n % 9]);
				res[n / 9][n % 9] = 0;
			}
		}
	}
}



void Solve_Sudoku(char input[])
{
	FILE *fp1, *fp2;
	fopen_s(&fp1, input, "r");
	fopen_s(&fp2, "sudoku.txt", "w");

	int t = 0;
	int count = 0;
	while (fscanf(fp1, "%d", &res[t / 9][t % 9]) != EOF)  //判断是否到文件尾
	{
		if (count != 0)
		{
			OutputData[p++] = '\n';
			OutputData[p++] = '\n';
		}

		for (t = 1; t < 81; t++)     //读入一个需要求解的数独
			fscanf(fp1, "%d", &res[t / 9][t % 9]);

		suc = 0;

		memset(vis, 0, sizeof(vis));

		for (t = 0; t < 81; t++)
		if (res[t / 9][t % 9] != 0)   //当前格子有数字，跳到下一格
		{
			SetVis(t / 9, t % 9, res[t / 9][t % 9]);
		}

		TraceBack(0);   //回溯求解

		count++;

		t = 0;
	}

	OutputData[p++] = '\0';

	fwrite(OutputData, 1, p, fp2);  //将结果写入文件

	fclose(fp1);
	fclose(fp2);
}



int main(int argc, char **argv)
{

	if (argc != 3)    //命令行字符串数量不是3
	{
		printf("命令格式有误，请重新输入命令\n");
	}
	else
	{
		if (argv[1][1] == 'c')   //命令行第二个命令是-c
		{
			int i = 0;
			int f = 0;

			while (argv[argc - 1][i] != '\0')
			{
				if (argv[argc - 1][i] >= '0' && argv[argc - 1][i] <= '9')
				{
					i++;
				}
				else
				{
					f = 1;
					break;
				}
			}   //判断第三个字符串中是否有非数字字符

			if (f == 1)
			{
				printf("输入数字中有非数字字符！\n");
			}
			else
			{
				num = atoi(argv[argc - 1]);         //将数字字符串参数转化为数字。

				if (num >= 1 && num <= 1000000)                    //检验输入值是否越界。 
				{
					int k = 0;

					for (k = 0; k < num; k++)   //生成数独
					{
						Creat_Sudoku(k);
					}

					OutputData[p++] = '\0';

					FILE *fp;
					fopen_s(&fp, "sudoku.txt", "w");
					fwrite(OutputData, 1, strlen(OutputData), fp);  //将所有数独写入文件
					fclose(fp);


				}
				else
				{
					printf("输入的数据应在1到1000000之间\n");
				}
			}
		}
		else
		{
			if (argv[1][1] == 's')  //解数独
			{
				Solve_Sudoku(argv[2]);
			}
			else
			{
				printf("有效的命令应为-c或-s\n");
			}
		}
	}

	return 0;

}
