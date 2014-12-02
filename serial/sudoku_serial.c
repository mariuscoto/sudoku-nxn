#include<stdio.h>
#include<stdlib.h>

void print_sudoku(int** sudoku, int n)
{
    int i,j;
    int p = n*n;
    for(i=0;i<p;i++)
    {
        if(i%n==0)
        {
            printf("\n");
        }
        for(j=0;j<p;j++)
        {
            if(j%n==0)
            {
                printf(" ");
            }
            printf("%d ",sudoku[i][j]);
        }
        printf("\n");
    }
}

int** read_sudoku(char* filename, int* n)
{
    int i,j,p;
    int** sudoku;
    FILE *f = fopen(filename,"rt");
    fscanf(f,"%d",n);
    p = (*n)*(*n);
    sudoku = calloc(p,sizeof(int*));
    for(i=0;i<p;i++)
    {
        sudoku[i] = calloc(p,sizeof(int));
        for(j=0;j<p;j++)
        {
            fscanf(f,"%d",&sudoku[i][j]);
        }
    }
    fclose(f);
    return sudoku;
}

int verify_position(int** sudoku, int n, int pi, int pj)
{
    int i,j;
    int p = n*n;
    int w = sudoku[pi][pj];
    int sq_i,sq_j,sq_i_n,sq_j_n;
    for(i=0;i<p;i++)
    {
        if(pj != i && sudoku[pi][i] == w)
        {
            return 0;
        }
        if(pi != i && sudoku[i][pj] == w)
        {
            return 0;
        }
    }

    sq_i = pi/n;
    sq_i *= n;
    sq_j = pj/n;
    sq_j *= n;
    sq_i_n = sq_i+n;
    sq_j_n = sq_j+n;
    for(i=sq_i;i<sq_i_n;i++)
    {
        for(j=sq_j;j<sq_j_n;j++)
        {
            if(i != pi && j != pj && sudoku[i][j] == w)
            {
                return 0;
            }
        }
    }
    
    return 1;
}

int try_another(int** sudoku, int n, int i, int j)
{
    int p = n*n;
    while(1)
    {
        sudoku[i][j]++;
        if(sudoku[i][j] > p)
        {
            sudoku[i][j] = 0;
            return 0;
        }
        if(verify_position(sudoku,n,i,j) == 1)
        {
            return 1;
        }
    }
}

void next(int *i, int *j, int p)
{
    (*j)++;
    if(*j == p)
    {
        *j = 0;
        (*i)++;
    }
}

void prev(int *i, int *j, int p)
{
    (*j)--;
    if(*j < 0)
    {
        *j = p-1;
        (*i)--;
    }
}

int solve(int** sudoku,int** sudoku_o, int n)
{
    int i,j;
    int p = n*n;
    
    i=0;
    j=0;

    int noop = 0;
    while(i>=0 && i<p && sudoku_o[i][j]!=0)
    {
        next(&i,&j,p);
    }
    while(i>=0 && i<p)
    {
        noop ++;
        if(noop%100000==0)
            print_sudoku(sudoku,n);
        if(try_another(sudoku,n,i,j) == 1)
        {
            //printf("%d %d\n",i,j);
            next(&i,&j,p);
            while(i>=0 && i<p && sudoku_o[i][j]!=0)
            {
                next(&i,&j,p);
            }
        }
        else
        {
            prev(&i,&j,p);
            while(i>=0 && i<p && sudoku_o[i][j]!=0)
            {
                prev(&i,&j,p);
            }
        }
    }
    if(i==p)
    {
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[])
{ 
    int** sudoku;
    int** sudoku_o;
    int n;
    if(argc!=2)
    {
        printf("Usage: ./sudoku {filename}\n");
        return 1;
    }
    sudoku = read_sudoku(argv[1],&n);
    sudoku_o = read_sudoku(argv[1],&n);

    if(solve(sudoku,sudoku_o,n) == 0)
    {
        printf("Puzzle cannot be solved!\n");
        return 1;
    }
    print_sudoku(sudoku,n);
    return 0;
}
