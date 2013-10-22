#ifndef _TREE_H_
#define _TREE_H_
#include "util.h"
typedef struct _Node *Node;
struct _Node
{
    /*Node internal Data*/
    int height;
    Node parent, left, right;

    /*User data*/
    int data;
};

Node Node_Create(int data);
void Node_Delete(Node n);

typedef struct _Tree *Tree;
struct _Tree
{
    int num_elements;
    Node root;
};


Tree Tree_Create(void);
int Tree_AddNode(Tree t, int data);
void Tree_Print(Tree t);
void Tree_Inorder(Tree t);
void Tree_Preorder(Tree t);
#endif
