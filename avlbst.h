#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rotateLeft(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* x)
{
    AVLNode<Key, Value>* y = x->getRight();
    x->setRight(y->getLeft());
    if (y->getLeft()) y->getLeft()->setParent(x);
    y->setParent(x->getParent());
    if (!x->getParent()) this->root_ = y;
    else if (x == x->getParent()->getLeft()) x->getParent()->setLeft(y);
    else x->getParent()->setRight(y);
    y->setLeft(x);
    x->setParent(y);

    int b = y->getBalance();
    if (b <= 0) {
        x->setBalance(x->getBalance() - 1 - std::max(0, b));
        y->setBalance(y->getBalance() - 1 + std::min(0, static_cast<int>(x->getBalance())));
    } else {
        x->setBalance(0);
        y->setBalance(0);
    }
    if (!y->getParent()) this->root_ = y;
}



template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* x)
{
    AVLNode<Key, Value>* y = x->getLeft();
    x->setLeft(y->getRight());
    if (y->getRight()) y->getRight()->setParent(x);
    y->setParent(x->getParent());
    if (!x->getParent()) this->root_ = y;
    else if (x == x->getParent()->getLeft()) x->getParent()->setLeft(y);
    else x->getParent()->setRight(y);
    y->setRight(x);
    x->setParent(y);

    int b = y->getBalance();
    if (b >= 0) {
        x->setBalance(x->getBalance() + 1 - std::min(0, b));
        y->setBalance(y->getBalance() + 1 + std::max(0, static_cast<int>(x->getBalance())));
    } else {
        x->setBalance(0);
        y->setBalance(0);
    }
    if (!y->getParent()) this->root_ = y;
}



template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
     if (this->root_ == nullptr) {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        return;
    }

    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parent = nullptr;

    while (curr != nullptr) {
        parent = curr;
        if (new_item.first < curr->getKey()) {
            curr = curr->getLeft();
        } else if (new_item.first > curr->getKey()) {
            curr = curr->getRight();
        } else {
            curr->setValue(new_item.second);
            return;
        }
    }

    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
    if (new_item.first < parent->getKey()) {
        parent->setLeft(newNode);
    } else {
        parent->setRight(newNode);
    }

    AVLNode<Key, Value>* node = newNode;

    while (parent != nullptr) {
        if (node == parent->getLeft()) parent->updateBalance(-1);
        else parent->updateBalance(1);

        if (parent->getBalance() == 0) break;
        else if (parent->getBalance() == -2) {
            if (node->getBalance() == 1) {
                rotateLeft(node);
                rotateRight(parent);
            } else {
                rotateRight(parent);
            }
            break;
        } else if (parent->getBalance() == 2) {
            if (node->getBalance() == -1) {
                rotateRight(node);
                rotateLeft(parent);
            } else {
                rotateLeft(parent);
            }
            break;
        }

        node = parent;
        parent = parent->getParent();
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
   AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if (!node) return;

    AVLNode<Key, Value>* toFix = nullptr;
    AVLNode<Key, Value>* parent = node->getParent();
    int balanceDirection = 0;

    if (node->getLeft() && node->getRight()) {
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(node));
        this->nodeSwap(node, pred);
        parent = node->getParent();
    }

    AVLNode<Key, Value>* child = node->getLeft() ? node->getLeft() : node->getRight();
    if (child) child->setParent(parent);

    if (!parent) {
        this->root_ = child;
    } else {
        if (node == parent->getLeft()) {
            parent->setLeft(child);
            balanceDirection = 1;
        } else {
            parent->setRight(child);
            balanceDirection = -1;
        }
    }

    delete node;

    node = parent;
    while (node) {
        node->updateBalance(balanceDirection);

        if (node->getBalance() == 1 || node->getBalance() == -1) break;
        else if (node->getBalance() == 2) {
            AVLNode<Key, Value>* r = node->getRight();
            if (r->getBalance() == -1) {
                AVLNode<Key, Value>* rl = r->getLeft();
                r->setLeft(rl->getRight());
                if (rl->getRight()) rl->getRight()->setParent(r);
                node->setRight(rl->getLeft());
                if (rl->getLeft()) rl->getLeft()->setParent(node);
                rl->setLeft(node);
                rl->setRight(r);
                rl->setParent(node->getParent());

                if (!node->getParent()) this->root_ = rl;
                else if (node == node->getParent()->getLeft()) node->getParent()->setLeft(rl);
                else node->getParent()->setRight(rl);

                node->setParent(rl);
                r->setParent(rl);

                if (rl->getBalance() == 1) node->setBalance(-1);
                else node->setBalance(0);
                if (rl->getBalance() == -1) r->setBalance(1);
                else r->setBalance(0);
                rl->setBalance(0);

                node = rl->getParent();
                balanceDirection = (node && rl == node->getLeft()) ? 1 : -1;
            } else {
                node->setRight(r->getLeft());
                if (r->getLeft()) r->getLeft()->setParent(node);
                r->setLeft(node);
                r->setParent(node->getParent());

                if (!node->getParent()) this->root_ = r;
                else if (node == node->getParent()->getLeft()) node->getParent()->setLeft(r);
                else node->getParent()->setRight(r);

                node->setParent(r);

                if (r->getBalance() == 0) {
                    node->setBalance(1);
                    r->setBalance(-1);
                    break;
                } else {
                    node->setBalance(0);
                    r->setBalance(0);
                    node = r->getParent();
                    balanceDirection = (node && r == node->getLeft()) ? 1 : -1;
                }
            }
        } else if (node->getBalance() == -2) {
            AVLNode<Key, Value>* l = node->getLeft();
            if (l->getBalance() == 1) {
                AVLNode<Key, Value>* lr = l->getRight();
                l->setRight(lr->getLeft());
                if (lr->getLeft()) lr->getLeft()->setParent(l);
                node->setLeft(lr->getRight());
                if (lr->getRight()) lr->getRight()->setParent(node);
                lr->setRight(node);
                lr->setLeft(l);
                lr->setParent(node->getParent());

                if (!node->getParent()) this->root_ = lr;
                else if (node == node->getParent()->getLeft()) node->getParent()->setLeft(lr);
                else node->getParent()->setRight(lr);

                node->setParent(lr);
                l->setParent(lr);

                if (lr->getBalance() == -1) node->setBalance(1);
                else node->setBalance(0);
                if (lr->getBalance() == 1) l->setBalance(-1);
                else l->setBalance(0);
                lr->setBalance(0);

                node = lr->getParent();
                balanceDirection = (node && lr == node->getLeft()) ? 1 : -1;
            } else {
                node->setLeft(l->getRight());
                if (l->getRight()) l->getRight()->setParent(node);
                l->setRight(node);
                l->setParent(node->getParent());

                if (!node->getParent()) this->root_ = l;
                else if (node == node->getParent()->getLeft()) node->getParent()->setLeft(l);
                else node->getParent()->setRight(l);

                node->setParent(l);

                if (l->getBalance() == 0) {
                    node->setBalance(-1);
                    l->setBalance(1);
                    break;
                } else {
                    node->setBalance(0);
                    l->setBalance(0);
                    node = l->getParent();
                    balanceDirection = (node && l == node->getLeft()) ? 1 : -1;
                }
            }
        } else if (node->getBalance() != 0) {
            break;
        } else {
            balanceDirection = (node->getParent() && node == node->getParent()->getLeft()) ? 1 : -1;
            node = node->getParent();
        }
    }

}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
