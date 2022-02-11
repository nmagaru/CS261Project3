#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct mazeStruct
{
 //char arr[32][32];  /* allows for a maze of size 30x30 plus outer walls */
 char**  arr; /* allows for a maze of a desired size through memory allocation */
 int xsize, ysize;
 int xstart, ystart;
 int xend, yend;
} maze;

int main (int argc, char **argv)
{
	// debug mode code
	bool debugmode;
	int stringcompare;

	// loop through possible ARGuments
	for (int h = 0; h < argc; h++)
	{
		stringcompare = strcmp(argv[h], "-d");

		// update debug bool if debug flag found
		if (stringcompare == 0)
		{
			debugmode = true;
			break;
		}

		// debug bool is false otherwise, do not show debug statements
		debugmode = false;
	}


	// initialize stack struct for linked list of coordinate values
	typedef struct LinkedList
	{
		int xcoord;
		int ycoord;
		struct LinkedList* Next;

	} Node;
	Node* Head = NULL;
	Head = (Node*)malloc(sizeof(Node));

	// function for initializing stack linked list
	void init(Node** head, int x, int y)
	{
		(*head)->xcoord = x;
		(*head)->ycoord = y;
		(*head)->Next = NULL;
	}
	
	// function to check if stack is empty, where head of linked list is NULL
	bool is_empty (Node* head)
	{
		if (head == NULL)
			return true;
		
		return false;
	}
	
	// function for adding a node to end of linked list
	void push (Node** head, int x, int y)
	{
		Node* newnode;
		newnode = (Node*)malloc(sizeof(Node));

		newnode->xcoord = x;
		newnode->ycoord = y;
		newnode->Next = *head;

		*head = newnode;
		
		// debug: print out coord of pushed node
		if (debugmode)
			printf("Pushed: (%d, %d)\n", x, y);
	}

	// function for popping/removing a node from top of stack/end of list
	void pop (Node** head)
	{	
		if (*head == NULL)
			return;

		// debug: print out coord of current node to be popped
		if (debugmode)
			printf("Popped: (%d, %d)\n", (*head)->xcoord, (*head)->ycoord);
		
		Node* popnode = *head;
		*head = (*head)->Next;
		
		free(popnode);
	}

	// function to access the top element of the stack, which is a node
	Node* top (Node* head)
	{
		if (is_empty(head))
			return NULL;

		return head;
	}

	// function to clear the stack by freeing all the nodes
	void clear (Node** head)
	{
		while ((*head) != NULL)
			pop(head);
	}
		
	// function to print out list nodes and their values
	void printList (Node* head)
	{
		if (head == NULL)
			return;

		printList(head->Next);

		printf("(%d, %d)\n", head->xcoord + 1, head->ycoord + 1);
	}


  maze m1;

  int xpos, ypos;
  int i,j;

  FILE *src;

  bool** visitarr; // array to track if position is visited or unvisited

  /* REMOVED: verify the proper number of command line arguments were given */
  /* if(argc < 2) {
     printf("Usage: %s <input file name>\n", argv[0]);
     exit(-1);
  }*/
   
  // Loop through arguments to see if any file is valid
 	for (int k = 1; k < argc; k++)
 	{
		if( (src = fopen(argv[k], "r")) != NULL)
			break; // break at first instance of valid file
	}

  	if (src == NULL )
  	{
    		printf("Error: No valid file detected.\n");
    		exit(-1);
	}

  /* read in the size, starting and ending positions in the maze */
  fscanf (src, "%d %d", &m1.xsize, &m1.ysize);
  	// check both maze sizes if valid (greater than 0)
	while (m1.xsize <= 0 || m1.ysize <= 0)
	{
		printf("Invalid: Maze sizes must be greater than 0.\n");
		fscanf (src, "%d %d", &m1.xsize, &m1.ysize);
	}

  fscanf (src, "%d %d", &m1.xstart, &m1.ystart);
	// check if starting coord is valid (within maze size)
	while (m1.xstart <= 0 || m1.xstart > m1.xsize || m1.ystart <= 0 || m1.ystart > m1.ysize)
	{
		printf("Invalid: Starting coordinate(s) are not within the maze's range.\n");
		fscanf (src, "%d %d", &m1.xstart, &m1.ystart);
	}
  	
  fscanf (src, "%d %d", &m1.xend, &m1.yend);
  	// check if ending coord is valid (within maze size)
	while (m1.xend <= 0 || m1.xend > m1.xsize || m1.yend <= 0 || m1.yend > m1.ysize)
	{
		printf("Invalid: Ending coordinate(s) are not within the maze's range.\n");
		fscanf (src, "%d %d", &m1.xend, &m1.yend);
	}
	// check if end coords overlap with starting position
	while (m1.xstart == m1.xend && m1.ystart == m1.yend)
	{
		printf("Invalid: Ending coordinates match starting coordinates.");
		fscanf (src, "%d %d", &m1.xend, &m1.yend);
	}

  /* print them out to verify the input */
  printf ("size: %d, %d\n", m1.xsize, m1.ysize);
  printf ("start: %d, %d\n", m1.xstart, m1.ystart);
  printf ("end: %d, %d\n", m1.xend, m1.yend);

  // dynamically allocate memory for the maze and visit arrays
 	m1.arr = (char **)malloc((m1.xsize + 2) * sizeof(char *));
	for (i = 0; i < m1.xsize + 2; i++)
		m1.arr[i] = (char *)malloc((m1.ysize + 2) * sizeof(char));

	visitarr = (bool **)malloc((m1.xsize + 2) * sizeof(bool *));
	for (i = 0; i < m1.xsize + 2; i++)
		visitarr[i] = (bool *)malloc((m1.ysize + 2) * sizeof(bool));

  /* initialize the maze to empty */
  for (i = 0; i < m1.xsize+2; i++)
     for (j = 0; j < m1.ysize+2; j++)
       m1.arr[i][j] = '.';

  /* mark the borders of the maze with *'s */
  for (i=0; i < m1.xsize+2; i++)
    {
     m1.arr[i][0] = '*';
     m1.arr[i][m1.ysize+1] = '*';
    }
  for (i=0; i < m1.ysize+2; i++)
    {
     m1.arr[0][i] = '*';
     m1.arr[m1.xsize+1][i] = '*';
    }

  /* mark the starting and ending positions in the maze */
  m1.arr[m1.xstart][m1.ystart] = 's';
  m1.arr[m1.xend][m1.yend] = 'e';
		  
  /* mark the blocked positions in the maze with *'s */
  while (fscanf (src, "%d %d", &xpos, &ypos) != EOF)
    {
	// check if blocked coord is valid (within maze sizes)
	if (xpos <= 0 || xpos > m1.xsize || ypos <= 0 || ypos > m1.ysize)
	{
		printf("Invalid: Blocked coordinates are outside the maze's range.\n");
		continue;
	}

	// check if the blocked coord overlaps with start or end positions
	if ( (xpos == m1.xstart && ypos == m1.ystart) || (xpos == m1.xend && ypos == m1.yend) )
	{
		printf("Invalid: Blocked coordinates block start/end position.\n");
		continue;
	}

     m1.arr[xpos][ypos] = '*';
    }

  /* print out the initial maze */
  for (i = 0; i < m1.xsize+2; i++)
    {
     for (j = 0; j < m1.ysize+2; j++)
       printf ("%c", m1.arr[i][j]);
     printf("\n");
    }

  	// assign bool values in visitarr if position is unvisited or visited (or blocked)
  	for (i = 0; i < m1.xsize + 2; i++)
  	{
	  	for (j = 0; j < m1.ysize + 2; j++)
	  	{
		  	if (m1.arr[i][j] == '.')
		  		visitarr[i][j] = false;
		  	else if ((m1.arr[i][j] == '*') || (m1.arr[i][j] == 's'))
				visitarr[i][j] = true;
		}
	}

	// initialize  start coordinates in stack linked list
	init(&Head, m1.xstart, m1.ystart);
	
	int xcur, ycur;
	// loop through maze until a solution/no solution is found
	while (!is_empty(Head))
	{
		Node* topnode = top(Head);
		xcur = topnode->xcoord;
		ycur = topnode->ycoord;
		
		// check if coordinate of stack top is at end position
		if ((xcur == m1.xend - 1) && (ycur == m1.yend - 1))
		{
			break;
		}

		// check if coordinate of stack top has a right neighbor
		if (!visitarr[xcur][ycur + 1])
		{
			push(&Head, xcur, ycur + 1);
			visitarr[xcur][ycur + 1] = true;
			continue;
		}
		

		// check if coordinate of stack top has a down neighbor
		if(!visitarr[xcur + 1][ycur])
		{
			push(&Head, xcur + 1, ycur);
			visitarr[xcur + 1][ycur] = true;
			continue;
		}
	

		// check if coordinate of stack top has a left neighbor
		if(!visitarr[xcur - 1][ycur])
		{
			push(&Head, xcur - 1, ycur);
			visitarr[xcur - 1][ycur] = true;
			continue;
		}

		// check if coordinates of stack top has an above neighbor
		if(!visitarr[xcur][ycur - 1])
		{
			push(&Head, xcur, ycur - 1);
			visitarr[xcur][ycur - 1] = true;
			continue;
		}
	
		// else, pop coordinate from end of list/stack
		pop(&Head);
	}

	// display end message(s) depending on empty list(no sol) or filled list(sol)
	if (is_empty(Head))
		printf("This maze has no solution.\n");
	else
		printList(Head);

	// code to prevent memory leaks
	// deallocate both arrays
	for (i = 0; i < m1.xsize + 2; i++)
		free(m1.arr[i]);
	free(m1.arr);

	for (i = 0; i < m1.xsize + 2; i++)
		free(visitarr[i]);
	free(visitarr);

	// clear linked list
	clear(&Head);

	// close text file
	fclose(src);

	return 0;

}
