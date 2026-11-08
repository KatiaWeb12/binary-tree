typedef char* NodeElem_t;
#define PRINT_NODE_FORMAT "%s"
#define AKINATOR

#define TX_USE_//speak
#include <TXLib.h>
#include <windows.h>

#include "binary-tree.h"
#include "binary-tree.cpp"
#include "graphics.cpp"

int main() {

    setlocale(LC_ALL, "");

    BinaryTree_t binaryTree = {};
    initBinaryTree(&binaryTree);

    ErrorCode error = readBinaryTree(akinatorFileName, &binaryTree);
    if(error != ERR_OK) return error;

    playAkinator(&binaryTree);

    error = changeFilesComplication(&binaryTree);
    if(error != ERR_OK) return error;

    destroyBinaryTree(&binaryTree);

    return 0;
}

bool getAnswer(){

    char answer[MAX_ANSWER_LENGTH] = "";

    while(true){

        printf("Напиши да или нет: ");
        //speak("Напиши да или нет: ");

        readLine(answer, MAX_ANSWER_LENGTH);

        if(strcmp(answer, "да") == 0) return true;
        else if(strcmp(answer, "нет") == 0) return false;
        else{
            printf("Неверный ответ\n");
            //speak("Неверный ответ\n");
        }
    }
}

ErrorCode readLine(char* buffer, size_t capacity){

    assert(buffer);
    assert(capacity > 0);

    if(fgets(buffer, (int)capacity, stdin) == NULL){
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo,
                            __func__,
                            __LINE__,
                            "Incorrect input-data",
                            ERR_INPUT_OUTPUT);
        printErrorIntoConsole(&debugLogInfo);

        return ERR_INPUT_OUTPUT;
    }

    size_t length = strlen(buffer);

    if (length > 0 && buffer[length - 1] == '\n') buffer[length - 1] = '\0';

    toLowerCase(buffer);

    return ERR_OK;
}

ErrorCode playAkinator(BinaryTree_t* binaryTree){

    assert(binaryTree);

    VERIFY_OK(binaryTree, __func__);

    ErrorCode error = ERR_OK;

    int chosenMenuPoint = getMenuPoint();

    while(chosenMenuPoint != EXIT_GAME){

        switch (chosenMenuPoint){

            case GUESS: {
                error = guessObject(binaryTree);
                if(error != ERR_OK) return error;
                break;
            }
            case WATCH: {
                createDebugImage(binaryTree);
                break;
            }
            case IDENTIFY: {
                identifyObject(binaryTree);
                break;
            }
            case COMPARISON: {
                comparisonObjects(binaryTree);
                break;
            }
            default: {
                printf("Некорректный пункт.\n");

                break;
            }
        }

        chosenMenuPoint = getMenuPoint();
    };

    printf("Игра окончена.\n");
    //speak("Игра окончена.\n");

    return ERR_OK;
}

int getMenuPoint(){

    printf("\nВыбери действие:\n"
            "1. Угадать объект\n"
            "2. Посмотреть общее дерево\n"
            "3. Получить определение объекта\n"
            "4. Сравнить объекты\n"
            "5. Закончить игру\n\n");
    // speak("\nВыбери действие:\n"
    //         "1. Угадать объект\n"
    //         "2. Посмотреть общее дерево\n"
    //         "3. Получить определение объекта\n"
    //         "4. Сравнить объекты\n"
    //         "5. Закончить игру\n\n");

    printf("Введи номер выбранного действия: ");
    //speak("Введи номер выбранного действия: ");

    int chosenMenuPoint = 0;
    scanf("%d", &chosenMenuPoint);

    int symbol = 0;
    while ((symbol = getchar()) != '\n' && symbol != EOF) {};

    return chosenMenuPoint;
}

