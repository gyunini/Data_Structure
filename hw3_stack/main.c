//
//  main.c
//  자구 과제3
//
//  Created by 이균 on 2022/02/18.
//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#define MAX_STACK_SIZE 50
#define EXIT_FAILURE -1
#define MAX_STRING_LEN 50

typedef struct {
    char c;
}element;

element stack[MAX_STACK_SIZE];

int top = -1;

int i = 0, j = 0, cnt = 0;
int unmatch = 0;
char opp = '\0';
element e;
char input_string[MAX_STRING_LEN] = { 0 };
char redundant[MAX_STRING_LEN] = { 0 };

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

void redundant_string(char *p)
{
    if (j != 0)
    {
        if (cnt == 1)
            printf("** Your string is not balanced. \"%s\" is redundant.\n", p);
        else
            printf("** Your string is not balanced. \"%s\" are redundant.\n", p);
    }
}

void closed()
{
    if (top != -1 && unmatch == 0)
    {
        printf("** Your string is not balanced. \"");
        for (int k = 0; k <= top; k++)
        {
            printf("%c", stack[k].c);
        }
        if (top == 0)
            printf("\" is not closed\n");
        else
            printf("\" are not closed\n");
    }
    else if (top == -1 && unmatch == 0 && j == 0)
        printf("** Your string is balanced\n");
}

void check(char* p, char* ptr)
{
    while (*(p + i) != '\0')
    {
        if (*(p + i) == '(' || *(p + i) == '{' || *(p + i) == '[')
        {
            e.c = *(p + i);
            push(e);
        }
        else if (*(p + i) == ')' || *(p + i) == '}' || *(p + i) == ']')
        {
            if (top != -1)
            {
                if ((stack[top].c == '(' && *(p + i) == ')') || (stack[top].c == '{' && *(p + i) == '}') ||
                    (stack[top].c == '[' && *(p + i) == ']')) // 짝이 맞으면
                {
                    e = pop();
                }
                else
                {
                    if (stack[top].c == '(')
                        opp = ')';
                    else if (stack[top].c == '{')
                        opp = '}';
                    else if (stack[top].c == '[')
                        opp = ']';
                    printf("** Your string is not balanced. '%c' should have been '%c'.\n", *(p + i), opp);
                    unmatch = 1;
                    break;
                }
            }
            else
            {
                *(ptr + j) = *(p + i);
                j++;
                cnt++;
            }
        }
        i++;
    }
}

void process()
{
    while (1)
    {
        //변수 초기화
        i = 0;
        unmatch = 0;
        j = 0;
        cnt = 0;

        char input_string[MAX_STRING_LEN] = { 0 };
        char redundant[MAX_STRING_LEN] = { 0 };
        top = -1;

        printf("Input your string: ");
        scanf("%s", input_string);

        if (input_string[i] == 'q')
        {
            printf("** Bye\n");
            return;
        }

        check(input_string, redundant);
        
        redundant_string(redundant);
        
        closed();
    }
}

int main(void)
{
    process();
    return 0;
}

//while (input_string[i] != '\0')
        //{
        //    if (input_string[i] == '(' || input_string[i] == '{' || input_string[i] == '[')
        //    {
        //        e.c = input_string[i];
        //        push(e);
        //    }
        //    else if (input_string[i] == ')' || input_string[i] == '}' || input_string[i] == ']')
        //    {
        //        if (top != -1)
        //        {
        //            if ((stack[top].c == '(' && input_string[i] == ')') || (stack[top].c == '{' && input_string[i] == '}') ||
        //                (stack[top].c == '[' && input_string[i] == ']')) // 짝이 맞으면
        //            {
        //                e = pop();
        //            }
        //            else
        //            {
        //                if (stack[top].c == '(')
        //                    opp = ')';
        //                else if (stack[top].c == '{')
        //                    opp = '}';
        //                else if (stack[top].c == '[')
        //                    opp = ']';
        //                printf("** Your string is not balanced. '%c' should have been '%c'.\n", input_string[i], opp);
        //                unmatch = 1;
        //                break;
        //            }
        //        }
        //        else
        //        {
        //            redundant[j] = input_string[i];
        //            j++;
        //            cnt++;
        //        }
        //    }
        //    i++;
        //}
