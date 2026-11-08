typedef int NodeElem_t;
#define PRINT_NODE_FORMAT "%d"
#define TREE_SORTING

#include "binary-tree.h"
#include "binary-tree.cpp"
#include "graphics.cpp"

int main(){

    NodeElem_t arr[MAX_ARR_SIZE] = {};
    size_t size = getArr(arr);

    BinaryTree_t binaryTree = {};
    initBinaryTree(&binaryTree);

    treeSorting(arr, size, &binaryTree);

    printSortedTree(&binaryTree);

    destroyBinaryTree(&binaryTree);

    return 0;
}

size_t getArr(NodeElem_t* arr){

    assert(arr);

    size_t size = 0;

    printf("Enter array elements.\nWrite ';' to complete the input.\n");

    while (size < MAX_ARR_SIZE && scanf("%d", &arr[size]) == 1) {
        size++;
    }

    return size;

}

ErrorCode treeSorting(NodeElem_t* arr, size_t size, BinaryTree_t* binaryTree){

    assert(binaryTree);
    assert(arr);

    verifyBinaryTree(binaryTree);

    for (size_t i = 0; i < size; i++) {

        ErrorCode error = insertObject(binaryTree, arr[i]);
        if (error != ERR_OK) return error;
    }

    return ERR_OK;
}

void printSortedTree(BinaryTree_t* binaryTree){

    assert(binaryTree);

    printf("Result: ");
    printSortedTreeNode(binaryTree->root);
    putchar('\n');

    createDebugImage(binaryTree);
}

void printSortedTreeNode(const Node* node){

    if (node == NULL) return;

    printSortedTreeNode(node->left);

    printf("%d ", node->data);

    printSortedTreeNode(node->right);

}

ErrorCode insertObject(BinaryTree_t* binaryTree, NodeElem_t value){

    assert(binaryTree);

    Node* currentNode = binaryTree->root;
    Node* parent = NULL;

    while(currentNode != NULL){

        parent = currentNode;

        if(value < currentNode->data){
            currentNode = currentNode->left;
        }
        else{
            currentNode = currentNode->right;
        }

    }

    Node* newNode = NULL;
    ErrorCode error = allocateMemoryForNode(&newNode);
    if(error != ERR_OK) return error;

    newNode->data = value;
    newNode->parent = parent;

    if(parent == NULL){
        binaryTree->root = newNode;
    }
    else if(value < parent->data){
        parent->left = newNode;
    }
    else{
        parent->right = newNode;
    }

    binaryTree->dim++;

    return ERR_OK;
}

