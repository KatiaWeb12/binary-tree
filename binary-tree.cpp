static ErrorCode createTreeNode(BinaryTree_t* binaryTree,
                                Node** currentNode,
                                NodeElem_t value,
                                Node** newNode);
static ErrorCode allocateMemoryForNode(Node** treeNode);
static ErrorCode formBinaryTree(BinaryTree_t* binaryTree, char* treeText);
static ErrorCode writeTreeNode(FILE* file, const Node* node);
void destroyTree(BinaryTree_t* binaryTree);
static ErrorCode afterOpenedBracket(BinaryTree_t* binaryTree, char** current, Node** currentNode);
static ErrorCode verifyNode(const Node* node,
                       const Node* parent,
                       const BinaryTree_t* binaryTree,
                       size_t* count,
                       const char* function,
                       int line);


void initBinaryTree(BinaryTree_t* binaryTree){

    assert(binaryTree);

    binaryTree->root = NULL;
    binaryTree->dim = 0;
}

ErrorCode readBinaryTree(const char* fileName, BinaryTree_t* binaryTree){

    assert(fileName);

    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo, __func__, __LINE__, "File-problem", ERR_FILE_NOT_FOUND);
        printErrorIntoConsole(&debugLogInfo);
        printf("Cannot open file: %s\n", fileName);

        return ERR_FILE_NOT_FOUND;
    }

    char treeText[MAX_PROGRAM_CAPACITY] = "";

    size_t textRead = fread(treeText, 1, MAX_PROGRAM_CAPACITY, file);
    treeText[textRead] = '\0';

    FCLOSE(file);

    return formBinaryTree(binaryTree, treeText);

}

static ErrorCode formBinaryTree(BinaryTree_t* binaryTree, char* treeText){

    assert(binaryTree);
    assert(treeText);

    Node* currentNode = NULL;

    char* current = treeText;

    while(*current != '\0'){

        if (isspace((unsigned char)*current)) {

            current++;
            continue;
        }
        else if(*current == '('){

            afterOpenedBracket(binaryTree, &current, &currentNode);
        }

        else if(*current == ')'){

            if(currentNode == NULL){
                error_debug debugLogInfo = {};
                ERROR_STRUCT_FORMAT(&debugLogInfo,
                                    __func__,
                                    __LINE__,
                                "Incorrect input-data. Invalid brackets sequence. Extra ')'",
                                ERR_INVALID_ARGUMENT);
                printErrorIntoConsole(&debugLogInfo);

                return ERR_INVALID_ARGUMENT;
            }

            currentNode = currentNode->parent;
            current++;

        }
        else{
            error_debug debugLogInfo = {};
            ERROR_STRUCT_FORMAT(&debugLogInfo,
                                __func__,
                                __LINE__,
                            "Incorrect input-data. Invalid symbol in file.",
                            ERR_INVALID_ARGUMENT);
            printErrorIntoConsole(&debugLogInfo);

            return ERR_INVALID_ARGUMENT;
        }

    }

    if(currentNode != NULL || binaryTree->root == NULL){
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo,
                            __func__,
                            __LINE__,
                            "Incorrect input-data. Invalid brackets sequence.",
                            ERR_INVALID_ARGUMENT);
        printErrorIntoConsole(&debugLogInfo);

        return ERR_INVALID_ARGUMENT;
    }

    return ERR_OK;
}

static ErrorCode afterOpenedBracket(BinaryTree_t* binaryTree, char** current, Node** currentNode){

    assert(binaryTree);
    assert(current);
    assert(currentNode);

    (*current)++;

    if (**current != '"'){
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo,
                            __func__,
                            __LINE__,
                            "Incorrect input-data",
                            ERR_INVALID_ARGUMENT);
        printErrorIntoConsole(&debugLogInfo);

        return ERR_INVALID_ARGUMENT;;
    }

    (*current)++;

    char* start = *current;

    while (**current != '"' && **current != '\0') (*current)++;

    if (**current != '"') {

        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo,
                            __func__,
                            __LINE__,
                            "Incorrect input-data",
                            ERR_INVALID_ARGUMENT);
        printErrorIntoConsole(&debugLogInfo);

        return ERR_INVALID_ARGUMENT;
    }

    size_t length = *current - start;

    char* value = (char*)calloc(length + 1, sizeof(char));
    if (value == NULL) {

        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo,
                            __func__,
                            __LINE__,
                            "Incorrect input-data",
                            ERR_OUT_OF_MEMORY);
        printErrorIntoConsole(&debugLogInfo);

        return ERR_OUT_OF_MEMORY;
    }

    for (size_t i = 0; i < length; i++) {
        value[i] = start[i];
    }

    (*current)++;

    Node* newNode = NULL;
    ErrorCode error = createTreeNode(binaryTree,
                                    currentNode,
                                    value,
                                    &newNode);

    if (error != ERR_OK) {

        free(value);
        return error;
    }

    binaryTree->dim++;

    return ERR_OK;

}

