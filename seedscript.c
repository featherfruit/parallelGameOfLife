#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main()
{

	int rows = 300;
	int cols = 300;
	char **matrix = (char **)malloc(rows * sizeof(char *));
		for (int i = 0; i < rows; i++)
		{
			matrix[i] = malloc(sizeof(char) * cols);
		}


	for (int i = 0; i < rows; i++)
			for(int j = 0; j < cols; j++)
				matrix[i][j] = "-*"[random () % 2];

	// for (int i = 0; i < rows; i++)
	// {
	// 		for(int j = 0; j < cols; j++){
	// 			printf("%c",matrix[i][j]);
	// 		}
	// 	printf("\n");
	// }
		

	FILE *outFPtr = fopen("seed", "w");
	for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
				fprintf(outFPtr, "%c", matrix[i][j]);

			fprintf(outFPtr, "\n");
		}

	for (int i = 0; i < rows; i++)
		free(matrix[i]);
		free(matrix);
		
		return 0;

}
