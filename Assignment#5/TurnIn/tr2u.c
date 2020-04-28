#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
	const char *from;
	const char *to;
	
	//check the number of arguments are valid
	if (argc != 3)
	{
		write(STDERR_FILENO, "ERROR: invalid number of arguments.\n", 36);
		exit(1);
	}
	else
	{
		from = argv[1];
		to = argv[2];
	}	
	char map[256] = {'\0'};
	int fromLength = strlen(from);
	if(fromLength == 0)
	{
		write(STDERR_FILENO, "ERROR: invalid number of arguments.\n",36);
		exit(1);
	}
	int toLength = strlen(to);
	//lengths must be equal
	if(fromLength != toLength)
	{
		write(STDERR_FILENO, "ERROR: different lengths.\n", 26);
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
				write(STDERR_FILENO, "ERROR: found duplicates.\n", 25);
				exit(1);
			}
		}
	}

	//Buffers needed for read and write
	char output[1];
	int traverser = 0;
	int i;
	while(read(0, output, 1) > 0)
	{
		traverser = -1;
		for(i = 0; from[i] != '\0'; i++)
		{	
			if(output[0] == from[i])		
				traverser = i;

		}		
		if (traverser != -1)
		{
			if(write(1, &to[traverser], 1) < 0)
			{
				write(2, "ERROR: can't output.\n", 21);
				exit(1);
			}
		}
		else
		{
			if(write(1, output, 1) < 0)
			{
				write(2, "ERROR: can't output.\n",21);
				exit(1);
			} 
		}
	}	
	return 0;
}
