#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


#include "tree.h"
#include "util.h"

#include <iostream>
#include <queue>
#include <string>
#include <set>
using namespace std;

template <class KEY>
void Node<KEY>::SetLeftNode(Node<KEY> *left)
{
    if(!IS_NULL(left))
    {
        left->parent = this;
    }
    this->left = left;
    UpdateHeight();
}

template <class KEY>
void Node<KEY>::SetRightNode(Node<KEY> *right)
{
    if(!IS_NULL(right))
    {
        right->parent = this;
    }
    this->right = right;
    UpdateHeight();
}

template <class KEY>
int Node<KEY>::UpdateHeight(void)
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

template <class KEY>
int Node<KEY>::GetBalance(void)
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

template <class KEY>
void Node<KEY>::InsertBefore(Node<KEY> *node)
{
    Node *prev = this->prev;
    node->next = this;
    node->prev = this->prev;
    this->prev = node;
    if(!IS_NULL(prev))
    {
        prev->next = node;
    }
    return;
}

template <class KEY>
void Node<KEY>::InsertAfter(Node<KEY> *node)
{
    Node *next = this->next;
    node->next = next;
    node->prev = this;
    this->next = node;
    if(!IS_NULL(next))
    {
        next->prev = node;
    }
    return;
}

template <class KEY>
Tree<KEY>::~Tree()
{
    Node<KEY> *tmp = this->first, *old;
    while(!IS_NULL(tmp))
    {
        old = tmp->GetNext();
        delete tmp;
        tmp = old;
    }
}

template <class KEY>
void Tree<KEY>::RotateLeft(Node<KEY> *n)
{
    Node<KEY> *p = n->GetParent(), *temp = n->GetRight();
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

template <class KEY>
void Tree<KEY>::RotateRight(Node<KEY> *n)
{
    Node<KEY> *p = n->GetParent(), *temp = n->GetLeft();
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

template <class KEY>
void Tree<KEY>::BalanceAt(Node<KEY> *n)
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

template <class KEY>
Node<KEY>* Tree<KEY>::AddNode(const KEY key, const string data)
{
    Node<KEY> *n = new Node<KEY>(key);
    if(IS_NULL(this->root))
    {
        num_keys++;
        num_elements++;
        this->root = n;
        this->last = this->first = n;
        n->SetData(data);
        return n;
    }

    Node<KEY> *tmp = this->root;
    while(!IS_NULL(tmp))
    {
        if(tmp->GetKey() > key)
        {
            if(IS_NULL(tmp->GetLeft()))
            {
                tmp->SetLeftNode(n);
                tmp->InsertBefore(n);
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
                Node<KEY> *n2 = tmp;
                n2->InsertAfter(n);
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
            num_elements++;
            tmp->SetData(data);
            delete n;
            /* There is a node already with the same key.
             * Return the old node*/
            return tmp;
        }
    }
    this->num_keys++;
    this->num_elements++;
    n->SetData(data);
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

template <class KEY>
Node<KEY>* Tree<KEY>::FindNode(KEY key)
{
    Node<KEY> *tmp = this->root;
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

template <class KEY>
void Tree<KEY>::PrintTree(void)
{
    int i = 0, count = 1;
    std::queue<Node<KEY>*> myq;
    myq.push(this->root);
    while(!myq.empty())
    {
        Node<KEY> *tmp = myq.front(); myq.pop();
        if(!IS_NULL(tmp))
            cout<<" "<<tmp->GetKey()<<" ";
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

template <class KEY>
void Tree<KEY>::CheckList(void)
{
    int count = 1;
    KEY key = 0;
    Node<KEY> *tmp = this->first;
    while(!IS_NULL(tmp))
    {
        
        for(set<string>::iterator it = tmp->GetData().begin(); it != tmp->GetData().end(); ++it)
        {
            cout<<count<<"> "<<tmp->GetKey()<<" = "<<*it<<endl;
            count++;
        }
        if(key <= tmp->GetKey())
            key = tmp->GetKey();
        else
        {
            abort();
        }
        tmp = tmp->GetNext();
    }

    cout<<"Number of keys : "<<num_keys<<endl<<"Number of elements : "<<num_elements<<endl;
}

template <class KEY>
void Tree<KEY>::inorder_priv(Node<KEY> *n)
{
    if(IS_NULL(n)) return;
    inorder_priv(n->GetLeft());
    cout<<" "<<n->GetKey()<<" ";
    inorder_priv(n->GetRight());
}
template <class KEY>
void Tree<KEY>::InOrder(void)
{
    printf("\n\n");
    inorder_priv(root);
    printf("\n\n");
}

template <class KEY>
void Tree<KEY>::preorder_priv(Node<KEY> *n)
{
    if(IS_NULL(n)) return;
    cout<<" "<<n->GetKey()<<" ";
    preorder_priv(n->GetLeft());
    preorder_priv(n->GetRight());
}

template <class KEY>
void Tree<KEY>::PreOrder(void)
{
    printf("\n\n");
    preorder_priv(root);
    printf("\n\n");
}

template <class KEY>
void Tree<KEY>::postorder_priv(Node<KEY> *n)
{
    if(IS_NULL(n)) return;
    postorder_priv(n->GetRight());
    cout<<" "<<n->GetKey()<<" ";
    postorder_priv(n->GetLeft());
}

template <class KEY>
void Tree<KEY>::PostOrder(void)
{
    printf("\n\n");
    postorder_priv(root);
    printf("\n\n");
}

template <class KEY>
int Tree<KEY>::height_priv(Node<KEY> *n)
{
    if(IS_NULL(n))
        return 0;
    return 1 + MAX(height_priv(n->GetLeft()), height_priv(n->GetRight()));
}

template <class KEY>
int Tree<KEY>::Height(void)
{
    return height_priv(this->root);
}

template class Tree<int>;
