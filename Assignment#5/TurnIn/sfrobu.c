#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>

int fOption = 0;
int frobcmp(char const *a, char const*b)
{
    unsigned char newA;
    unsigned char newB;
    while((*a) != ' ' && (*b) != ' ')
    {
        if(fOption == 0)
        {
            newA = (*a) ^42;
            newB  = (*b)^42;
        }
        else //fOption == 1
        {
            newA = toupper((*a)^42);
            newB = toupper((*b)^42);
        }
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

int voidFrobcmp(void const*a, void const *b)
{
    return frobcmp(*(char**)a, *(char**)b);
}

//ERROR MESSAGES:
void mallocError(void)
{
    write(2, "ERROR: Malloc Error.\n", 21);
    exit(1);
}

void reallocError(void)
{
    write(2, "ERROR: Realloc Error.\n",21);
    exit(1);
}

void stdinError(void)
{
    write(2, "ERROR: Stdin Error.\n", 20);
    exit(1);
}

//free all remaining memory
//also called when program ends prematurely with error
void freeMem(char *current, char **total, int count)
{
    if (current != NULL)
        free(current);
    if (total!= NULL)
    {
        int i = 0;
        while( i < count)
        {
            free(total[i]);
            i++;
        }
        free(total);
    }
}

//check for the -f option
int numArgs(int argc, char *argv[])
{
    if(argc == 2)
    {
        if(strcmp(argv[1], "-f") != 0)
        {
            write(2, "ERROR: Incorrect number of arguments.\n",38);
            exit(1);
        }
        else
            return 1;
    }
    else if (argc > 2)
    {
        write(2, "ERROR: Incorrect number of arguments.\n", 38);
        exit(1);
    }
    return 0;
}


int main(int argc, char *argv[])
{
    int currentSize = 40;
    char *current = (char*)malloc(currentSize*sizeof(char));
    if(current == NULL)
    {
        free(current);
        mallocError();
    }
    int totalSize = 40;
    char **total = (char**)malloc(totalSize*sizeof(char*));
    
    if(total == NULL)
    {
        free(current);
        free(total);
        mallocError();
    }
    
    //read the current character
    char  b[1];
    lseek(0,0,SEEK_CUR);
    if(read(0,b,sizeof(char)) < 0)
    {
        free(current);
        free(total);
        stdinError();
    }
    //read the next character
    char a[1];
    lseek(0,0,SEEK_CUR);
    if(read(0,a,sizeof(char)) < 0)
    {
        free(current);
        free(total);
        stdinError();
    }
    
    int index = 0;
    int count = 0;
    bool currSizeBigger = false;
    bool totSizeBigger = false;
    
    while(true)
    {
        current[index] = b[0];
        index++;
        if(index >= currentSize)
            currSizeBigger = true;
        if(count >= totalSize)
            totSizeBigger = true;
        
        if(currSizeBigger)
        {
            //everytime add size, will need to add more room for memory
            currentSize*=2;
            char *temp = (char*)realloc(current, currentSize *sizeof(char));
            if(temp != NULL)
            {
               current = temp;
            }
            else
            {
                freeMem(current, total, count);
                reallocError();
            }
        }
        //reset size
        else if (b[0] == ' ')
        {
            total[count] = current;
            currentSize = 40;
            count++;
            char *temp = (char*)malloc(currentSize*sizeof(char));
            char **temp2;
            if(temp != NULL)
            {
                if(totSizeBigger)
                {
                    totalSize *= 2;
                    temp2 = (char**)realloc(total,totalSize *sizeof(char*));
                    if(temp2 != NULL)
                    {
                        total = temp2;
                    }
                    else
                    {
                        freeMem(current, total,count);
                        reallocError();
                    }
                    
                }
                current = temp;
                index = 0;
            }
            //temp == NULL
            else
            {
                freeMem(current,total,count);
                mallocError();
            }
        }
        
        b[0] = a[0];
        //when read() == 0, it reached the end of the file
        lseek(0,0,SEEK_CUR);
        if(read(0,a,sizeof(char)) == 0)
            break;
        else
        {
            currSizeBigger = false;
            totSizeBigger = false;
        }
    }
    
    if(b[0] != ' ')
    {
        current[index] = b[0];
        index++;
    }
    current[index] = ' ';
    total[count] = current;
    index++;
    count++;
    
    //check for the -f foption calling numArgs
    fOption = numArgs(argc, argv);
    qsort(total,count,sizeof(char*), voidFrobcmp);
    
    //traverse through total to print out
    int i = 0;
    int j = 0;
    while(i < count)
    {
        j = 0;
        while(total[i][j] != ' ')
        {
            if(write(1, (void*)&(total[i][j]), sizeof(char)) < 0)
            {
                freeMem(current, total, count);
                write(2, "ERROR: Stdout Error. \n", 21);
                exit(1);
            }
            j++;
        }
        if(total[i][j] == ' ')
        {
            if (write(1, (void*)&(total[i][j]), sizeof(char)) < 0)
            {
                freeMem(current,total, count);
                write(2, "ERROR: Stdout Error.\n", 21);
                exit(1);
            }
        }
        i++;
    }
    int h = 0;
    for(h = 0; h < count; h++)
        free(total[h]);
    free(total);
    
    exit(0);
}

