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
	

#if 0 
    if(argc == 3)
    for(i = count+1; i >= 0; i--)
    {
        Node *tmp = t.FindNode(i);
        if(IS_NULL(tmp))
        {
            log_msg("key '%d' not found", i);
        }
    }    
    t.PrintTree();
    t.InOrder();
    t.PrintList();
	log_msg("Height of the tree : %d", t.Height());
#endif    
    return 0;
}
