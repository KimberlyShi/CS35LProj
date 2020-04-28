#include <stdlib.h>
#include <stdio.h>

//takes in 2 arguments
//negative if a<b
//0 if a ==b
//positive a>b

int frobcmp(char const *a, char const *b)
{
	while ((*a) != ' ' && (*b) != ' ')
	{
		unsigned char newA = (*a)^42;
		unsigned char newB = (*b)^42;
		if (newA < newB || *a == ' ')
			return -1;
		else if (newA > newB || *b == ' ')
			return 1;
		a++;
		b++;
	}
	if ((*a) != ' ')
		return 1;
	if ((*b) != ' ')
		return -1;
	return 0;
}

int voidFrobcmp(const void *a, const void *b)
{
	return frobcmp(*(char**)a, *(char**)b);
}

int getValidChar(void)
{
	if(ferror(stdin) != 0)
	{
		fprintf(stderr, "Stdin error! \n");
		exit(1);
	}
	return getchar();
}

void mallocError(void)
{
	fprintf(stderr, "Malloc Error \n");
	exit(1);
}

void reallocError(void)
{
	fprintf(stderr, "Realloc Error \n");
	exit(1);
}

void checkStdout(void)
{
	if(ferror(stdin) != 0)
	{
		fprintf(stderr, "Stdout error! \n");
		exit(1);
	}	
}

void frobinacted(void)
{
	//allocate memory wehn first reading from stdin
	char *current = (char*)malloc(sizeof(char));
	if (current == NULL)
	{
		free(current);
		mallocError();
	}
	char **total = (char**)malloc(sizeof(char*));
	if (total == NULL)
	{
		free(total);
		mallocError();
	}

	char letter = getValidChar();
	char nextLetter = getValidChar();

	int index = 0;
	int count = 0;
	while(letter != EOF)
	{
		current[index] = letter;
		char *temp = (char*)realloc(current, (index + 2) * sizeof(char));
		if(temp != NULL)
			current = temp;
		if (temp == NULL)
		{
			free(current);
			reallocError();
		}
		index++; 
		
		//indicates the end of that line
		//will need to begin to read from next text line
		if(letter == ' ')
		{
			total[count] = current;
			//reallocates memory for current and next letter
			char *try = (char*)malloc(40*sizeof(char));
			if (try == NULL)
			{
				free(try);
				free(current);
				mallocError();
			}
			free(try);

			char **temp2 = (char**)realloc(total, (count +2) * sizeof(char*));
			count++;
			if (temp2 != NULL)
			{
				total=temp2;
				current = (char*)malloc(sizeof(char));
				if(current == NULL)
				{
					free(current);
					mallocError;
				}
			}
			else //will be null error
			{
				free(total);
				reallocError();
			}
			//reset to the beginning the word
			index = 0;
			//signal the end of the line	
			if (nextLetter == ' ')
			{
				while (letter == ' ')
					letter = getValidChar();
				nextLetter = getValidChar();
				continue;
			}
			if (nextLetter == EOF)
				break;
		}
		//when letter != ' ' need to add space at end
		if (nextLetter == EOF)
		{
			letter = ' ';
			continue;
		}
		letter = nextLetter;
		nextLetter = getValidChar();

	}
	qsort(total, count, sizeof(char*), voidFrobcmp);
	
	//print to stdout
	for(int j = 0; j < count; j++)
	{
		checkStdout();
		printf("%s", total[j]);
	}

	//free all the memory
	for (int i = 0; i < count; i++)
		free(total[i]);
	free(total);
	exit(0);
}

int main()
{
	//printf("%d", frobcmp("*{_CIA\030\031 ", "*`_GZY\v "));
	frobinacted();
	exit(0);
	return 0;
}
