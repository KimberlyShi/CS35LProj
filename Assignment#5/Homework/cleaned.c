#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

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


void error (char *message)
{
    fprintf(stderr, "%s\n", message);
    exit(1);
}

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
    
    char  b[1];
    if(read(0,b,sizeof(char)) < 0)
    {
        free(current);
        free(total);
        stdinError();
    }
    char a[1];
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
            currentSize*=2;
            char *temp = (char*)realloc(current, currentSize *sizeof(char));
            if(temp == NULL)
            {
                freeMem(current, total, count);
                reallocError();
            }
            else
                current = temp;
        }
        //reset size
        else if (b[0] == ' ')
        {
            total[count] = current;
            currentSize = 40;
            count++;
            char *temp = (char*)malloc(currentSize*sizeof(char));
            
            if(temp == NULL)
            {
                freeMem(current,total,count);
                mallocError();
            }
            char **temp2;
            if(totSizeBigger)
            {
                totalSize *= 2;
                temp2 = (char**)realloc(total,totalSize *sizeof(char*));
                if(temp2 == NULL)
                {
                    freeMem(current, total,count);
                    reallocError();
                }
                total = temp2;
            }
            current = temp;
            index = 0;
        }
        
        b[0] = a[0];
        currSizeBigger = false;
        totSizeBigger = false;
        
        if(read(0,a,sizeof(char)) == 0)
            break;
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
    
    fOption = numArgs(argc, argv);
    qsort(total,count,sizeof(char*), voidFrobcmp);
    int i = 0;
    int j = 0;
    while(i < count)
    {
        if(ferror(stdout))
        {
            freeMem(current,total, count);
            write(2, "ERROR: Stdout Error. \n", 21);
            exit(1);
        }
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
        if (write(1, (void*)&(total[i][j]), sizeof(char)) < 0)
        {
            freeMem(current,total, count);
            write(2, "ERROR: Stdout Error.\n", 21);
            exit(1);
        }
        i++;
    }
    int h = 0;
    for(h = 0; h < count; h++)
        free(total[h]);
    free(total);
    
    exit(0);
}

