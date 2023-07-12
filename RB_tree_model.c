#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define numOfWords 100
#define length 32

/**
 * Small collection of functions that implements a Red-Black tree data structure working as a dictionary.
 * I chose an iterative approach, every function can be reimplemented using a recursive approach.
 * "main" contains an example of usage (test.txt contains required words)
 */

/**
 * RB node, if color == true then color is red otherwise is red
 */
typedef struct node
{
    struct node *father;
    struct node *son_l;
    struct node *son_r;
    bool color;
    char key[];
}node;


/**
 * Sentinel creation and assignment
 * @param nodeToAssign is node for sentinel assignment
 * @return sentinel node
 */
void initializeSentinel (node **nodeToAssign) {
    node *sentinel = (node *)malloc(sizeof(node));
    sentinel->father = sentinel;
    sentinel->son_l = sentinel;
    sentinel->son_r = sentinel;
    sentinel->color = false;

    *nodeToAssign = sentinel;
}

/**
 * RB root
 */
node *root = NULL;

/**
 * String comparison
 * @param s1 is 1st string
 * @param s2 is 2nd string
 * @return -1 if s1 < s2, 1 if s1 > s2, 0 if s1 == s2
 */
int stringCompare (char const s1[], char const s2[])
{
    for (int i = 0; i < length; i++)
    {
        if (s1[i] < s2[i])
            return -1;
        if (s1[i] > s2[i])
            return 1;
    }
    return 0;
}

/**
 * Extract minimum from tree
 * @param recursiveNode is node used in recursion
 * @param sentinel is sentinel used in RB tree
 * @return minimum node
 */
node *minimum(node *recursiveNode, node *sentinel)
{
    node *temp = recursiveNode;
    while (temp->son_l != sentinel)
        temp = temp->son_l;
    return temp;
}

/**
 * Extract maximum from tree
 * @param recursiveNode is node used in recursion
 * @param sentinel is sentinel used in RB tree
 * @return maximum node
 */
node *maximum(node *recursiveNode, node *sentinel)
{
    node *temp = recursiveNode;
    while (temp->son_r != sentinel)
        temp = temp->son_r;
    return temp;
}

/**
 * Extract successor of given node
 * @param givenNode is given node
 * @param sentinel is sentinel used in RB tree
 * @return successor node
 */
node *successor(node *givenNode, node *sentinel)
{
    if (givenNode->son_r != sentinel)
        return minimum(givenNode->son_r, sentinel);

    node *temp = givenNode->father;

    while (temp != sentinel && temp->son_r == givenNode)
    {
        givenNode = temp;
        temp = temp->father;
    }

    return temp;
}

/**
 * Print tree with colours
 * @param nodo is root
 * @param sentinel is sentinel used in RB tree
 */
void printTreeWithColours (node *nodo, node *sentinel)
{
    node *handle = minimum(nodo, sentinel);
    node *upper_bound = maximum(nodo, sentinel);

    while (1)
    {
        if (handle != upper_bound)
        {
            printf("%s ", handle->key);
            if (handle->color == true)
                printf ("RED\n");
            else
                printf ("BLACK\n");
            handle = successor(handle, sentinel);
        }
        else
        {
            printf("%s ", handle->key);
            if (handle->color == true)
                printf ("RED\n");
            else
                printf ("BLACK\n");
            break;
        }
    }
}

/**
 * Left rotation for RB tree balance
 * @param centerOfRotation is center of rotation
 * @param sentinel is sentinel used in RB tree
 */
void leftRotation(node *centerOfRotation, node *sentinel)
{
    node *temp = centerOfRotation->son_r;
    centerOfRotation->son_r = temp->son_l;
    if (temp->son_l != sentinel)
        temp->son_l->father = centerOfRotation;

    temp->father = centerOfRotation->father;

    if (centerOfRotation->father == sentinel)
        root = temp;
    else if (centerOfRotation == centerOfRotation->father->son_l)
        centerOfRotation->father->son_l = temp;
    else
        centerOfRotation->father->son_r = temp;

    temp->son_l = centerOfRotation;
    centerOfRotation->father = temp;
}

/**
 * Right rotation for RB tree balance
 * @param centerOfRotation is center of rotation
 * @param sentinel is sentinel used in RB tree
 */
