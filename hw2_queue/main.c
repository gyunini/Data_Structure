//
//  main.c
//  자구 과제2
//
//  Created by 이균 on 2022/02/18.
//
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define Max_Queue_Size 6
#define Max_Name_Length 20
#define Hospital_Arrived 1
#define Put_Waitlist 1
#define Medical_Care_Clear 2
#define Cancel_Process 2
#define Terminate_Service 3
#define TRUE 1
#define FALSE 0

typedef struct {
    char name[Max_Name_Length];
}element;

element queue[Max_Queue_Size] = {0};

int rear = 0;
int front = 0;
int qcnt = 0;
int num, one;
int queueclear = 0;
element e;

void queueFull()
{
    if (rear != 0)
    {
        rear -= 1;
    }
    else
        rear = Max_Queue_Size - 1;

    printf("큐가 꽉 찼습니다.\n");
}

element queueEmpty()
{
    printf("대기중인 환자가 없습니다.\n");
    printf("감사합니다.\n");
    printf("-------------\n");
    return queue[rear];
}

void addq(element item)
{
    rear = (rear + 1) % Max_Queue_Size;
    if (front == rear)
        queueFull();
    queue[rear] = item;
    qcnt++;
    if (queueclear)
        queueclear = FALSE;
}

element deleteq()
{
    if (front == rear)
    {
        queueclear = FALSE;
        return queueEmpty();
    }
    front = (front + 1) % Max_Queue_Size;
    qcnt--;
    if (qcnt == 0)
        queueclear = TRUE;
    return queue[front];
}

void arrived()
{
    if (qcnt == Max_Queue_Size - 1)
    {
        printf("새 진료 접수가 불가능 합니다.\n");
        printf("감사합니다.\n");
        printf("----------\n");
    }
    else
    {
        printf("현재 대기자수는 %d명입니다. 다음에서 원하는 기능의 번호를 선택해 주세요. 1. 대기자 등록 2. 취소\n", qcnt);// 큐의 개수
        scanf("%d", &one);
        if (one == Put_Waitlist)
        {
            printf("대기자의 이름을 등록해 주세요: ");
            scanf("%s", e.name);
            addq(e);
            printf("감사합니다.\n");
            printf("----------\n");
        }
        else if (one == Cancel_Process) // 취소
        {
            printf("취소하셨습니다. 다시 서비스를 선택해주세요.\n");
            printf("----------\n");
        }
        else
        {
            printf("번호를 잘못 입력하셨습니다.\n");
            printf("------------\n");
        }
    }
}

void treatment_clear()
{
    e = deleteq();
    if (qcnt != 0)
    {
        printf("%s 님이 진료 대기입니다.\n", queue[(front +1) % Max_Queue_Size].name);
        printf("감사합니다.\n");
        printf("-------------\n");
    }
    else if(queueclear)
    {
        return queueEmpty();
    }
}

void service()
{
    while (1)
    {
        printf("\n\n\n진료관리 서비스에 오신것을 환영합니다. 원하는 기능을 선택해 주세요. 1. 병원도착 2. 진료완료 3. 서비스 종료\n");
        scanf("%d", &num);
        if (num == Hospital_Arrived)
            arrived();

        else if (num == Medical_Care_Clear)
            treatment_clear();

        else if (num == Terminate_Service)
        {
            printf("서비스를 종료합니다.\n");
            printf("감사합니다.\n");
            break;
        }
        else
        {
            printf("번호를 잘못 입력하셨습니다.\n");
            printf("------------\n");
        }
    }
}

int main(void)
{
    service();
    return 0;
}
