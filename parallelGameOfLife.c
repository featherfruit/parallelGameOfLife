/*
	Author:Featherfruit on Github
	Conways Parallel Game Of Life in C.
	Rules-Check serial progam file in the same repository
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>

/*
	*  =  alive
	"-"=  dead
	please check serial version for detailed comments :*
*/
int main(int argc, char *argv[])
{
	if (argc != 7)
	{
		printf("Invaild  number of Arguments entered \n");
		return -1;
	}

	int rows, cols, steps,num_thread;
	rows = atoi(argv[2]);
	cols = atoi(argv[3]);
	steps = atoi(argv[4]);
	num_thread = atoi(argv[6]);

	srand( time(NULL) );
	struct timeval start, end;
	gettimeofday(&start, NULL);
	//start

	char **matrix = (char **)malloc(rows * sizeof(char *));
	for (int i = 0; i < rows; i++)
	{
		matrix[i] = malloc(sizeof(char) * cols);
	}

	void writeToFile(FILE *, char **, int, int);
	int aliveNoOfNeighbours(char **, int, int, int, int);
	void matrixCopy(char **, char **, int, int);

	FILE *fp = fopen(argv[1], "r");

	if (fp == NULL)
	{
		printf("no file to open");
		return 0;
	}

	char temp_ch;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			fscanf(fp, "%c", &matrix[i][j]); // i and j represent the coordinates on the grid
			if (j == cols - 1)
				fscanf(fp, "%c", &temp_ch);
		}
	}
	fclose(fp);
	//*copy of the grid that will be modified at each step*
	char **copy = (char **)malloc(rows * sizeof(char *));
	for (int i = 0; i < rows; i++)
	{
		copy[i] = malloc(sizeof(char) * cols);
	}

	// logic for game
	for (int iterations = 1; iterations <= steps; iterations++)
	{
		matrixCopy(copy, matrix, rows, cols);

		int i,j;
		omp_set_dynamic(0);
		omp_set_num_threads(num_thread);
		#pragma omp parallel for collapse(2) shared(matrix,copy) private(i,j)
		for (i = 0; i < rows; i++)
		{
			for (j = 0; j < cols; j++)
			{
				// only matrix is modified while copy is only used as refrence for alive no of cells

				// if your a live cell these are your moves
				int Bliveneighbours = aliveNoOfNeighbours(copy, rows, cols, i, j);
				if (copy[i][j] == '*')
				{
					if (Bliveneighbours < 3)
					{
						matrix[i][j] = '-';
						// live cell dies by underpopulation
					}

					else if (Bliveneighbours > 4)
					{
						matrix[i][j] = '-';
						// live cell dies by overpopulation
					}
				}

				// if your a dead cell this is your only move
				else if (copy[i][j] == '-')
				{
					if (Bliveneighbours == 3)
						matrix[i][j] = '*';
					// dead cell comes to life as it has 3 neighbours
				}
			}
		}
	}

	FILE *outFPtr = fopen(argv[5], "w");
	writeToFile(outFPtr, matrix, rows, cols);

	//free the memory

	for (int i = 0; i < rows; i++)
		free(matrix[i]);
	free(matrix);

	for (int i = 0; i < rows; i++)
	free(copy[i]);
	free(copy);

	//end of execution
	gettimeofday(&end, NULL);

	//execution time calculation
	float exec_time = ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
	exec_time /= 1000000;
	printf("Time taken to execute: %fs\n\n", exec_time);

	return 0;
}

// functions

// function(1) to return no of neighbours operates on the matrix corresponding to the step
int aliveNoOfNeighbours(char **matrix, int rows, int cols, int i, int j)
{
	// printf("Here!\n");
	int count = 0;

	// corner cases no of neighbours = 3
	if (i == 0 && j == 0)
	{
		for (int a = 0; a <= 1; a++)
		{
			for (int b = 0; b <= 1; b++)
			{
				if (matrix[a][b] == '*')
				{
					count = count + 1;
				}
			}
		}
	}

	else if (i == rows - 1 && j == 0)
	{
		for (int a = rows - 2; a <= rows - 1; a++)
		{
			for (int b = 0; b <= 1; b++)
			{
				if (matrix[a][b] == '*')
				{
					count = count + 1;
				}
			}
		}
	}
	else if (i == rows - 1 && j == cols - 1)
	{
		for (int a = rows - 2; a <= rows - 1; a++)
		{
			for (int b = cols - 2; b <= cols - 1; b++)
			{
				if (matrix[a][b] == '*')
				{
					count = count + 1;
				}
			}
		}
	}
	else if (i == 0 && j == cols - 1)
	{
		for (int a = 0; a <= 1; a++)
		{
			for (int b = cols - 2; b <= cols - 1; b++)
			{
				if (matrix[a][b] == '*')
				{
					count = count + 1;
				}
			}
		}
	}

	// edgecases
	else if (i == 0 && j != cols - 1 && j != 0)
	{
		for (int a = 0; a <= 1; a++)
		{
			for (int b = j - 1; b <= j + 1; b++)
			{
				if (matrix[a][b] == '*')
				{
					count = count + 1;
				}
			}
		}
	}

	else if (i == rows - 1 && j != cols - 1 && j != 0)
	{
		for (int a = rows - 2; a <= rows - 1; a++)
		{
			for (int b = j - 1; b <= j + 1; b++)
			{
				if (matrix[a][b] == '*')
				{
					count = count + 1;
				}
			}
		}
	}

	else if (j == 0 && i != 0 && i != rows - 1)
	{
		for (int a = i - 1; a <= i + 1; a++)
		{
			for (int b = 0; b <= 1; b++)
			{
				if (matrix[a][b] == '*')
				{
					count = count + 1;
				}
			}
		}
	}

	else if (j == cols - 1 && i != 0 && i != rows - 1)
	{
		for (int a = i - 1; a <= i + 1; a++)
		{
			for (int b = cols - 2; b <= cols - 1; b++)
			{
				if (matrix[a][b] == '*')
				{
					count = count + 1;
				}
			}
		}
	}

	// generalcases no of neighbours = 8
	else
	{
		for (int a = i - 1; a <= i + 1; a++)
		{
			for (int b = j - 1; b <= j + 1; b++)
			{
				if (matrix[a][b] == '*')
				{
					count = count + 1;
				}
			}
		}
	}
	return count;
}

// function to write to output file(3)
void writeToFile(FILE *outFPtr, char **matrix, int rows, int cols)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
			fprintf(outFPtr, "%c", matrix[i][j]);

		fprintf(outFPtr, "\n");
	}
}

void matrixCopy(char **copy, char **matrix, int rows, int cols)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
			copy[i][j] = matrix[i][j];
	}
}