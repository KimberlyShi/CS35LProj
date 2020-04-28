#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <ctype.h>

int frobcmp(char const *a, char const*b)
{
	int sum = 0;
	for (; (*a) != ' ' && (*b) != ' '; a++, b++)
	{
		unsigned char ac = (*a) ^42;
		unsigned char bc = (*b)^42;

		if(ac > bc)
			return 1;
		else if (ac < bc)
			return -1;
	}
	if ((*a) != ' ')
		return 1;
	if ((*b) != ' ')
		return -1;
	
	return sum;
}

int frobcmpUpper(char const *a, char const *b)
{
	int sum = 0;
	for (; (*a) != ' '&& (*b) != ' '; a++, b++)
	{
		unsigned char ac = toupper((*a)^42);
		unsigned char bc = toupper((*b)^42);
		
		if(ac > bc)
			return 1;
		else if (ac < bc)
			return -1;
	}
	
	if((*a) != ' ')
		return 1;
	if ((*b) != ' ')
		return -1;

	return sum;
}

int compar(void const*a, void const *b)
{
	return frobcmp(*(char**)a, *(char**)b);
}

int compar2(void const *a, void const *b)
{
	return frobcmpUpper(*(char**)a, *(char**)b);
}

void error (char *message)
{
	fprintf(stderr, "%s\n", message);
	exit(1);
}

void freeMem(char **w, int wentries, char *in)
{
	if (w!= NULL)
	{
		int i;
		for (i = 0; i < wentries; i++)
			free(w[i]);
		free(w);
	}
	if (in != NULL)
		free(in);
}
int main(int argc, char *argv[])
{
	int f = 0;
	if (argc > 2)
		error("too many arguments");
	else if (argc == 2)
	{
		if(strcmp(argv[1], "-f") == 0)
			f = 1;
		else
			error("invalid argument.");
	}

	int sizec = 40, sizew = 40, index = 0, count = 0;
	char *current = (char*)malloc(sizec*sizeof(char));
	char **words = (char**)malloc(sizew*sizeof(char*));

	if(current == NULL || words == NULL)
	{
		freeMem(words, 0, current);
		error("malloc failed");
	}
	
	char c[1], p[1];
	if(read(0, p, sizeof(char)) < 0 || read(0,c,sizeof(char)) <0)
	{
		freeMem(words, 0, current);
		error("stdin error");
	}

	while(c[0] > -1)
	{
		current[index++] = p[0];

		if (index >= sizec)
		{
			char *temp = (char*)realloc(current, sizec*2*sizeof(char));
			sizec*=2;
			if(temp == NULL)
			{
				freeMem(words, count, current);
				error("realloc error");
			}
			current = temp;
		}

		if (p[0] == ' ')
		{
			words[count++] = current;
			sizec = 40;
			char *temp = (char*)malloc(sizec*sizeof(char));

			if(temp == NULL)
			{
				freeMem(words,count, current);
				error("malloc error");
			}

			if(count >= sizew)
			{
				char **temp = (char**)realloc(words,sizew*2*sizeof(char*));
				sizew *= 2;

				if(temp == NULL)
				{
					freeMem(words,count,current);
					error("realloc error");
				}
				words = temp;
			}
			current = temp;
			index = 0;
		}
		
		p[0] = c[0];

		if(read(0,c,sizeof(char)) == 0)
			break;
	}
	
	if(p[0] != ' ')
		current[index++] = p[0];
	current[index++] = ' ';
	words[count++] = current;

	if(f == 0)
		qsort(words,count,sizeof(char*), compar);
	else
		qsort(words,count,sizeof(char*), compar2);

	int i,j;
	for(i = 0; i < count; i++)
	{
		if(ferror(stdout))
		{
			freeMem(words, count, current);
			error("error in stdout");
		}
		for (j = 0; words[i][j] != ' '; j++)
		{
			if(write(1, (void*)&(words[i][j]), sizeof(char)) < 0)
			{
				freeMem(words, count, current);
				error("error in stdout");
			}
		}
		if (write(1, (void*)&(words[i][j]), sizeof(char)) < 0)
		{
			freeMem(words, count, current);
			error("error in stdout");
		}
	}
	freeMem(words, count, NULL);
}
