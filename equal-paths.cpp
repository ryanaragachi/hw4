#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <algorithm>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

bool checkEqualPaths(Node* node, int depth, int& expectedLeafDepth) {
    if (node == nullptr) return true;

    if (node->left == nullptr && node->right == nullptr) {
        if (expectedLeafDepth == -1) {
            expectedLeafDepth = depth;
        }
        return depth == expectedLeafDepth;
    }

    return checkEqualPaths(node->left, depth + 1, expectedLeafDepth) &&
           checkEqualPaths(node->right, depth + 1, expectedLeafDepth);
}



bool equalPaths(Node * root)
{
    // Add your code below
    int expectedLeafDepth = -1;
    return checkEqualPaths(root, 0, expectedLeafDepth);

}

