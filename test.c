#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IS_NULL(x) ((x) == NULL)
#define MAX(x, y)  ((x > y) ? (x) : (y))
typedef struct _Node *Node;
struct _Node
{
    /*Node internal Data*/
    int height;
    Node parent, left, right;

    /*User data*/
    int data;
};

Node Node_Create(int data)
{
    Node tmp = (Node)malloc(sizeof(struct _Node));
    memset(tmp, 0, sizeof(struct _Node));
    tmp->data = data;
    return tmp;
}

int Node_GetHeight(Node n)
{
    return n->height;
}

int Node_GetData(Node n)
{
    return n->data;
}

int Node_UpdateHeight(Node n)
{
    int height = 0;
    if(!IS_NULL(n->left) && !IS_NULL(n->right))
    {
        int left_height = Node_GetHeight(n->left);
        int right_height = Node_GetHeight(n->right);
        height = MAX(left_height, right_height) + 1;
    }
    else if(!IS_NULL(n->left))
    {
        height = Node_GetHeight(n->left) + 1;;
    }
    else if(!IS_NULL(n->right))
    {
        height = Node_GetHeight(n->right) + 1;;
    }
    return height;
}

int Node_GetBalance(Node n)
{
    int balance = 0;
    if(!IS_NULL(n->left) && !IS_NULL(n->right))
    {
        balance = Node_GetHeight(n->left) - Node_GetHeight(n->right); 
    }
    else if(!IS_NULL(n->left))
    {
        balance = Node_GetHeight(n->left) - 1;
    }
    else if(!IS_NULL(n->right))
    {
        balance = (Node_GetHeight(n->right) - 1) * (-1) ;
    }
    return balance;
}

Node Node_SetLeftNode(Node n, Node left)
{
    if(!IS_NULL(left))
    {
        left->parent = n;
    }
    n->left = left;
    Node_UpdateHeight(n);
    return n->left;
}

Node Node_SetRightNode(Node n, Node right)
{
    if(!IS_NULL(right))
    {
        right->parent = n;
    }
    n->right = right;
    Node_UpdateHeight(n);
    return n->right;
}

int main(int argc, char *argv[])
{
    Node n = Node_Create(10);    
    return 0;
}
