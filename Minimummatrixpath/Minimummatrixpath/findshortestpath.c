#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Takes a file in the following format
2
2,3
1,2
3
1,2,3
4,5,6
7,8,9
...
First number is the length of the nxn matrix the rest until the next length specifier is the values contained in the matrix

*/


typedef struct Path Path;

struct Path /* Linked list of path coordinates*/
{
	int x; //x-coordinate of the path
	int	y; //y-coordinate of the path
	int pathsum; // Sum of the steps passed over
	Path* next_path; //Pointer to the next member of the linked list
};
/*
Allocates memory for the matrix
*/
int ** allocMatrix(int length)
{
	int i;
	int **x;
	x = malloc(length * sizeof x);
	for (i = 0; i < length; i++)
	{
		x[i] = malloc(length * sizeof *x[i]);
	}
	return x;
}
/*
Creates a new path linked list member and returns the pointer to it, by default the next path is set to null (appends to the end of the list)
*/
Path* newPath(int x, int y, int pathsum)
{
	Path* path_pointer;
	if ((path_pointer = malloc(sizeof *path_pointer)) != NULL)
	{
		path_pointer->x = x;
		path_pointer->y = y;
		path_pointer->pathsum = pathsum;
		path_pointer->next_path = NULL;
	}


	return path_pointer;
}
/*Checks if the path can move both to the right and to the left
0 - no
1 - yes
*/
int checkSplit(int x, int y, int length)
{
	if ((x < length - 1) && (y < length - 1)) return 1;
	return 0;
}

/*
Finds the path with the smallest matrix sum by checking each possible permutation
The algorithm starts at the topmost left corner. Each cycle it checks if it can move down or right, if both it splits into two path
objects and moves the current path right otherwise it moves either right or down

Returns the smallest matrix sum at completion

*/
int findShortestpath(int** matrix, int length)
{
	int xcurr; //Current paths x coordiant
	int ycurr; //Current paths y coordinate
	int smallest; //Smallest sum;
	int end = 0; //Control variable for when all paths have reached the bottom right corner
	int no_of_paths = 0; //Number of currently active paths (number of members in the linked list)
	Path *root; //Root node in the linked list
	Path *currpath; //Current node
	Path *temp_path; //Temporary storage for a new node
	root = newPath(0, 0, matrix[0][0]); /*Create path identifier in the top left corner*/
	no_of_paths++;
	currpath = root;

	while (end != no_of_paths) /* Loop as long as all paths are not in the bottom right corner*/
	{
		end = 0;
		do /*Loop through all nodes*/
		{
			xcurr = currpath->x;
			ycurr = currpath->y;
			if (checkSplit(xcurr, ycurr, length) == 1) /* If path is splittable (can move in x and y directions) split the path and create a new path that moves down*/
			{
				temp_path = newPath(xcurr, ycurr + 1, currpath->pathsum + matrix[xcurr][ycurr + 1]); /* New path*/
				if(currpath->next_path == NULL) /*New path might need to be inserted into the linked list*/
				{
					currpath->next_path = temp_path;
				}else /*Needs to be inserted*/
				{
					temp_path->next_path = currpath->next_path;
					currpath->next_path = temp_path;
				}
				
				no_of_paths++;
				xcurr++; /* Old path moves right*/
				currpath->pathsum += matrix[xcurr][ycurr];
			}
			else /* If not splittable move in the direction possible*/
			{
				if (xcurr < length - 1)
				{
					xcurr++;
					currpath->pathsum += matrix[xcurr][ycurr];
				}
				else if(ycurr < length - 1)
				{
					ycurr++;
					currpath->pathsum += matrix[xcurr][ycurr];
				}
			}
			currpath->x = xcurr;
			currpath->y = ycurr;
			
			if (currpath->x == length - 1 && currpath->y == length - 1) /*Check if path is at the end, if all paths are at the end this will end the while loop*/
			{
				end++;
			}
			else
			{
				end = 0;
			}

		}
		while ((currpath = currpath->next_path) != NULL); /*Loop through linked list of paths*/

		currpath = root; /*Reset linked list*/

	}

	smallest = root->pathsum;
	do /*Loop through to check for the smallest sum*/
	{
		if (smallest > currpath->pathsum)
		{
			smallest = currpath->pathsum;
		}
	}
	while ((currpath = currpath->next_path) != NULL); /* Loop through linked list of paths*/
	return smallest;
}


int main(int argc, char** args[])
{

	FILE* file = fopen(args[1], "r");
	if(file == NULL)
	{
		printf("Cannot find file!");
		return 0;
	}
	char line[1024];
	char* tok;
	int** matrix;
	int length = 0;
	int i, j;
	int shortest;
	int lineno = 1;
	int new_matrix = 1;



	while(fgets(line,1024,file))
	{	
		if( new_matrix == 1)
		{
			sscanf(line, "%d", &length);
			matrix = allocMatrix(length);
			new_matrix = 0;
			i = 0;
		}
		else
		{
			j = 0;
			tok = strtok(line, ",");
			while (tok != NULL)
			{
				sscanf(tok, "%d", &matrix[i][j]);
				j++;
				tok = strtok(NULL, ",");
			}
			i++;
		}
			
		if(i == length)
		{
			shortest = findShortestpath(matrix, length);
			printf("%d\n", shortest);

			for (i = 0; i < length; i++)
			{
				free(matrix[i]);
			}
			free(matrix);

			new_matrix = 1;	
		}
	}
	return 0;
}
