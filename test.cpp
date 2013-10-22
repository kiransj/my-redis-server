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

    for(i = count; i >= 1; i--)
    {
        Node *n = new Node(i), *tmp;
        tmp = t.AddNode(n);
        if(tmp != n)
        {
            delete n;
        }
    }
    t.PrintTree();
    t.InOrder();
    t.PrintList();
    return 0;
}
