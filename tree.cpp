#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "tree.h"
#include "util.h"

void Node::SetLeftNode(Node *left)
{
    if(!IS_NULL(left))
    {
        left->parent = this;
    }
    this->left = left;
    UpdateHeight();
}

void Node::SetRightNode(Node *right)
{
    if(!IS_NULL(right))
    {
        right->parent = this;
    }
    this->right = right;
    UpdateHeight();
}

int Node::UpdateHeight(void)
{
    this->height = 0;
    if(!IS_NULL(this->left) && !IS_NULL(this->right))
    {
       int left_height = this->left->height;
       int right_height = this->right->height;
       this->height = MAX(left_height, right_height) + 1;
    }
    else if(!IS_NULL(this->left))
    {
        this->height = this->left->height + 1;
    }
    else if(!IS_NULL(this->right))
    {
        this->height = this->right->height + 1;
    }
    return this->height;
}

int Node::GetBalance(void)
{
    int balance = 0;

    if(!IS_NULL(this->left) && !IS_NULL(this->right))
    {
        balance = this->left->height - this->right->height;
    }
    else if(!IS_NULL(this->left))
    {
        balance = this->left->height + 1;
    }
    else if(!IS_NULL(this->right))
    {
        balance = (this->right->height + 1) * (-1);
    }   
    return balance;
}


void Tree::RotateLeft(Node *n)
{
    Node *p = n->GetParent(), *temp = n->GetRight();
    enum {LEFT, RIGHT} side = LEFT;

    if(!IS_NULL(p))
    {
        side = (p->GetLeft() == n) ? LEFT : RIGHT;
    }
    n->SetRightNode(temp->GetLeft());
    temp->SetLeftNode(n);
    
    if(!IS_NULL(p))
    {
        (side == LEFT) ? (p->SetLeftNode(temp)) : (p->SetRightNode(temp));
    }
    else
    {
        this->root = temp;
        temp->SetParentNode(NULL); 
    }
}

void Tree::RotateRight(Node *n)
{
    Node *p = n->GetParent(), *temp = n->GetLeft();
    enum {LEFT, RIGHT} side = LEFT;

    if(!IS_NULL(p))
    {
        side = (p->GetLeft() == n) ? LEFT : RIGHT;
    }
    n->SetLeftNode(temp->GetRight());
    temp->SetRightNode(n);
    
    if(!IS_NULL(p))
    {
        (side == LEFT) ? (p->SetLeftNode(temp)) : (p->SetRightNode(temp));
    }
    else
    {
        this->root = temp;
        temp->SetParentNode(NULL); 
    }
}

void Tree::BalanceAt(Node *n)
{
    int bal = n->GetBalance();
    if(bal > 1)
    {
        if(n->GetLeft()->GetBalance() < 0)
        {
            RotateLeft(n->GetLeft());
        }
        RotateRight(n);
    }
    else if(bal < -1)
    {
        if(n->GetRight()->GetBalance() > 0)
        {
            RotateRight(n->GetRight());
        }
        RotateLeft(n);
    }
}

Node* Tree::AddNode(Node *n)
{
    if(IS_NULL(this->root))
    {
        root = first = last = n;
        return n;
    }

    Node *tmp = this->root;
    while(!IS_NULL(tmp))
    {
        if(tmp->GetKey() > n->GetKey())
        {
            if(IS_NULL(tmp->GetLeft()))
            {
                tmp->SetLeftNode(n);

                /*now update the doubly linked list*/
                Node *prev = tmp->GetPrev();
                if(!IS_NULL(prev)) prev->SetNext(n);
                tmp->SetPrev(n);
                n->SetPrev(prev);
                n->SetNext(tmp);
                if(this->first->GetKey() > n->GetKey())
                    this->first = n;
                break;
            }
            else
            {
                tmp = tmp->GetLeft();
            }
        }
        else if(tmp->GetKey() < n->GetKey())
        {
            if(IS_NULL(tmp->GetRight()))
            {
                tmp->SetRightNode(n);

                /*Now update the doubly linked list*/
                Node *next = tmp->GetNext();
                n->SetNext(next);
                if(!IS_NULL(next)) next->SetPrev(n);
                tmp->SetNext(n);
                n->SetPrev(tmp);
                if(this->last->GetKey() < n->GetKey())
                    this->last = n;
                break;
            }
            else
            {
                tmp = tmp->GetRight();
            }
        }
        else
        {
            /* There is a node already with the same key.
             * Return the old node*/
            return tmp;
        }       
    }
    this->num_elements++;

    /*Now check if the tree needs rebalancing*/
    tmp = n;
    while(!IS_NULL(tmp))
    {
        tmp->UpdateHeight();
        this->BalanceAt(tmp);
        tmp = tmp->GetParent();
    }
    return n;   
}

Node* Tree::FindNode(int key)
{
    Node *tmp = this->root;
    while(!IS_NULL(tmp))
    {
        if(tmp->GetKey() > key)
        {
            tmp = tmp->GetLeft();
        }
        else if(tmp->GetKey() < key)
        {
            tmp = tmp->GetRight();
        }
        else
        {
            break;
        }
    }
    return tmp;
}

Tree::~Tree()
{
    Node *tmp = this->first, *old;
    while(!IS_NULL(tmp))
    {
        old = tmp->GetNext();
        delete tmp;
        tmp = old;
    }
}

#include <queue>
using namespace std;
void Tree::PrintTree(void)
{
    int i = 0, count = 1;
    std::queue<Node*> myq;
    myq.push(this->root);
    while(!myq.empty())
    {
        Node *tmp = myq.front(); myq.pop();
        if(!IS_NULL(tmp))
            printf(" %3d ", tmp->GetKey());
        else
            printf(" %3d ", 0);
        i++;
        if(i == count)
        {
            i = 0;
            count <<= 1;
            printf("\n");
        }
        if(tmp != NULL)
        {
            myq.push(tmp->GetLeft());
            myq.push(tmp->GetRight());
        }
    }
}

void Tree::PrintList(void)
{
    Node *tmp = this->first;
    printf("\n\n");
    while(!IS_NULL(tmp))
    {        
        printf(" %4d ", tmp->GetKey());
        tmp = tmp->GetNext();
    }
    printf("\n\n");
}

void Tree::inorder_priv(Node *n)
{
    if(IS_NULL(n)) return;
    inorder_priv(n->GetLeft());
    printf(" %4d ", n->GetKey());
    inorder_priv(n->GetRight());
}
void Tree::InOrder(void)
{
    printf("\n\n");
    inorder_priv(root);
    printf("\n\n");
}

void Tree::preorder_priv(Node *n)
{
    if(IS_NULL(n)) return;
    printf(" %4d ", n->GetKey());
    preorder_priv(n->GetLeft());
    preorder_priv(n->GetRight());
}

void Tree::PreOrder(void)
{
    printf("\n\n");
    preorder_priv(root);
    printf("\n\n");
}

void Tree::postorder_priv(Node *n)
{
    if(IS_NULL(n)) return;
    postorder_priv(n->GetRight());
    printf(" %4d ", n->GetKey());
    postorder_priv(n->GetLeft());
}

void Tree::PostOrder(void)
{
    printf("\n\n");
    postorder_priv(root);
    printf("\n\n");
}

int Tree::height_priv(Node *n)
{
	if(IS_NULL(n))
		return 0;
	return 1 + MAX(height_priv(n->GetLeft()), height_priv(n->GetRight()));
}

int Tree::Height(void)
{
	return height_priv(this->root);
}
