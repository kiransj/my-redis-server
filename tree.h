#ifndef _TREE_H_
#define _TREE_H_
#include "util.h"

class Node
{
    private:
        int height;
        /*Data structure needed for tree*/
        Node *parent, *left, *right;
        /*Data structure needed for Doubly linked list*/
        Node *prev, *next;

        /*Key and Data associated with it*/
        int key;

    public:
        Node(int k)
        {
            height = 0;
            key = k;
            parent = left = right = prev = next = NULL;
        }

        ~Node()
        {
            key = height = 0;
            parent = left = right = prev = next = NULL;
        }
        
        int GetHeight(void) { return height;}
        int GetKey(void) { return key; }
        int UpdateHeight(void);
        int GetBalance(void);

        void SetLeftNode(Node *left);
        void SetRightNode(Node *right);
        void SetParentNode(Node *parent) { this->parent = parent; }
        void SetNext(Node *next) { this->next = next; }
        void SetPrev(Node *prev) { this->prev = prev; }
        Node* GetLeft(void) { return left; }
        Node* GetRight(void) { return right; }
        Node* GetParent(void) { return parent; }
        Node* GetPrev(void) { return prev; }
        Node* GetNext(void) { return next; }
};

class Tree
{
    private:
        uint32_t num_elements;

        /*The root of the tree node*/
        Node *root;

        /*The first and the last node of the doubly linked list*/
        Node *first, *last;

    private:  /*Priv functions*/
        void RotateLeft(Node *n);
        void RotateRight(Node *n);
        void BalanceAt(Node *n);

        static void preorder_priv(Node *n);
        static void inorder_priv(Node *n);
        static void postorder_priv(Node *n);
    public:
        Tree()
        {
            root = first = last = NULL;
            num_elements = 0;
        }
        ~Tree();

        Node* AddNode(Node *n);
        Node* FindNode(int key);

        void PreOrder(void);
        void InOrder(void);
        void PostOrder(void);
        void PrintTree(void);
        void PrintList(void);
};

#endif