void rightRotation(node *centerOfRotation, node *sentinel)
{
    node *temp = centerOfRotation->son_l;
    centerOfRotation->son_l = temp->son_r;

    if (temp->son_r != sentinel)
        temp->son_r->father = centerOfRotation;

    temp->father = centerOfRotation->father;

    if (centerOfRotation->father == sentinel)
        root = temp;
    else if (centerOfRotation == centerOfRotation->father->son_r)
        centerOfRotation->father->son_r = temp;
    else
        centerOfRotation->father->son_l = temp;

    temp->son_r = centerOfRotation;
    centerOfRotation->father = temp;
}

/**
 * Balancing function
 * @param givenNode is given node
 * @param sentinel is sentinel used in RB tree
 */
void balancingRBtree(node *givenNode, node *sentinel)
{
    while (givenNode->father->color == true)
    {
        if (givenNode->father == givenNode->father->father->son_l)
        {
            node *temp = givenNode->father->father->son_r;
            if (temp->color == true)
            {
                givenNode->father->color = false;
                temp->color = false;
                givenNode->father->father->color = true;
                givenNode = givenNode->father->father;
            }
            else
            {
                if (givenNode == givenNode->father->son_r)
                {
                    givenNode = givenNode->father;
                    leftRotation(givenNode, sentinel);
                }
                else
                {
                    givenNode->father->color = false;
                    givenNode->father->father->color = true;
                    rightRotation(givenNode->father->father, sentinel);
                }
            }
        }
        else
        {
            node *temp = givenNode->father->father->son_l;
            if (temp->color == true)
            {
                givenNode->father->color = false;
                temp->color = false;
                givenNode->father->father->color = true;
                givenNode = givenNode->father->father;
            }
            else
            {
                if (givenNode == givenNode->father->son_l)
                {
                    givenNode = givenNode->father;
                    rightRotation(givenNode, sentinel);
                }
                else
                {
                    givenNode->father->color = false;
                    givenNode->father->father->color = true;
                    leftRotation(givenNode->father->father, sentinel);
                }
            }
        }
    }
    root->color = false;
}

/**
 * Insert element
 * @param toBeInserted is string to be inserted
 * @param sentinel is sentinel used in RB tree
 */
void insertElement(char toBeInserted[], node *sentinel)
{
    node *nodo = (node *)malloc(sizeof(node) + sizeof(char) * length);
    nodo->color = true;
    memcpy(nodo->key, toBeInserted, length);

    node *pred = sentinel;
    node *curr;

    curr = root;

    while (curr != sentinel)
    {
        pred = curr;
        if (stringCompare(nodo->key, curr->key) < 0)
            curr = curr->son_l;
        else
            curr = curr->son_r;
    }

    nodo->father = pred;

    if (pred == sentinel)
    {
        root = nodo;
        root->father = sentinel;
    }
    else if (stringCompare(nodo->key, pred->key) < 0)
        pred->son_l = nodo;
    else
        pred->son_r = nodo;

    nodo->son_l = sentinel;
    nodo->son_r = sentinel;
    nodo->color = true;

    balancingRBtree(nodo, sentinel);
}

/**
 * Reparation after operations in RB tree
 * @param givenNode is given node
 * @param sentinel is sentinel used in RB tree
 */
void reparationRBtree (node *nodo, node *sentinel)
{
    while (nodo != root && nodo->color == false)
    {
        if (nodo == nodo->father->son_l)
        {
            node *temp = nodo->father->son_r;
            if (temp->color == true)
            {
                temp->color = false;
                nodo->father->color = true;
                leftRotation(nodo->father, sentinel);
                temp = nodo->father->son_r;
            }
            if (temp->son_l->color == false && temp->son_r->color == false)
            {
                temp->color = true;
                nodo = nodo->father;
            }
            else
            {
                if (temp->son_r->color == false)
                {
                    temp->son_l->color = false;
                    temp->color = true;
                    rightRotation(temp, sentinel);
                    temp = nodo->father->son_r;
                }
                temp->color = nodo->father->color;
                nodo->father->color = false;
                temp->son_r->color = false;
                leftRotation(nodo->father, sentinel);
                nodo = root;
            }
        }
        else
        {
            if (nodo == nodo->father->son_r)
            {
                node *temp = nodo->father->son_l;
                if (temp->color == true)
                {
                    temp->color = false;
                    nodo->father->color = true;
                    rightRotation(nodo->father, sentinel);
                    temp = nodo->father->son_l;
                }
                if (temp->son_l->color == false && temp->son_r->color == false)
                {
                    temp->color = true;
                    nodo = nodo->father;
                }
                else
                {
                    if (temp->son_l->color == false) {
                        temp->son_r->color = false;
                        temp->color = true;
                        leftRotation(temp, sentinel);
                        temp = nodo->father->son_l;
                    }
                    temp->color = nodo->father->color;
                    nodo->father->color = false;
                    temp->son_l->color = false;
                    rightRotation(nodo->father, sentinel);
                    nodo = root;
                }
            }
        }
    }
    nodo->color = false;
}

