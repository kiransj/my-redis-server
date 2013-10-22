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
        Node n = Node_Create(i), tmp;
        tmp = Tree_AddNode(t, n);
        if(!IS_NULL(n->parent))
        {            
            log_msg("Parent of '%3d' is '%3d'", n->key, n->parent->key);
        }
        else
        {
            log_msg("Parent of '%3d' is NULL", n->key);
        }
    }

    if(argc == 3)
    {
        for(i = count+1; i >= 0; i--)
        {
            if(IS_NULL(Tree_Find(t, i)))
            {
                log_msg("Key %5d not found", i);
            }
        }
    }
    return 0;
}
