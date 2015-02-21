/*
11/01/2014 Melissa Stockman

Implementation of a Sudoku puzzle solver 

Input: a CSV file consisting of an unsolved Sudoku with 0's representing blanks
Output: a CSV file with the solved Sudoku. 
*/

#include <stdio.h>
#include <stdlib.h>

#define PUZZLESIZE 9

struct TILE
{
     int value;		                   // value of the tile
	 int isBlank;                     // true if this tile is a blank
	 int validValues[PUZZLESIZE];      // valid values for this tile based on initial puzzle
	 int validValueIndex;              // index into validValues array
};
struct TILE puzzle[PUZZLESIZE][PUZZLESIZE];

// Read puzzle from CSV file
char readInPuzzle(char *inputFile)
{
     FILE *fp;
	 int i,j,k;
		 
	 if (!inputFile)
		 return 1;
	 fp = fopen(inputFile,"r");
	 if (!fp)
	 {
	    printf("Couldn't open input file %s\n", inputFile);
		return 1;
	 }
	 else
	 {
		for (i=0;i<PUZZLESIZE;i++)
		{
			for (j=0;j<PUZZLESIZE;j++)
			{
				if (j==PUZZLESIZE-1)
				{
					fscanf(fp,"%d",&puzzle[i][j].value );
				}
				else
				{
					fscanf(fp,"%d,",&puzzle[i][j].value );
				}
				
				if (puzzle[i][j].value == 0)
				{
					puzzle[i][j].isBlank = 1;
					for (k=0;k<PUZZLESIZE;k++)
					     puzzle[i][j].validValues[k] = 0;
				}
				else
				{
					puzzle[i][j].isBlank = 0;
				}
				
			}
		}
		return 0;
	 }
}
// Check if num is a valid value for this row
int validRow(int x, int num)
{
	int i;

	for (i=0;i<PUZZLESIZE;i++)
	{
		if (puzzle[x][i].value  == num)
			return 0;
	}
	return 1;
}
// Check if num is a valid value for this column
int validCol(int y, int num)
{
	int i;

	for (i=0;i<PUZZLESIZE;i++)
	{
		if (puzzle[i][y].value == num)
			return 0;
	}
	return 1;
}
// Check if num is a valid value for the 3x3 box
int validBox(int row, int col, int num)
{
	int i,j,fromx,fromy;

	fromx = (row/3)*3;
	fromy = (col/3)*3;

	for (i=fromx;i<fromx+3;i++)
	{
		for (j=fromy;j<fromy+3;j++)
		{
			if (puzzle[i][j].value == num)
				return 0;
		}
	}
	return 1;
}

void initValidValues()
{
	int i,j,k;

	for (i=0;i<PUZZLESIZE;i++)
	{
		for (j=0;j<PUZZLESIZE;j++)
		{
			if (!puzzle[i][j].isBlank)
				continue;
			puzzle[i][j].validValueIndex = 0;
			for (k=1;k<=PUZZLESIZE;k++)
			{
		        if (validRow(i,k) && validCol(j,k) && validBox(i,j,k))
				{
					puzzle[i][j].validValues[puzzle[i][j].validValueIndex] = k;
					puzzle[i][j].validValueIndex++;
				}
			}
		}
	}
}

// Print out the puzzle
void printPuzzle()
{
	int i,j;

	for (i=0;i<PUZZLESIZE;i++)
	{
		for (j=0;j<PUZZLESIZE;j++)
		{
			printf("%d",puzzle[i][j].value);
			if (j<PUZZLESIZE-1)
				printf(",");
		}
		printf("\n");
	}
}

void solveNext(int row, int col);

// solve the puzzle
void solvePuzzle(int row, int col)
{
	int i;

	// if reached the last row then print out puzzle solution and exit
	if (row>PUZZLESIZE-1)
	{
		printPuzzle();
		exit(0);
	}
	// if the tile is not a blank then move to the next tile
	if (!puzzle[row][col].isBlank)
		solveNext(row,col);
	else
	{
		// look through the valid values for this blank tile
		for (i=0;i<puzzle[row][col].validValueIndex;i++)
		{
        	if ( validRow(row,puzzle[row][col].validValues[i]) && validCol(col,puzzle[row][col].validValues[i]) &&
				validBox(row,col,puzzle[row][col].validValues[i]) )
			{
				puzzle[row][col].value = puzzle[row][col].validValues[i];
			    solveNext(row,col);
			}
		}
		// didn't find a valid value 
		puzzle[row][col].value = 0; 
	}
}

// move to the next tile in the puzzle
void solveNext(int row, int col)
{
	if(col < PUZZLESIZE-1)
       solvePuzzle(row,col+1);
    else
       solvePuzzle(row+1,0);
}



int main(int argc, char **argv)
{

	if (readInPuzzle( argv[1] ))
		return 1;

	// initialize the legal values for the blank tiles
	initValidValues();

	// solve the puzzle
	solvePuzzle(0,0);

	return 0;

}

