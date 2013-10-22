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
    for(i = count; i >= 1; i--)
    {
        Node n = Node_Create(random() % 100000), tmp;
        tmp = Tree_AddNode(t, n);
        if(tmp != n)
        {
            Node_Delete(n);
        }
    }

    Tree_Delete(t);
    return 0;
}
