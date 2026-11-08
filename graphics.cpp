static ErrorCode formPicture(BinaryTree_t* binaryTree);
static void recordGraphicBlock(Node* node, FILE* file);
static void transliterate(const char* source, char* destination);
static const char* transliterateChar(unsigned char symbol);

ErrorCode createDebugImage(BinaryTree_t* binaryTree){

    assert(binaryTree);

    ErrorCode error = formPicture(binaryTree);
    if (error != ERR_OK) return error;

    system("dot graph.txt -Tpng -o binary-tree-picture.png");

    deletingResultFile("graph.txt");

    return ERR_OK;

}

static ErrorCode formPicture(BinaryTree_t* binaryTree){

    assert(binaryTree);

    const char fileName[] = "graph.txt";

    FILE* graphicCodeFile = fopen(fileName, "w");
    if (graphicCodeFile == NULL) {
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo, __func__, __LINE__, "File-problem", ERR_FILE_NOT_FOUND);
        printErrorIntoConsole(&debugLogInfo);
        printf("Cannot open graphic code file: %s\n", fileName);

        return ERR_FILE_NOT_FOUND;
    }

    fprintf(graphicCodeFile,
        "digraph BinaryTree {\n"
        "graph ["
        "bgcolor=\"#F0F7FF\", "
        "rankdir=TB, "
        "splines=polyline, "
        "nodesep=0.35, "
        "ranksep=0.65, "
        "pad=0.3, "
        "];\n"

        "node ["
        "shape=Mrecord, "
        "style=filled, "
        "fillcolor=\"#FFC0FF\", "
        "color=\"#481569\", "
        "fontcolor=\"#481569\", "
        "fontname=\"Arial\", "
        "fontsize=16, "
        "margin=\"0.18,0.10\""
        "];\n");

    recordGraphicBlock(binaryTree->root, graphicCodeFile);

    fprintf(graphicCodeFile, "\n}\n");

    FCLOSE(graphicCodeFile);

    return ERR_OK;
}

static void recordGraphicBlock(Node* node, FILE* file){

    assert(file);

    if (node == NULL) return;

    char transliteratedData[MAX_PROGRAM_CAPACITY] = "";
    transliterate(node->data, transliteratedData);

    fprintf(file,
            "node%p [label = \"" PRINT_NODE_FORMAT "\";]\n",
            (const void*)node,
            transliteratedData);

    if(node->left != NULL){
        fprintf(file,
            "\tnode%p -> node%p "
            "[color=\"#1947bd\"];\n",
            (const void*)node,
            (const void*)node->left);
    }

    if(node->right != NULL){
        fprintf(file,
            "\tnode%p -> node%p "
            "[color=\"#1947bd\"];\n",
            (const void*)node,
            (const void*)node->right);
    }

    recordGraphicBlock(node->left, file);
    recordGraphicBlock(node->right, file);

}

static void transliterate(const char* source, char* destination) {

    assert(source);
    assert(destination);

    size_t i = 0; //pos in source
    size_t j = 0; //pos in destination

    while (source[i] != '\0') {

        unsigned char symbol = (unsigned char)source[i];

        const char* latin = transliterateChar(symbol);

        if (latin != NULL) {

            size_t length = strlen(latin);

            if (j + length >= MAX_PROGRAM_CAPACITY) break;

            for (size_t k = 0; k < length; k++) {
                destination[j++] = latin[k];
            }
        }
        else if (symbol < 128) { // if symbol not Russian

            destination[j++] = (char)symbol;
        }

        i++;
    }

    destination[j] = '\0';
}

static const char* transliterateChar(unsigned char symbol){

    switch (symbol) {

        case 0xC0: case 0xE0: return "a";     // А а
        case 0xC1: case 0xE1: return "b";     // Б б
        case 0xC2: case 0xE2: return "v";     // В в
        case 0xC3: case 0xE3: return "g";     // Г г
        case 0xC4: case 0xE4: return "d";     // Д д
        case 0xC5: case 0xE5: return "e";     // Е е
        case 0xA8: case 0xB8: return "yo";    // Ё ё
        case 0xC6: case 0xE6: return "zh";    // Ж ж
        case 0xC7: case 0xE7: return "z";     // З з
        case 0xC8: case 0xE8: return "i";     // И и
        case 0xC9: case 0xE9: return "y";     // Й й
        case 0xCA: case 0xEA: return "k";     // К к
        case 0xCB: case 0xEB: return "l";     // Л л
        case 0xCC: case 0xEC: return "m";     // М м
        case 0xCD: case 0xED: return "n";     // Н н
        case 0xCE: case 0xEE: return "o";     // О о
        case 0xCF: case 0xEF: return "p";     // П п
        case 0xD0: case 0xF0: return "r";     // Р р
        case 0xD1: case 0xF1: return "s";     // С с
        case 0xD2: case 0xF2: return "t";     // Т т
        case 0xD3: case 0xF3: return "u";     // У у
        case 0xD4: case 0xF4: return "f";     // Ф ф
        case 0xD5: case 0xF5: return "kh";    // Х х
        case 0xD6: case 0xF6: return "ts";    // Ц ц
        case 0xD7: case 0xF7: return "ch";    // Ч ч
        case 0xD8: case 0xF8: return "sh";    // Ш ш
        case 0xD9: case 0xF9: return "shch";  // Щ щ
        case 0xDA: case 0xFA: return "";      // Ъ ъ
        case 0xDB: case 0xFB: return "y";     // Ы ы
        case 0xDC: case 0xFC: return "";      // Ь ь
        case 0xDD: case 0xFD: return "e";     // Э э
        case 0xDE: case 0xFE: return "yu";    // Ю ю
        case 0xDF: case 0xFF: return "ya";    // Я я

        default: return NULL;
    }
}
