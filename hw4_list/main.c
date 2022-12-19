//
//  main.c
//  list_assign4
//
//  Created by 201720736 Software 이균 on 2021/10/20.
//  Data Structure 4th programming assignment
//

#include <stdio.h>
#include <stdlib.h>
/* To avoid security error on Visual Studio */
#define _CRT_SECUE_NO_WARNINGS
#define MAX_STACK_SIZE 50
#define MALLOC(p,s) \
if(!((p) = malloc(s))){\
fprintf(stderr, "Insufficient memory"); \
exit(EXIT_FAILURE);\
} // Macro for Dynamic allocation

/*====================================================================*/
/* Structure, Function and variables to use Stack at inverse list data */
typedef struct {
    int stackData;
}element;

element e;
element stack[MAX_STACK_SIZE];

int top = -1;

void stackFull()
{
    fprintf(stderr, "Stack is Full, cannot add element\n");
    exit(EXIT_FAILURE);
}

element stackEmpty()
{
    fprintf(stderr, "Stack is empty\n");
    return stack[top];
}

void push(element item)
{
    if (top >= MAX_STACK_SIZE - 1)
    {
        stackFull();
        return;
    }
    stack[++top] = item;
}

element pop()
{
    if (top == -1)
    {
        return stackEmpty();
    }
    return stack[top--];
}

/*====================================================================*/
/* Make a chained list */
typedef struct listNode* listPointer;
typedef struct listNode{
    int data;
    listPointer link;
};

/*====================================================================*/
/* return node(retNode) if you delete node, and re-use(getNode) that returned node if you add data */
/*====================================================================*/

listPointer avail = NULL; /* a global variable that points to the first node of the free nodes list */

listPointer getNode(void)
{
    listPointer node;
    if(avail){
        node = avail;
        avail = avail->link;
    }
    else
        MALLOC(node, sizeof(*node));
    return node;
}

void retNode(listPointer* node, listPointer prev)
{
    listPointer temp;
    temp = (*node)->link;
    if(avail){
        (*node)->link = avail;
        avail = *node;
    }
    else{
        avail = *node;
        avail->link = NULL;
    }

    if(prev)
        prev->link = temp;
    else
    {
        *node = temp;
    }
}

/*====================================================================*/
/* Function for add, delete, print, print reverse data in the list */
/*====================================================================*/



/* This function add data according to the size */
void addData(listPointer* first, int* nump)
{
    listPointer head = *first;
    listPointer insertPtr = NULL;
    listPointer prev = NULL;
    while(head)    /* Check data and add according to the size */
    {
        if(head->data == *nump)
        {
            printf("중복된 데이터입니다.\n");
            printf("-------\n");
            return;
        }
        if(head->link)
        {
            if( head->data < *nump && head->link->data > *nump)
            {
                insertPtr = head;
                break;
            }
            else if(head->data > *nump)
            {
                insertPtr = prev;
                break;
            }
        }
        else
        {
            if( head->data < *nump)
            {
                insertPtr = head;
            }
            else{
                insertPtr = prev;
            }
            break;
        }
        prev = head;
        head = head->link;
    }
    
    listPointer temp;
    if(avail){
        listPointer ptr = getNode();
        ptr->data = *nump;
        if(insertPtr)
        {
            ptr->link = insertPtr->link;
            insertPtr->link = ptr;
        }
        else
        {
            if(*first){
                ptr->link = *first;
                *first = ptr;
            }
            else{ 
                ptr->link = NULL;
                *first = ptr;
            }
        }
    }
    else{
        MALLOC(temp, sizeof(*temp));
        temp->data = *nump;
        if(insertPtr)
        {
            temp->link = insertPtr->link;
            insertPtr->link = temp;
        }
        else // 앞에 삽입
        {
            if(*first){
                temp->link = *first;
                *first = temp;
            }
            else{
                temp->link = NULL;
                *first = temp;
            }
        }
    }
    printf("성공했습니다.\n");
    printf("-------\n");
}


void delete(listPointer* first, int* nump)
{
    listPointer temp = *first;
    listPointer prev = *first;
    while(1){
        if(temp->data == *nump)
            break;
        else if(temp->link == NULL)
        {
            printf("해당하는 데이터가 없습니다.\n");
            printf("-------\n");
            return;
        }
        prev = temp;
        temp = temp->link;
    }
    
//  * if you don't use getNode, retNode to manage useless data, use this code.
   /* if(*first != temp){
        prev->link = temp->link;
    }
    else{
        *first = (*first)->link;
    }
    free(temp); */
    
    if(*first != temp)
        retNode(&temp, prev);
    else
        retNode(first, NULL);
    printf("성공했습니다.\n");
    printf("-------\n");

}

void listPrint(listPointer* first)
{
    listPointer temp = *first;
    if(temp == NULL)
    {
        printf("List가 비어있습니다.\n");
        printf("-------\n");
        return;
    }
    while(1){
        printf("%d ", temp->data);
        if(temp->link == NULL)
        {
            printf("\n");
            printf("-------\n");
            break;
        }
        temp = temp->link;
    }
}

/*====================================================================*/
/* The code in the textbook and this code can also use for inverse the list */
//listPointer invert2(listPointer lead)
//{
//    listPointer middle, trail;
//    middle = NULL;
//    while(lead){
//        trail = middle;
//        middle = lead;
//        lead = lead->link;
//        middle->link = trail;
//    }
//    return middle;
//}

/*====================================================================*/
/* push list in stack and pop to print the inversed list */
void invert(listPointer lead)
{
    while(1)
    {
        if(!(lead)){
            printf("List가 비어있습니다.\n");
            printf("-------\n");
            return;
        }
        e.stackData = lead->data;
        push(e);
        lead = lead->link;
        if(lead == NULL)
            break;
    }
    while(top != -1){
        e = pop();
        printf("%d ", e.stackData);
    }
    printf("\n");
    printf("-------\n");
}


/*====================================================================*/
/* The main function of this program */
int main(void) {
    char command;
    int addNum, delNum;
    listPointer start, invertStart; // invertStart2;
    start = invertStart = NULL; //invertStart2 = NULL;
    while(1){
        printf("* 명령어를 입력하세요(a: 데이터 추가, d: 데이터 삭제, p: 정순 출력, r: 역순 출력, q: 종료)\n");
        scanf("%c", &command);
        if(command == 'q'){
            printf("프로그램을 종료합니다. 감사합니다.\n");
            break;
        }
        else if(command == 'a')
        {
            printf("추가할 데이터를 입력하세요: ");
            scanf("%d", &addNum);
            addData(&start, &addNum);
        }
        else if(command == 'd')
        {
            printf("삭제할 데이터를 입력하세요: ");
            scanf("%d", &delNum);
            delete(&start, &delNum);
        }
        else if(command == 'p')
        {
            listPrint(&start);
        }
        else if(command == 'r')
        {
            /* listPointer invertStart = invert2(start);
            list_print(&invertStart);
            listPointer invertStart2 = invert2(invertStart);
            start = invertStart2; */
            // code for if you use invert2() instead of invert().
            invert(start);
        }
        else
        {
            printf("이해할 수 없는 명령어입니다.\n");
        }
        fflush(stdin);  // clear buffer
    }
    return 0;
}