ErrorCode guessObject(BinaryTree_t* binaryTree){

    assert(binaryTree);

    if(binaryTree->root == NULL) addRootEl(binaryTree);

    Node* current= binaryTree->root;

    while(!(current->left == NULL && current->right == NULL)){

        printf("Этот объект %s?\n", current->data);
        //speak("Этот объект ");
        //speak(current->data);

        bool answer = getAnswer();

        current = answer ? current->right : current->left;
    }

    printf("\nТы загадал %s?\n", current->data);
    //speak("Ты загадал %s?");

    if(getAnswer()){

        printf("Успех! Я отгадал!\n");
        //speak("Успех! Я отгадал!");
        return ERR_OK;
    }
    else{

        printf("Похоже, что я не знаю этот объект.\n");
        //speak("Похоже, что я не знаю этот объект.");
        return getNewObject(binaryTree, current);
    }

    return ERR_OK;
}

void identifyObject(BinaryTree_t* binaryTree){

    assert(binaryTree);

    printf("Введите объект: ");

    char value[50] = "";
    scanf("%s", value);

    bool result = seachValue(binaryTree->root, value);
    if(!result) printf("Объекта '%s' не существует в нашей базе.\n", value);
}

bool seachValue(Node* node, NodeElem_t value){

    if (node == NULL) return false;

    if(node->data != NULL && strcmp(node->data, value) == 0){

        printIdentify(node, value);
        return true;
    }

    if (seachValue(node->left, value)) {
        return true;
    }

    if (seachValue(node->right, value)) {
        return true;
    }

    return false;

}

void printIdentify(Node* node, const char* value){

    assert(node);

    printf("\nОпределение '%s':\n| ", value);
    //speak("Определение");
    //speak(value);

    printPathFromRoot(node);

    printf("\n");
}

void printPathFromRoot(Node* node){

    if (node == NULL) return;

    if (node->parent != NULL){

        printPathFromRoot(node->parent);

        if(node->parent->left == node){
            printf("НЕ %s | ", node->parent->data);
        }
        else{
            printf("%s | ", node->parent->data);
        }
    }
}

void comparisonObjects(BinaryTree_t* binaryTree){

    assert(binaryTree);

    char object1[MAX_ANSWER_LENGTH] = "";
    char object2[MAX_ANSWER_LENGTH] = "";

    printf("Введи первый объект: ");
    if (readLine(object1, sizeof(object1)) != ERR_OK) {
        return;
    }

    printf("Введи второй объект: ");
    if (readLine(object2, sizeof(object2)) != ERR_OK) {
        return;
    }

    Node* node1 = findObject(binaryTree->root, object1);
    Node* node2 = findObject(binaryTree->root, object2);

    if (node1 == NULL) {
        printf("Объект '%s' не найден!\n", object1);
        return;
    }

    if (node2 == NULL) {
        printf("Объект '%s' не найден!\n", object2);
        return;
    }

    if (node1 == node2) {
        printf("Выбран один и тот же объект!\n");
        return;
    }

    Node* commonAncestor = findCommonAncestor(node1, node2);

    printf("\nОбщие признаки:\n");
    printCommonConditions(commonAncestor);
    printf("|\n");

    printf("\nНо '%s':\n", object1);
    printDifferentConditions(node1, commonAncestor);
    printf("|\n");

    printf("\nА '%s':\n", object2);
    printDifferentConditions(node2, commonAncestor);
    printf("|\n");

    printf("\n\nСравнение завершено.\n");
}

static Node* findObject(Node* node, const char* value){

    if (node == NULL) return NULL;

    if (strcmp(node->data, value) == 0) return node;

    Node* found = findObject(node->left, value);
    if (found != NULL) return found;

    return findObject(node->right, value);
}

Node* findCommonAncestor(Node* node1, Node* node2){

    Node* current = node1;

    while (current != NULL) {

        if (isAncestor(current, node2)) return current;

        current = current->parent;
    }

    return NULL;
}

