#ifndef _TREE_H_
#define _TREE_H_
#include "util.h"
typedef struct _Node *Node;
struct _Node
{
    /*Node internal Data for tree*/
    int height;
    Node parent, left, right;

    /*DDL data*/
    Node prev, next;

    /*User data*/
    int key;
};

Node Node_Create(int key);
void Node_Delete(Node n);

typedef struct _Tree *Tree;
struct _Tree
{
    int num_elements;
    Node root;

    /*The first node in DDL*/
    Node first, last;
};

Tree Tree_Create(void);
Node Tree_AddNode(Tree t, Node n);
void Tree_Print(Tree t);
void Tree_Inorder(Tree t);
void Tree_Preorder(Tree t);
void Tree_Postorder(Tree t);
Node Tree_Find(Tree t, int key);
void Tree_Delete(Tree t);
#endif
