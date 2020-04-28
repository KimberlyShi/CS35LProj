#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, const char *argv[])
{
	const char *from;
	const char *to;
	
	//check the number of arguments are valid
	if (argc != 3)
	{
		fprintf(stderr, "ERROR: invalid number of arguments. \n");
		exit(1);
	}
	else
	{
		from = argv[1];
		to = argv[2];
	}	
	int fromLength = strlen(from);
	if(fromLength == 0)
	{
		fprintf(stderr, "ERROR: invalid number of arguments. \n");
		exit(1);
	}
	int toLength = strlen(to);
	//lengths must be equal
	if(fromLength != toLength)
	{
		fprintf(stderr, "ERROR: different lengths. \n");
		exit(1);
	}

	//check for duplicates
	char const *tempA;
	char const *tempB;
	for(tempA = from; *tempA != '\0'; tempA++)
	{
		for(tempB = tempA + 1; *tempB != '\0'; tempB++)
		{
			if(*tempA == *tempB)
			{
				fprintf(stderr, "ERROR: found duplicates. \n");
				exit(1);
			}
		}
	}


	char current;
	int traverser = 0;
	int i;
	while(((current = getchar()) != EOF) && !feof(stdin))
	{
		if(ferror(stdin))
		{
			fprintf(stderr, "ERROR: reading from stdin. \n");
			exit(1);
		}
		traverser = -1;
		for(i = 0; from[i] != '\0'; i++)
		{	
			if(current == from[i])
				traverser = i;
		}		
		if (traverser != -1)
		{
			putchar(to[traverser]);
			if (ferror(stdout))
			{
				fprintf(stderr, "ERROR: can't output \n");
				exit(1);
			}
		}
		else
		{
			putchar(current);
			if (ferror(stdout))
			{
				fprintf(stderr, "ERROR: can't output. \n");
				exit(1);
			}
		}
	}	
	return 0;
}
