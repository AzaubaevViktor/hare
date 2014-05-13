#include "huff.h"
static char* concatenateStrings(const char * str1, const char * str2)
{
    int lengthStr1 = strlen(str1);
    int i;
    char * result = (char*)calloc((strlen(str1) + strlen(str2) + 1), sizeof(char));

    if (NULL == result)
    {
        return NULL;
    }

    for (i = 0; i < lengthStr1; i++)
    {
        result[i] = str1[i];
    }
    for (i = 0; i < strlen(str2); i++)
    {
        result[i + lengthStr1] = str2[i];
    }

    return result;
}

size_t* createTableFrequencies(FILE *file)
{
    size_t *table = (size_t*)calloc(COUNT_SYMBOLS, sizeof(size_t));
    int symbol;

    while ((symbol = fgetc(file)) != EOF)
    {
       table[symbol]++;
    }

    return table;
}

struct Node* createNode(size_t data, int symbol)
{
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));

    node->data = data;
    node->symbol = symbol;

    node->next = NULL;
    node->left = NULL;
    node->right = NULL;

    return node;
}

void insertNodeInList(struct Node** list, struct Node* node)
{
    struct Node* tmpNode = *list;

    if (node->data <= tmpNode->data)
    {
        node->next = tmpNode;
        (*list) = node;
        return;
    }

    while (tmpNode->next)
    {
        if (node->data <= tmpNode->next->data)
            break;
        tmpNode = tmpNode->next;
    }
    node->next = tmpNode->next;
    tmpNode->next = node;
}

struct Node* createList(size_t* table)
{
    int i;
    struct Node* head = NULL;

    for (i = 0; i < COUNT_SYMBOLS; i++)
    {
        if (table[i])
        {
            if (head)
                insertNodeInList(&head, createNode(table[i], i));
            else
                head = createNode(table[i], i);
        }
    }

    return head;
}

struct Node* createTree(struct Node* list)
{
    if (!list)
        return NULL;

    struct Node* tmpNode = createNode(0,-1);

    tmpNode->next = NULL;
    tmpNode->left = list;
    list = list->next;
    tmpNode->right = list;
    
    if (!tmpNode->right)
        return tmpNode;

    tmpNode->data = tmpNode->left->data + tmpNode->right->data;

    if (!list->next)
        return tmpNode;

    insertNodeInList(&list, tmpNode);
    return createTree(list->next);
}

void createCodes(struct Code* codes, struct Node* node, char* buf)
{
    if (node)
    {
        if (node->symbol > -1)
        {
            strcpy(codes[node->symbol].code, buf);
            codes[node->symbol].size = strlen(buf);
        }
        else
        {
            if (node->left)
                createCodes(codes, node->left, concatenateStrings(buf, "0"));

            if (node->right)
                createCodes(codes, node->right, concatenateStrings(buf, "1"));
        }
    }
}

// delete this shit
void printTree(struct Node* node, int n)
{
    int i;


    if (node->left)
        printTree(node->left, n + 1);

    for (i = 0; i < n; i++)
        printf("-");

    if (node->symbol == -1)
        printf("'%d' - %d\n", node->symbol, node->data);
    else
        printf("'%c' - %d\n", (char)node->symbol, node->data);

    if (node->right)
        printTree(node->right, n + 1);
}

void printList(struct Node* head)
{
    struct Node* tmpNode = head;

    while (tmpNode)
    {
        printf("'%c' - %d\n", (char)tmpNode->symbol, tmpNode->data);
        tmpNode = tmpNode->next;
    }
}

void printTable(size_t* table)
{
    int i;

    for (i = 0; i < COUNT_SYMBOLS; i++)
        if (table[i])
            printf("'%c' - %d\n", (char)i, table[i]);
}

void printCodes(struct Code* codes)
{
    int i;
    for (i = 0; i < COUNT_SYMBOLS; i++)
        if (codes[i].size)
            printf("'%c' - '%s'\n", i, codes[i].code);
}

void coding(struct Code* codes, char* bytesForCoding, int countBytesForCoding, unsigned char* codingBits, int* countCodingBits)
{
    int i, j;
    int tmpCountCodingBits = 0;

    for (i = 0; i < strlen(bytesForCoding); i++)
        codingBits[i] = 0;

    char bits[] = {1, 2, 4, 8, 16, 32, 64, 128};

    for (i = 0; i < strlen(bytesForCoding); i++)
    {
        for (j = 0; j < codes[bytesForCoding[i]].size; j++)
        {
            unsigned char tmp = (codes[bytesForCoding[i]].code[j] == '1' ? bits[7 - (tmpCountCodingBits % 8)] : 0);
            codingBits[tmpCountCodingBits / 8] |= tmp;




//            printf("count coding bits = %d\n\ttmp = %d\n\tres = %d\n", tmpCountCodingBits, tmp, codingBits[tmpCountCodingBits / 8]);

//            if (tmpCountCodingBits && !((tmpCountCodingBits + 1) % 8))
//                printf("\tresult byte = %d\n", codingBits[tmpCountCodingBits / 8]);

            tmpCountCodingBits++;
        }
    }

    *countCodingBits = tmpCountCodingBits;
}

void writeHuffTreeInBuffer(struct Node* node, char* buffer, int* lengthTree, char* countUsedBits)
{
    static char left1[9] = {0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF};
    static char right1[9] = {0, 1, 3, 7, 15, 31, 63, 127, 255};
    static char bits[] = {1, 2, 4, 8, 16, 32, 64, 128};


    if (node)
    {
        if (node->symbol > -1)
        {
            unsigned char ch = node->symbol;

            (*countUsedBits)++;
            if (8 == (*countUsedBits))
            {
                (*lengthTree)++;
                (*countUsedBits) = 0;
            }

            buffer[(*lengthTree)    ] |= ((ch >>     (*countUsedBits)) & right1[8 - (*countUsedBits)]);
            buffer[(*lengthTree) + 1] |= ((ch << 8 - (*countUsedBits)) &  left1[    (*countUsedBits)]);
            (*lengthTree)++;
        }
        else
        {
            buffer[(*lengthTree)] |= bits[7 - *countUsedBits];
            (*countUsedBits)++;
            if (8 == (*countUsedBits))
            {
                (*lengthTree)++;
                (*countUsedBits) = 0;
            }

            if (node->left)
                writeHuffTreeInBuffer(node->left, buffer, lengthTree, countUsedBits);

            if (node->right)
                writeHuffTreeInBuffer(node->right, buffer, lengthTree, countUsedBits);
        }
    }
}