/**
 * Used in deletion of node
 * @param node1 is first node
 * @param node2 is second node
 * @param sentinel is sentinel used in RB tree
 */
void substituteSubtree (node *node1, node *node2, node *sentinel)
{
    if (node1->father == sentinel)
        root = node2;
    else if (node1 == node1->father->son_l)
        node1->father->son_l = node2;
    else
        node1->father->son_r = node2;

    node2->father = node1->father;
}

/**
 * Delete node
 * @param nodeToBeDeleted is node to be deleted
 * @param sentinel is sentinel used in RB tree
 */
void deleteNode(node *nodeToBeDeleted, node *sentinel)
{
    node *temp1 = nodeToBeDeleted;
    node *temp2 = NULL;
    bool old_color = temp1->color;

    if (nodeToBeDeleted->son_l == sentinel)
    {
        temp2 = nodeToBeDeleted->son_r;
        substituteSubtree(nodeToBeDeleted, nodeToBeDeleted->son_r, sentinel);
    }
    else if (nodeToBeDeleted->son_r == sentinel)
    {
        temp2 = nodeToBeDeleted->son_l;
        substituteSubtree(nodeToBeDeleted, nodeToBeDeleted->son_l, sentinel);
    }
    else
    {
        temp1 = minimum(nodeToBeDeleted->son_r, sentinel);
        old_color = temp1->color;
        temp2 = temp1->son_r;

        if (temp1->father == nodeToBeDeleted)
            temp2->father = temp1;
        else
        {
            substituteSubtree(temp1, temp1->son_r, sentinel);
            temp1->son_r = nodeToBeDeleted->son_r;
            temp1->son_r->father = temp1;
        }

        substituteSubtree(nodeToBeDeleted, temp1, sentinel);
        temp1->son_l = nodeToBeDeleted->son_l;
        temp1->son_l->father = temp1;
        temp1->color = nodeToBeDeleted->color;
    }

    if (old_color == false)
        reparationRBtree(temp2, sentinel);
}

/**
 * Reduce tree using a custom condition
 * @param sentinel is sentinel used in RB tree
 */
void reduceTree (node *sentinel)
{
    node *handle = minimum(root, sentinel);
    node *upper_bound = maximum(root, sentinel);
    bool end = false;
    bool del = false;

    while (!end)
    {
        /**
         * Deletion condition
         */
        if (handle->key[0] == 'a' || handle->key[0] == 'e' || handle->key[0] == 'i' || handle->key[0] == 'o' || handle->key[0] == 'u') {
            deleteNode(handle, sentinel);
            del = true;
        }

        if (del) {
            handle = minimum(root, sentinel);
            upper_bound = maximum(root, sentinel);
            del = false;
        }
        else if (handle == upper_bound)
            end = true;
        else
            handle = successor(handle, sentinel);
    }
}

/**
 * Example of usage, insert 100 words (32 chars length) and delete words using example condition (words starting with a vowel in this example)
 */
int main()
{
    char buffer[length];
    int trash = 0;
    node *sentinel = NULL;
    initializeSentinel(&sentinel);
    root = sentinel;

    for (int i = 0; i < numOfWords; i++)
    {
        trash = scanf ("%s", buffer);
        if (trash == 0)
            return -1;

        insertElement(buffer, sentinel);
    }

    puts("Complete tree:");
    printTreeWithColours(root, sentinel);

    reduceTree(sentinel);
    printf("\n");
    puts("Reduced tree:");
    printTreeWithColours(root, sentinel);
}
