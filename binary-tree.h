#include <TXLib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdlib.h>
#include <cctype> // для isspace
#include <locale.h>

const size_t MAX_PATH_LENGTH = 300;
const size_t MAX_PROGRAM_CAPACITY = 1000;
const size_t MAX_PARENT_COUNT = 100;
const size_t MAX_ANSWER_LENGTH = 50;
const size_t DIRECTORY_NAME_MAX_LENGTH = 50;
const size_t MAX_ARR_SIZE = 20;

const char* akinatorFileName = "akinator-tree.txt";

#include "../universal-features/error.h"
#include "../universal-features/colors.h"
#include "../universal-features/useful-functions.h"

enum menuPoint{
    GUESS = 1,
    WATCH = 2,
    IDENTIFY = 3,
    COMPARISON = 4,
    EXIT_GAME = 5
};

struct Node{
    NodeElem_t data;
    Node* left;
    Node* right;
    Node* parent;
};

struct BinaryTree_t{
    Node* root;
    size_t dim;
};

// Prototypes
ErrorCode readBinaryTree(const char* fileName, BinaryTree_t* binaryTree);
ErrorCode writeBinaryTree(BinaryTree_t* binaryTree, const char* fileName);
void initBinaryTree(BinaryTree_t* binaryTree);
void destroyBinaryTree(BinaryTree_t* binaryTree);
void destroyTreeNode(Node* node);
ErrorCode verifyBinaryTree(const BinaryTree_t* binaryTree, const char* function, int line);

//Sorting
ErrorCode treeSorting(NodeElem_t* arr, size_t size, BinaryTree_t* binaryTree);
size_t getArr(NodeElem_t* arr);
void printSortedTree(BinaryTree_t* binaryTree);
void printSortedTreeNode(const Node* node);
ErrorCode insertObject(BinaryTree_t* binaryTree, NodeElem_t value);

//Akinator
ErrorCode readLine(char* buffer, size_t capacity);
bool getAnswer();
Node* createNewAkinatorNode(const char* text, Node* parent);
ErrorCode getNewObject(BinaryTree_t* binaryTree, Node* node);
ErrorCode playAkinator(BinaryTree_t* binaryTree);
void addRootEl(BinaryTree_t* binaryTree);
ErrorCode changeFilesComplication(BinaryTree_t* binaryTree);
int getMenuPoint();
ErrorCode guessObject(BinaryTree_t* binaryTree);
void identifyObject(BinaryTree_t* binaryTree);
bool seachValue(Node* node, NodeElem_t value);
void printIdentify(Node* node, const char* value);
void printPathFromRoot(Node* node);
void speak(const char* text);
static Node* findObject(Node* node, const char* value);
bool isAncestor(Node* ancestor, Node* node);
Node* findCommonAncestor(Node* node1, Node* node2);
void printCommonConditions(Node* node);
void printCondition(Node* node);
void printDifferentConditions(Node* node, Node* ancestor);
void comparisonObjects(BinaryTree_t* binaryTree);
void toLowerCase(char* str);

// Graphics
ErrorCode createDebugImage(BinaryTree_t* binaryTree);



#define VERIFY_OK(binaryTree, function)                                                 \
    do {                                                                                \
        if (verifyBinaryTree((binaryTree), (function), __LINE__) != ERR_OK) {           \
            abort();                                                                    \
        }                                                                               \
    } while (0)



