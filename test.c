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
    n->height = height;
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
        balance = Node_GetHeight(n->left) + 1;
    }
    else if(!IS_NULL(n->right))
    {
        balance = (Node_GetHeight(n->right) + 1) * (-1) ;
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


typedef struct _Tree *Tree;
struct _Tree
{
    int num_elements;
    Node root;
};

Tree Tree_Create(void)
{
    Tree t = (Tree)malloc(sizeof(struct _Tree));
    t->root = NULL;
    t->num_elements = 0;
    return t;
}


void RotateLeft(Tree t, Node n)
{
    Node p = n->parent, temp = NULL;
    enum { LEFT, RIGHT } side;
    if(!IS_NULL(p))
    {
        side = (p->left == n) ? LEFT : RIGHT;
    }
    temp = n->right;
    Node_SetRightNode(n, temp->left);
    Node_SetLeftNode(temp, n);
    if(!IS_NULL(p))
    {
        if(side == LEFT)
        {
            Node_SetLeftNode(p, temp);
        }
        else 
        {
            Node_SetRightNode(p, temp);
        }
    }
    else
    {
        t->root = temp;
        temp->parent = NULL;
    }
}

void RotateRight(Tree t, Node n)
{
    Node p = n->parent, temp = NULL;
    enum { LEFT, RIGHT } side;
    if(!IS_NULL(p))
    {
        side = (p->left == n) ? LEFT : RIGHT;
    }
    temp = n->left;
    Node_SetLeftNode(n, temp->right);
    Node_SetRightNode(temp, n);
    if(!IS_NULL(p))
    {
        if(side == LEFT)
        {
            Node_SetLeftNode(p, temp);
        }
        else 
        {
            Node_SetRightNode(p, temp);
        }
    }
    else
    {
        t->root = temp;
        temp->parent = NULL;
    }
}

void Tree_BalanceAt(Tree t, Node n)
{
    int bal = Node_GetBalance(n);
    if(bal > 1)
    {
        if(Node_GetBalance(n->left) < 0)
            RotateLeft(t, n->left);
        RotateRight(t, n);
    }
    else if(bal < -1)
    {
        if(Node_GetBalance(n->right) > 0)
            RotateRight(t, n->right);
        RotateLeft(t, n);
    }    
}

int Tree_AddNode(Tree t, int data)
{
    Node n = Node_Create(data);
    if(IS_NULL(t->root))
    {
        t->root = n;
        return 0;
    }
    else
    {
        Node tmp = t->root;
        while(1)
        {
            if(Node_GetData(tmp) > data)
            {
                if(IS_NULL(tmp->left))
                {
                    Node_SetLeftNode(tmp, n);
                    break;
                }
                else
                {
                    tmp = tmp->left;   
                }
            }
            else if(Node_GetData(tmp) < data)
            {
                if(IS_NULL(tmp->right))
                {
                    Node_SetRightNode(tmp, n);
                    break;
                }
                else
                {
                    tmp = tmp->right;
                }
            }
            else
            {
                return 0;
            }
        }
        t->num_elements++;
        tmp = n;
        while(!IS_NULL(tmp))
        {
            Node_UpdateHeight(tmp);
            Tree_BalanceAt(t, tmp);
            tmp = tmp->parent;
        }
    }
}

void Tree_Print(Tree t)
{
    int i = 0, count = 1;
    int back=0, top = 0;
    Node queue[1000];
    queue[top++] = t->root;
    while(back != top)
    {
        Node tmp = queue[back++];
        if(!IS_NULL(tmp))
            printf("  %3d  ", tmp->data);
        else
            printf("  %3d  ", 0);
        i++;
        if(i == count)
        {
            i = 0;
            count <<= 1;
            printf("\n");
        }
        if(tmp != NULL)
        {
            queue[top++] = tmp->left;
            queue[top++] = tmp->right;
        }
    }
}

void Tree_Inorder_priv(Node root)
{
    if(IS_NULL(root)) return;
    Tree_Inorder_priv(root->left);
    printf("  %d  ", root->data);
    Tree_Inorder_priv(root->right);
}
void Tree_Inorder(Tree t)
{
    Tree_Inorder_priv(t->root);
}

void Tree_Preorder_priv(Node root)
{
    if(IS_NULL(root)) return;
    printf("  %d  ", root->data);
    Tree_Preorder_priv(root->left);    
    Tree_Preorder_priv(root->right);
}
void Tree_Preorder(Tree t)
{
    Tree_Preorder_priv(t->root);
}

int main(int argc, char *argv[])
{
    int i = 0;
    Tree t = Tree_Create();
    for(i = 1; i <= 20; i++)
    {
        Tree_AddNode(t, i);
    }
    Tree_Inorder(t);
    printf("\n");
    Tree_Preorder(t);
    printf("\n");
    Tree_Print(t);
    printf("\n");
    return 0;
}
