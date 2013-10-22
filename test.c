#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

int main(int argc, char *argv[])
{
    int i = 0, count;
    Tree t;
    if(argc == 1)
    {
        log_msg("usage ./a.out <number>");
        return -1;
    }
    count = atoi(argv[1]);
    t = Tree_Create();
    for(i = 1; i <= count; i++)
    {
        Tree_AddNode(t, i);
    }
    return 0;
}