static ErrorCode createTreeNode(BinaryTree_t* binaryTree,
                                Node** currentNode,
                                NodeElem_t value,
                                Node** newNode){

    assert(currentNode);
    assert(binaryTree);

    ErrorCode error = allocateMemoryForNode(newNode);
    if(error != ERR_OK) return error;

    (*newNode)->data = value;

    if(*currentNode == NULL) binaryTree->root = *newNode;

    else{
        (*newNode)->parent = *currentNode;

        if((*newNode)->parent->left == NULL){

            (*newNode)->parent->left = *newNode;
        }
        else if((*newNode)->parent->right == NULL){
            (*newNode)->parent->right = *newNode;
        }

    }

    *currentNode = *newNode;

    return ERR_OK;

}

static ErrorCode allocateMemoryForNode(Node** treeNode){

    Node* memory = (Node*)calloc(1, sizeof(Node));
    if (memory == NULL){
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo, __func__, __LINE__, "ERR_OUT_OF_MEMORY", ERR_OUT_OF_MEMORY);
        printErrorIntoConsole(&debugLogInfo);
        return ERR_OUT_OF_MEMORY;
    }

    *treeNode = memory;

    return ERR_OK;
}

ErrorCode writeBinaryTree(BinaryTree_t* binaryTree, const char* fileName){

    assert(fileName);

    VERIFY_OK(binaryTree, __func__);

    FILE* file = fopen(fileName, "w");
    if (file == NULL) {
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo, __func__, __LINE__, "File-problem", ERR_FILE_NOT_FOUND);
        printErrorIntoConsole(&debugLogInfo);
        printf("Cannot open file: %s\n", fileName);

        return ERR_FILE_NOT_FOUND;
    }

    ErrorCode error  = writeTreeNode(file, binaryTree->root);
    if(error != ERR_OK) return error;

    FCLOSE(file);

    return ERR_OK;

}

static ErrorCode writeTreeNode(FILE* file, const Node* node) {

    assert(file);
    assert(node);

    fprintf(file, "(\"" PRINT_NODE_FORMAT "\"", node->data);

    if (node->left != NULL) {

        fprintf(file, " ");

        ErrorCode error = writeTreeNode(file, node->left);
        if (error != ERR_OK) return error;
    }

    if (node->right != NULL) {

        fprintf(file, " ");

        ErrorCode error = writeTreeNode(file, node->right);
        if (error != ERR_OK) return error;
    }

    fprintf(file, ")");

    return ERR_OK;
}

void destroyBinaryTree(BinaryTree_t* binaryTree) {

    assert(binaryTree);

    VERIFY_OK(binaryTree, __func__);

    destroyTreeNode(binaryTree->root);

    binaryTree->root = NULL;
    binaryTree->dim = 0;
}

void destroyTreeNode(Node* node) {

    if (node == NULL) return;

    destroyTreeNode(node->left);
    destroyTreeNode(node->right);

    #ifdef AKINATOR
        free(node->data);
    #endif
    free(node);
}

ErrorCode verifyBinaryTree(const BinaryTree_t* binaryTree, const char* function, int line){

    if (binaryTree == NULL){
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo, function, line, "NULL binaryTree", ERR_INVALID_ARGUMENT);
        printErrorIntoConsole(&debugLogInfo);

        return ERR_INVALID_ARGUMENT;
    }

    if (binaryTree->root == NULL) {
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo, function, line, "NULL binaryTree->root", ERR_INVALID_ARGUMENT);
        printErrorIntoConsole(&debugLogInfo);

        return ERR_INVALID_ARGUMENT;
    }

    if (binaryTree->dim == 0) {
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo, function, line, "DIM = 0", ERR_INVALID_ARGUMENT);
        printErrorIntoConsole(&debugLogInfo);

        return ERR_INVALID_ARGUMENT;
    }

    if (binaryTree->root->parent != NULL){
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo, function, line, "INVALID root parent", ERR_INVALID_ARGUMENT);
        printErrorIntoConsole(&debugLogInfo);

        return ERR_INVALID_ARGUMENT;
    };

    size_t count = 0;

    return verifyNode(binaryTree->root, NULL, binaryTree, &count, function, line);
}

static ErrorCode verifyNode(const Node* node,
                       const Node* parent,
                       const BinaryTree_t* binaryTree,
                       size_t* count,
                       const char* function,
                       int line){

    if (node == NULL) return ERR_OK;

    if (*count >= binaryTree->dim){
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo, function, line, "INVALID DIM", ERR_INVALID_ARGUMENT);
        printErrorIntoConsole(&debugLogInfo);

        return ERR_INVALID_ARGUMENT;
    };

    (*count)++;

    if (node->parent != parent){
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo, function, line, "NULL node", ERR_INVALID_ARGUMENT);
        printErrorIntoConsole(&debugLogInfo);

        return ERR_INVALID_ARGUMENT;
    };

    if (verifyNode(node->left, node, binaryTree, count, function, line) != ERR_OK){
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo, function, line, "Invalid node", ERR_INVALID_ARGUMENT);
        printErrorIntoConsole(&debugLogInfo);

        return ERR_INVALID_ARGUMENT;
    }

    if (verifyNode(node->right, node, binaryTree, count, function, line) != ERR_OK){
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo, function, line, "Invalid node", ERR_INVALID_ARGUMENT);
        printErrorIntoConsole(&debugLogInfo);

        return ERR_INVALID_ARGUMENT;
    }

    return ERR_OK;
}
