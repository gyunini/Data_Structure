//
//  main.c
//  Ch2_assignment
//  Created by 이균 on 2021/09/12.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_USERS 100
#define MAX_NAME_LEN 20
#define MAX_PHONE_LEN 14
#define TYPE_CONSUMER 'C'
#define TYPE_SELLER 'S'
#define MAX_STR 20
#define MIN_VAL 0
#define MAX_VAL 99999999

typedef union {
    int purchase;
    int sales;
} bizRecord;

typedef struct {
    char name[MAX_NAME_LEN];
    char type;
    char phone[MAX_PHONE_LEN];
    bizRecord record;
} User;

typedef struct {
    int val;
    char name[MAX_NAME_LEN];
} UserInfo;

typedef struct {
    int count;
    float average;
    UserInfo min_user;
    UserInfo max_user;
}Summary;


void print_record(User *p)
{
    printf("name %s ", p->name);
    printf("%c ", p->type);
    printf("%s ", p->phone);
    if (p->type==TYPE_SELLER)
        printf("%d\n",p->record.sales);
    else if (p->type==TYPE_CONSUMER)
        printf("%d\n",p->record.purchase);
    return;
}

int read_data(User* user_list)
{
    int size=0; User* pUser; int val;
    char type;
    char name[MAX_NAME_LEN], phone[MAX_PHONE_LEN];
    
    pUser=user_list;
    printf("고객정보를 입력해 주세요 (<이름> <고객 유형> <전화번호> <금액>):\n");
    while (1) {
        if (size >= MAX_USERS) {
            printf("이 프로그램은 최대 %d명까지만 입력받을 수 있습니다.", MAX_USERS);
            break;
        }

        scanf("%s", name);
        if (strcmp(name, "--") == 0) {
            break;
        } else {
            strcpy(pUser->name, name);
        }

        scanf(" %c %s %d", &type, phone, &val);
        if (type != TYPE_CONSUMER && type != TYPE_SELLER) {
            printf("%c은 유효하지 않는 유저 타입입니다.", type);
        }
        else
        {
            size++;
            pUser->type=type;
            strcpy(pUser->phone, phone);
            if (type==TYPE_CONSUMER) {
                pUser->record.purchase=val;
            }
            else if (type==TYPE_SELLER)    {
                pUser->record.sales=val;
            }
        //    print_record(pUser);
            pUser++;
        }
    }
    return size;
}

void initialize_data(Summary *sum)
{
    strcpy(sum->min_user.name, "(none)");
    sum->min_user.val=MAX_VAL;
    strcpy(sum->max_user.name, "(none)");
    sum->max_user.val=MIN_VAL;
    sum->average=0.0;
    return;
}

void process_data(User* users, int max_count, char type, Summary *summary)
{
    int i,j;
    User* pUser=users;
    int sum=0, user_count=0;
    
    for (i=0;i<max_count;i++){
        if (pUser->type==type && type==TYPE_CONSUMER){
            if (pUser->record.purchase <= summary->min_user.val){
                strcpy(summary->min_user.name, pUser->name);
                summary->min_user.val = pUser->record.purchase;
            }
            if (pUser->record.purchase >= summary->max_user.val){
                strcpy(summary->max_user.name, pUser->name);
                summary->max_user.val = pUser->record.purchase;
            }
            sum+=pUser->record.purchase;
            user_count++;
        } else if (pUser->type==type && type==TYPE_SELLER){
            if (pUser->record.sales <= summary->min_user.val){
                strcpy(summary->min_user.name, pUser->name);
                summary->min_user.val = pUser->record.sales;
            }
            if (pUser->record.sales >= summary->max_user.val){
                strcpy(summary->max_user.name, pUser->name);
                summary->max_user.val = pUser->record.sales;
            }
            sum+=pUser->record.sales;
            user_count++;
        }
        pUser++;
    }
    if (user_count){
        summary->average=(float)sum/user_count;
    } else {
        summary->average=0.0;
    }

    summary->count=user_count;
    return;
}

void print_results (Summary sum, char* msg)
{
    char announce[MAX_STR]; memset(announce, 0, sizeof(announce));
    printf("%s%s%s %s ", "최소", msg,"자", sum.min_user.name);
    if (sum.count) {
        printf("%d\n", sum.min_user.val);
    } else {
        printf("\n");
    }
    printf("%s%s%s %s ", "최대", msg,"자", sum.max_user.name);
    if (sum.count) {
        printf("%d\n", sum.max_user.val);
    } else {
        printf("\n");
    }
    printf("%s%s%s %.1f\n", "평균", msg,"액", sum.average);
    return;
}

int main(){
    
    Summary seller_sum, consumer_sum;
    int count;
    User users[MAX_USERS]={0};
    
    initialize_data(&consumer_sum);
    initialize_data(&seller_sum);
    
    count=read_data(users);
    
    process_data(users, count, TYPE_CONSUMER, &consumer_sum);
    process_data(users, count, TYPE_SELLER, &seller_sum);
    
    print_results(consumer_sum, "구매");
    print_results(seller_sum,"판매");
    
    return 0;
}
