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
            Free(n);
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

    Tree_Print(t);
    printf("\n\n\n");
    Node n = t->first;
    while(n != NULL)
    {
        printf(" %4d ", n->key);
        n = n->next;
    }
    Tree_Inorder(t);

    printf("\n\n\n");
    n = t->last;
    while(n != NULL)
    {
        printf(" %4d ", n->key);
        n = n->prev;
    }
    Tree_Postorder(t);
    Tree_Delete(t);
    return 0;
}
