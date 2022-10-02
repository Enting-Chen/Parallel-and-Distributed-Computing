#include <iostream>
#include <omp.h>
#include <time.h>
using namespace std;

#define size 1000
#define ThreadNumber 5

int first[size][size]; //矩阵
int second[size];	   //向量
int result[size];	   //结果

void calculate(int row) //计算单元的乘积
{
	int r = 0;
	for (int i = 0; i < size; i++)
	{
		r += first[row][i] * second[i];
	}
	result[row] = r;
}

void Initialize() //矩阵、向量初始化
{
#pragma omp parallel for num_threads(ThreadNumber) //并行
	for (int row = 0; row < size; row++)
	{
		for (int col = 0; col < size; col++)
			first[row][col] = rand() % 10;
		second[row] = rand() % 10;
	}
}

void Multiplication() //矩阵乘法
{
	#pragma omp parallel for num_threads(ThreadNumber) \ 
	schedule(dynamic, 100) //动态调度
	//schedule(static, 10) //静态调度
	for (int row = 0; row < size; row++)
	{
		calculate(row);
	}
}

int main()
{
	clock_t sum = 0;
	for (int i = 0; i < 10; i++)
	{
		Initialize();
		clock_t t1 = clock();
		Multiplication();
		clock_t t2 = clock();
		// cout << "time: " << t2 - t1 << endl;
		sum += t2 - t1;
		//system("read -p 'Press Enter to continue...' var");
	}
	cout << "Thread number: " << ThreadNumber << endl;
	cout << "Average time: " << sum / 100 << endl;
	return 0;
}
