#include <stdio.h>
#include <windows.h>
#include <math.h>

void outputMatrix(float **M, int n, int m)
{
	printf("\n");
	for (int i = 0; i < n; i++)
	{
		printf("\t");
		for (int j = 0; j < m; j++)
		{
			printf("%5.3f\t", M[i][j]);
		}
		printf("\n\n");
	}
}

float ** ChangePosition(float **M, int n1, int n2, int m)
{
	float tmp;
	for (int j = 0; j < m; j++)
	{
		tmp = M[n1][j];
		M[n1][j] = M[n2][j];
		M[n2][j] = tmp;
	}
	return M;
}

float ** EntryMatrix(int *n, int *m)
{
	float d = 1;
	float **M;
	printf("Введите кол-во переменных и строк:\n");
	
	while (scanf("%d %d", m, n) != 2)
	{
		printf("Не удалось прочитать переменные, повторите попытку\n");
		scanf("%*[^\n]");
	}
	
	(*m)++;
	
	printf("\nВведите коэффициенты a и b из матрицы:\n");

	M = (float**)malloc(sizeof(float*) * (*n));
	for (int i = 0; i < (*n); i++)
	{
		M[i] = (float*)malloc(sizeof(float) * (*m));
		for (int j = 0; j < (*m); j++)
		{
			if (j == *m - 1)
				printf("b%d : ", i + 1);
			else
				printf("a%d%d : ", i + 1, j + 1);

			if (scanf("%f", &d) != 1)
			{
				printf("Не удалось прочитать переменную, повторите попытку\n");
				scanf("%*[^\n]");
				j--;
			}
			else
				M[i][j] = d;
		}
	}
	
	system("cls");
	printf("Введённая матрица:\n");
	outputMatrix(M, *n, *m);
	printf("Ввести матрицу заново? (1 - Да; 0 - Нет): ");
	return M;
}

int main(int argc, char** argv) {
	system("title Lab1.1 #24");
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);

	float** M;		//	Матрица
	int n = 0; 		//	количество строк
	int m = 0; 		//	количество столбцов
	int flag = 1;	//	Флаг
	float* resM;	//	Ответ для каждой переменной
	int independ;	//	Разница кол-ва переменных и кол-ва строк
	int quant;		//	
	float tmp;		//	
	
	while (flag != 0)
	{
		M = EntryMatrix(&n, &m);
		while (scanf("%d", &flag) != 1)
		{
			printf("Не удалось прочитать переменную, повторите попытку\n");
			scanf("%*[^\n]");
		}
	}
	
	independ = (m - 1) - n;
	if (independ > 0)
		printf("\nКак минимум %d переменных могут быть независимые\n\n", (m - 1) - n);
	
	resM = (float*)malloc(sizeof(float*) * (m - 1));
	for (int i = 0; i < m - 1; i++)
	{
		resM[i] = 0;
	}
	
	//	Приведение к треугольному виду
	for (int i = 0; i < n; i++)
	{
		//	Обмен со строкой, где число больше по модулю
		for (int itmp = i + 1; itmp < n; itmp++)
		{
			if (fabs(M[itmp][i]) > fabs(M[i][i]))
				M = ChangePosition(M, i, itmp, m);
		}
		if (M[i][i] != 0)
		{
			tmp = M[i][i];
			quant = n - i - 1;
			for (int j = 0; j < m; j++)
			{
				M[i][j] = M[i][j]/tmp;	
			}
			for (quant; quant != 0; quant--)
			{
				tmp = -(M[n - quant][i] / M[i][i]);
				for (int j = 0; j < m; j++)
				{
					M[n - quant][j] = M[n - quant][j] + (M[i][j] * tmp);
				}
			}
		}
	}

	printf("Матрица, приведенная к треугольному виду:\n\n");
	outputMatrix(M, n, m);

	//	Поиск неизвестных
	for (int i = n-1; i >= 0; i--)
	{
		tmp = M[i][m - 1];
		for (int j = m-2; j >= i; j--)
		{
			if (M[i][j] != 0 && i != j)
				tmp -= (M[i][j] * resM[j]);
			if (i == j)
			{
				if (M[i][j] == 0)
					resM[i] = 0;
				else
					resM[i] = tmp / M[i][j];
			}
		}
	}
	
	for (int i = 0; i < m-1; i++)
	{
		tmp = (float)independ;
		if (i < n)
		{
			printf("\nx%d = %3.3f", i + 1, resM[i]);
			if (resM[i] == 0)
				continue;
		}
		else
		{
			printf("\nx%d - Независимая", i + 1);
			tmp = 0;
		}
		if (tmp > 0)
		{
			for (int j = m-2; j >=i ; j--)
			{
				if (signbit(M[i][j]))
					printf(" + (%3.3fx%d / %3.3f)", fabs(M[i][j]), j + 1, M[i][i]);
				else
					printf(" - (%3.3fx%d / %3.3f)", M[i][j], j + 1, M[i][i]);
						
				for (int itmp = m-2; itmp > i; itmp--)
				{
					if (itmp <= n-1 && M[itmp][itmp] != 0)
					{
						printf(" + (%3.3f) * x%d(независимая часть)", M[i][itmp], itmp+1);
					}
				}
				tmp--;
				if (tmp == 0)
					break;
			}
		}
	}
	printf("\n");
	system("pause");
	return 0;
}