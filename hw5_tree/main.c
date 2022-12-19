//
//  main.c
//  Tree
//
//  Created by 이균 on 2021/11/02.
//

/* To avoid security error on Visual Studio */
#define _CRT_SECURE_NO_WARNINGS
#define MAX_STRING_SIZE 20
#define TRUE 1
#define FALSE 0
#define ERROR_OCCUR -1
#define NOT_ERROR_OCCUR 0
#include <stdio.h>
#include <stdlib.h>

typedef struct node *treePointer;
typedef struct node{
    char data;
    int value;
    treePointer leftChild, rightChild;
}Node;

char* exptr; // expression pointer
int error = NOT_ERROR_OCCUR;

// A utility function to check if 'c' is an operator
int isOperator(char c)
{
    if (c == '+' || c == '-' ||
            c == '*' || c == '/')
        return TRUE;
    return FALSE;
}

treePointer createNode(char data)
{
    treePointer ptr = (treePointer)malloc(sizeof(Node));
    ptr->data = data;
    ptr->leftChild = NULL;
    ptr->rightChild = NULL;
    ptr->value = 0;
    return ptr;
}

treePointer createTree(char** expr)
{
    char oneCharacter[2] = {0, };
    exptr = *expr;
    treePointer ptr = createNode(*exptr);
    if(isOperator(*exptr))
    {
        exptr += 1;
        ptr->leftChild = createTree(&exptr);
        exptr += 1;
        ptr->rightChild = createTree(&exptr);
    }
    else if(48 <= *exptr && *exptr <= 57){ // Not Operator -> expression is a digit-> 0~9
        oneCharacter[0] = *exptr;
        ptr->value = atoi(oneCharacter); // change character to integer
    }
    else // invalid input
    {
        error = ERROR_OCCUR;
        return NULL;
    }
    return ptr;
}

void postOrderEval(treePointer node)
{
    if(node)
    {
        postOrderEval(node->leftChild);
        postOrderEval(node->rightChild);
        switch (node->data) {
            case '+':
                node->value = (node->leftChild->value) + (node->rightChild->value);
                break;
            case '-':
                node->value = (node->leftChild->value) - (node->rightChild->value);
                break;
            case '*':
                node->value = (node->leftChild->value) * (node->rightChild->value);
                break;
            case '/':
                node->value = (node->leftChild->value) / (node->rightChild->value);
                break;
            default:
                break;
        }
    }
}


int main() {
    
    char **str;
    treePointer ptr;
    *str = malloc(sizeof(char)* MAX_STRING_SIZE);

    while(1)
    {
        printf("수식을 입력하시오(종료시 q를 입력하시오): \n");
        scanf("%s", *str);
        if(*str[0] == 'q')
            break;
        ptr = createTree(str);
        if(error == ERROR_OCCUR)
        {
            printf("잘못된 문자를 사용하였습니다\n");
            printf("----------------------------------\n");
            error = NOT_ERROR_OCCUR;
            continue;
        }
        postOrderEval(ptr);
        printf("계산결과는 %d입니다\n", ptr->value);
        printf("----------------------------------\n");
    }

    free(*str);
    return 0;
}



//int main() {
//    while(1)
//    {
//        printf("수식을 입력하시오(종료시 q를 입력하시오): \n");
//        scanf("%s", str);
//        if(str[0] == 'q')
//            break;
//        createTree(&str);
//    }
//    return 0;
//}



//treePointer createNode(char data)
//{
//    treePointer ptr = (treePointer)malloc(sizeof(Node));
//    ptr->data = data;
//    ptr->leftChild = NULL;
//    ptr->rightChild = NULL;
//    printf("making node = %c\n", data);
//    return ptr;
//}
//
//treePointer createTree(char** expr)
//{
//
//    exptr = *expr;
//    treePointer ptr = createNode(*exptr);
//
//    if(isOperator(*exptr))
//    {
//        exptr += 1;
//        printf("left child = %c\n", *exptr);
//        ptr->leftChild = createTree(&exptr);
//
//        exptr += 1;
//        printf("right child = %c\n", *exptr);
//        ptr->rightChild = createTree(&exptr);
//
//    }
//    else{ // Operator가 아닌 경우
//        //exptr += 1;
//        printf("leaf node = %c\n", *exptr);
//    }
//    return ptr;
//}