bool isAncestor(Node* ancestor, Node* node){

    while (node != NULL) {

        if (node == ancestor) return true;

        node = node->parent;
    }

    return false;
}

void printCommonConditions(Node* node){

    if (node == NULL || node->parent == NULL) return;

    printCommonConditions(node->parent);

    printCondition(node);
}

void printDifferentConditions(Node* node, Node* ancestor){

    if (node == ancestor) return;

    printDifferentConditions(node->parent, ancestor);

    printCondition(node);
}

void printCondition(Node* node){

    assert(node);
    assert(node->parent);

    if (node->parent->left == node) {
        printf("| НЕ %s ", node->parent->data);
    }
    else {
        printf("| %s ", node->parent->data);
    }
}

void addRootEl(BinaryTree_t* binaryTree){

    assert(binaryTree);

    binaryTree->root = createNewAkinatorNode("Неизвестный объект", NULL);
    binaryTree->dim = 1;

}

Node* createNewAkinatorNode(const char* text, Node* parent){

    assert(text);

    Node* newNode = NULL;

    if (allocateMemoryForNode(&newNode) != ERR_OK) return NULL;

    newNode->data = (char*)calloc(strlen(text) + 1, sizeof(char));

    if (newNode->data == NULL) {

        free(newNode);
        return NULL;
    }

    strcpy(newNode->data, text);

    newNode->parent = parent;

    return newNode;
}

ErrorCode getNewObject(BinaryTree_t* binaryTree, Node* node){

    assert(binaryTree);
    assert(node);

    char newValue[MAX_ANSWER_LENGTH] = "";
    char question[MAX_ANSWER_LENGTH] = "";

    printf("Что ты загадал?\n");
    //speak("Что ты загадал?");
    readLine(newValue, sizeof(newValue));

    printf("Чем %s отличается от %s? Он/она ...?\n", newValue, node->data);
    //speak("Чем");
    //speak(newValue);
    //speak("отличается от");
    //speak(node->data);
    readLine(question, sizeof(question));

    Node* newNode = createNewAkinatorNode(newValue, node);
    if(newNode == NULL){
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo,
                            __func__,
                            __LINE__,
                            "Incorrect input-data",
                            ERR_INVALID_DATA);
        printErrorIntoConsole(&debugLogInfo);

        return ERR_INVALID_DATA;
    }

    Node* lastNode = createNewAkinatorNode(node->data, node);
    if(lastNode == NULL){
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo,
                            __func__,
                            __LINE__,
                            "Incorrect input-data",
                            ERR_INVALID_DATA);
        printErrorIntoConsole(&debugLogInfo);

        return ERR_INVALID_DATA;
    }

    char* newQuestion = (char*)calloc(strlen(question) + 1, sizeof(char));
    if (newQuestion == NULL) {

        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo,
                            __func__,
                            __LINE__,
                            "Incorrect data",
                            ERR_INVALID_DATA);
        printErrorIntoConsole(&debugLogInfo);

        return ERR_INVALID_DATA;
    }

    strcpy(newQuestion, question);

    printf("Хотите ли вы сохранять изменения?\n");

    if(getAnswer()){

        node->data = newQuestion;
        node->right = newNode;
        node->left = lastNode;

        binaryTree->dim += 2;

        printf("В систему был добавлен новый объект.\n");
        //speak("В систему был добавлен новый объект.");
    }

    return ERR_OK;

}

ErrorCode changeFilesComplication(BinaryTree_t* binaryTree){

    assert(binaryTree);

    system("del /Q last-akinator-tree.txt");
    system("ren akinator-tree.txt last-akinator-tree.txt");

    return writeBinaryTree(binaryTree, akinatorFileName);

}

void speak(const char* text){

    assert(text);

    int result = txSpeak("%s", text);

    if (result == -1) {
        printf("Speech error!\n");
    }
}

void toLowerCase(char* str){

    assert(str);

    CharLowerBuffA(str, (DWORD)strlen(str));
}
