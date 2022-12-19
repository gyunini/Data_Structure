//
//  main.c
//  자구과제1
//
//  Created by 이균 on 2022/02/18.
//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

typedef struct 
{
    char name[20];
    enum tagfield {S, C} utype;
    char phoneNum[20];
    union {
        int smoney;
        int cmoney;
    }money;
}User;


int main(void)
{
    printf("고객정보를 입력해 주십시오 (<이름> <고객유형> <금액>):\n");
    User arr[100];
    char usertype[20];
    int MaxSmoney = 0, MaxCmoney = 0;
    int MinSmoney = 0, MinCmoney = 0;
    int MaxSindex = -1, MinSindex = -1, MaxCindex = -1, MinCindex = -1, sum_S = 0, sum_C = 0, cnt_C = 0, cnt_S = 0;
    double S_ave = 0, C_ave = 0;
    for (int i = 0; i < 100; i++)
    {
        printf("이름: ");
        scanf("%s", arr[i].name);
        if (!strcmp(arr[i].name, "--"))
            break;
        printf("고객유형: ");
        scanf("%s", usertype);
        if (usertype[0] == 'S')
            arr[i].utype = S;
        else
            arr[i].utype = C;
        printf("번호: ");
        scanf("%s", arr[i].phoneNum);
        printf("금액: ");
        scanf("%d", &arr[i].money);

        if (arr[i].utype == S) // 판매자의 최댓값을 구하기 위한 조건문
        {
            if (i == 0)//맨처음입력
            {
                MaxSmoney = arr[i].money.smoney;
                MaxSindex = 0;
            }
            else
            {
                if (MaxSmoney <= arr[i].money.smoney) // 최댓값이 같을 경우도 인덱스를 최신화 해서 가장 최근 고객을 출력
                {
                    MaxSmoney = arr[i].money.smoney;
                    MaxSindex = i;
                }
            }
        }
        if (arr[i].utype == S) // 판매자의 최솟값을 구하기 위한 조건문
        {
            if (i == 0)
            {
                MinSmoney = arr[i].money.smoney;
                MinSindex = 0;
            }
            else
            {
                if (MinSmoney != 0 && MinSmoney >= arr[i].money.smoney)  // 처음 입력이 들어와서 MinSmoney가 바뀌었거나 또는 다음 입력에서 더 작은 값이 들어올 경우 또는 같은 값일 경우 가장 최근의 고객정보로 인덱스 최신화
                {
                    MinSmoney = arr[i].money.smoney;
                    MinSindex = i;
                }
                else if (MinSmoney == 0) // 처음 입력이 S가 아닌 C일 경우 MinSmoney는 여전히 0이므로 최신화해야 함
                {
                    MinSmoney = arr[i].money.smoney;
                    MinSindex = i;
                }
            }
        }
        if (arr[i].utype == C)
        {
            if (i == 0)
            {
                MaxCmoney = arr[i].money.cmoney;
                MaxCindex = 0;
            }
            else
            {
                if (MaxCmoney <= arr[i].money.cmoney)
                {
                    MaxCmoney = arr[i].money.cmoney;
                    MaxCindex = i;
                }
            }
        }
        if (arr[i].utype == C)
        {
            if (i == 0)
            {
                MinCmoney = arr[i].money.cmoney;
                MinCindex = 0;
            }
            else
            {
                if (MinCmoney != 0 && MinCmoney >= arr[i].money.cmoney)
                {
                    MinCmoney = arr[i].money.cmoney;
                    MinCindex = i;
                }
                else if (MinCmoney == 0)
                {
                    MinCmoney = arr[i].money.cmoney;
                    MinCindex = i;
                }
            }
        }


        //평균 계산 위한 조건문
        if (arr[i].utype == C)
        {
            cnt_C++;
            if (i != 0)
            {
                sum_C += arr[i].money.cmoney;
                C_ave = (double)sum_C / (double)(cnt_C);

            }
            else
            {
                sum_C += arr[i].money.cmoney;
                C_ave = sum_C;
            }
        }
        if (arr[i].utype == S)
        {
            cnt_S++;
            if (i != 0)
            {
                sum_S += arr[i].money.smoney;
                S_ave = (double)sum_S / (double)(cnt_S);
            }
            else
            {
                sum_S += arr[i].money.smoney;
                S_ave = sum_S;

            }

        }
    }

    if (MinCindex != -1 || MaxCindex != -1)
    {
        printf("최저 구매자: %s %d\n", arr[MinCindex].name, MinCmoney);
        printf("최고 구매자: %s %d\n", arr[MaxCindex].name, MaxCmoney);
        printf("평균 구매액: %.1lf\n", C_ave);
    }
    else
    {
        printf("최저 구매자: %s\n", "(none)");
        printf("최고 구매자: %s\n", "(none)");
        printf("평균 구매액: %.1lf\n", C_ave);

    }
    if (MinSindex != -1 || MaxSindex != -1)
    {
        printf("최저 판매자: %s %d\n", arr[MinSindex].name, MinSmoney);
        printf("최고 판매자: %s %d\n", arr[MaxSindex].name, MaxSmoney);
        printf("평균 판매액: %.1lf\n", S_ave);
    }
    else
    {
        printf("최저 판매자: %s\n", "(none)");
        printf("최고 판매자: %s\n", "(none)");
        printf("평균 판매액: %.1lf\n", S_ave);
    }

    return 0;
}
