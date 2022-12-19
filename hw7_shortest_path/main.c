//
//  main.c
//  ShortestPath
//
//  Created by 이균 on 2021/12/05.
//
/* To avoid security error on Visual Studio */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <inttypes.h> // strtoimax
#define FALSE 0
#define TRUE 1
#define MAX_VERTICES 20 /* Maximum number of city */
#define MAX_CITYNAME 10 /* Maximum length of cityname */
#define MAX_NR_TOKENS    20    /* Maximum length of tokens in a command */
#define MAX_COMMAND    220 /* Maximum length of command string */
#define MAX_COST 1000
#define INT_MAX 999

/* create adjacency list */
typedef struct node *nodePointer;

typedef struct node {
    int vertex;
    int rental;
    char cityName[MAX_CITYNAME];
    nodePointer link;
}Node;

typedef struct treeNode* treePointer;

typedef struct treeNode {
    char key;
    int index;
    char cityName[MAX_CITYNAME];
    treePointer leftChild;
    treePointer rightChild;
} treeNode;

typedef struct {
    char cityName[MAX_CITYNAME];
}element;

element stack[MAX_VERTICES];
element e;
int top = -1;
int cityNum;
Node adjListsArr[MAX_VERTICES];
treePointer root = NULL;
nodePointer avail = NULL;
int n = MAX_VERTICES;
short int visited[MAX_VERTICES];


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
    if (top >= MAX_VERTICES - 1)
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

/* 공백과 '-'문자를 기준으로 문자열 파싱 */
static int __parse_command(char *command, int *nr_tokens, char *tokens[])
{
    char *curr = command;
    int token_started = FALSE;
    *nr_tokens = 0;

    while (*curr != '\0') {
        if (isspace(*curr) || !strncmp(curr, "-", 1)) {
            *curr = '\0';
            token_started = FALSE;
        } else {
            if (!token_started) {
                tokens[*nr_tokens] = curr;
                *nr_tokens += 1;
                token_started = TRUE;
            }
        }
        curr++;
    }
    return 0;
}

/* create Binary Search Tree(BST) */
treePointer newNode (char* str, int i)
{
    treePointer temp = (treePointer)malloc(sizeof(treeNode));
    temp->key = str[0];
    temp->index = i;
    strcpy(temp->cityName, str);
    temp->leftChild = temp->rightChild = NULL;
    return temp;
}

treePointer insert(treePointer node, char* str, int index)
{
    /* If the tree is empty, return a new node*/
    if(node == NULL)
       return newNode(str, index);
    /* Otherwise, recur down the tree*/
    if(str[0] < node->key)
        node->leftChild= insert(node->leftChild, str, index);
    else
        node->rightChild= insert(node->rightChild, str, index);
    /* return the (unchanged) node pointer*/
    return node;
}


treePointer search(treePointer root, char* str)
{
    if(!root)
        return NULL;
    if(!strcmp(str, root->cityName)) // find
        return root;
    if(str[0] < root->cityName[0])
        return search(root->leftChild, str);
    else
        return search(root->rightChild, str);
}

int check_dup(treePointer ptr0, treePointer ptr1)
{
    nodePointer temp, temp1;
    temp = (nodePointer) (&(adjListsArr[ptr0->index]));
    temp1 = temp->link;
    
    while(temp && temp1){
        if(temp->vertex == ptr0->index && temp1->vertex == ptr1->index){
            return TRUE;
        }
        temp1 = temp1->link;
    }
   
    return FALSE;
}

/* create cajacency list */
void createAdjList(treePointer ptr0, treePointer ptr1, char* cost)
{
    nodePointer temp;
    nodePointer nodePtr0;
    nodePointer nodePtr1;
    char* endptr;
    int rental = strtoimax(cost, &endptr, 10);
    
    if(avail)
    {
        nodePtr0 = avail;
        avail = avail->link;
    }
    else
        nodePtr0 = (nodePointer)malloc(sizeof(Node));
    
    if(avail)
    {
        nodePtr1 = avail;
        avail = avail->link;
    }
    else
        nodePtr1 = (nodePointer)malloc(sizeof(Node));
    
    nodePtr0->vertex = ptr0->index;
    nodePtr0->rental = rental;
    strcpy(nodePtr0->cityName, ptr0->cityName);
    nodePtr1->rental = rental;
    nodePtr1->vertex = ptr1->index;
    strcpy(nodePtr1->cityName, ptr1->cityName);

    
    temp = adjListsArr[ptr0->index].link;
    adjListsArr[ptr0->index].link = nodePtr1;
    nodePtr1->link = temp;
    
    temp = adjListsArr[ptr1->index].link;
    adjListsArr[ptr1->index].link = nodePtr0;
    nodePtr0->link = temp;
}

/* deapth first search adjacency list */
void dfs(int v)
{
    nodePointer w;
    visited[v] = TRUE;
    for(w = adjListsArr[v].link; w; w = w->link){
        if(!visited[w->vertex])
            dfs(w->vertex);
    }
}

void connected(void)
{
    for(int i = 0; i < MAX_VERTICES ; i++)
    {
        visited[i] = FALSE;
    }
    dfs(0);
}

int choose(int distance[], int n, short int found[])
{
    int min, minpos;
    minpos = -1;
    min = INT_MAX;
    
    for(int i = 0; i < n; i++)
    {
        if(distance[i] < min && !found[i])
        {
            min = distance[i];
            minpos = i;
        }
    }
    return minpos;
}

void shortestPath(int v, int distance[], int n, short int found[], int pi[])
{
    int u;
    nodePointer temp = adjListsArr[v].link;
    nodePointer temp1;
    
    // initialize
    for(int i = 0; i < n; i++)
    {
        distance[i] = MAX_COST;
        found[i] = FALSE;
        pi[i] = -1;
    }
    found[v] = TRUE;
    distance[v] = 0;
    while(temp)
    {
        distance[temp->vertex] = temp->rental;
        pi[temp->vertex] = v;
        temp = temp->link;
    }

    
    for(int i = 0; i < n-2; i++){
        u = choose(distance, n, found);
        if(visited[u])
            found[u] = TRUE;
        temp1 = adjListsArr[u].link;
        while(temp1){
            if(!found[temp1->vertex])
            {
                if(distance[u] + temp1->rental < distance[temp1->vertex]){
                    distance[temp1->vertex] = distance[u] + temp1->rental;
                    pi[temp1->vertex] = u;
                }
            }
            temp1 = temp1->link;
        }
    }
}

/* free BST */
void freeBST (treePointer ptr)
{
    if(ptr){
        freeBST(ptr->leftChild);
        freeBST(ptr->rightChild);
        free(ptr);
    }
    return;
}

/* free adjacency list using avail  - memory reuse */
void freeAdjlist()
{
    nodePointer temp1, temp2;
    
    for(int i = 0; i < MAX_VERTICES; i++)
    {
        temp1 = adjListsArr[i].link;
        while(temp1)
        {
            temp2 = temp1;
            temp1 = temp1->link;
            if(!avail)
                avail = temp2;
            else
            {
                avail->link = temp2;
                avail = temp2;
            }
        }
        adjListsArr[i].link = NULL;
    }
}

int main(void) {
    int mode;
    FILE *input = stdin;
    int nr_tokens = 0;
    treePointer ptr0;
    treePointer ptr1;
    treePointer ret;
    printf("도시 간 최소 비용 경로 안내 서비스에 오신 것을 환영합니다.\n");
    
    
    while(1)
    {
        char command[MAX_COMMAND] = {'\0'};
        char _command[MAX_COMMAND] = {'\0'};
        char *tokens[MAX_NR_TOKENS] = { NULL };
        
        char *_tokens[MAX_NR_TOKENS] = { NULL };
        int _nr_tokens = 0;
        printf("-------------------------------\n");
        printf("원하는 기능을 선택하세요 (1: 입력 2: 검색 3: 종료)\n");
        
        scanf("%d", &mode);
        getchar();
        if(mode == 1)
        {
            freeBST(root);
            freeAdjlist();
            root = NULL;
            while(1)
            {
                printf("도시 수를 입력해 주세요\n");
                scanf("%d", &cityNum);
                getchar();
                if(cityNum == 0)
                    break;
                
                do{ // 입력이 없을 경우 방지
                    printf("도시 이름을 입력해 주세요\n");
                    fgets(command, sizeof(command), input);
                    __parse_command(command, &nr_tokens, tokens);
                }while(!nr_tokens);
                
                if(nr_tokens > cityNum){
                    printf("제시된 도시 수보다 도시 이름의 갯수가 더 많습니다.\n");
                    continue;
                }
                if(nr_tokens < cityNum){
                    printf("제시된 도시 수보다 도시 이름의 갯수가 더 적습니다.\n");
                    continue;
                }
                if(nr_tokens == cityNum)
                    break;
            }//while
            
            for(int i = 0; i < cityNum; i++){
                adjListsArr[i].vertex = i;
                strcpy(adjListsArr[i].cityName, tokens[i]);
                /* create BST */
                ret = insert(root, tokens[i], i);
                if(root == NULL)
                    root = ret;
            }
            
            printf("도시 간 도로 구축 상황을 입력하세요\n");

            while(fgets(command, sizeof(command), input))
            {
                __parse_command(command, &_nr_tokens, _tokens);
                
                if(_nr_tokens == 0)
                    continue; /* 입력이 없는 경우 방지*/
                
                if(!strcmp(_tokens[0], "."))
                {
                    printf("도시 간 도로 구축 데이터베이스를 만들었습니다.\n");
                    break;
                }
                if(search(root, _tokens[0]) == NULL || search(root, _tokens[1]) == NULL)
                {
                    printf("잘못된 도시 이름입니다\n");
                }
                else
                {
                    ptr0 = search(root, _tokens[0]);
                    ptr1 = search(root, _tokens[1]);
                    if(check_dup(ptr0, ptr1))
                    {
                        printf("중복된 정보입니다\n");
                        continue;
                    }
                    createAdjList(ptr0, ptr1, _tokens[2]);
                }
            }//while
            connected();
            //continue;
        }//if(mode == 1)
        else if(mode == 2)
        {
            int* distance = malloc(cityNum * sizeof(int));
            short int* found = malloc(cityNum * sizeof(int));
            int* pi = malloc(cityNum * sizeof(int));
            
            printf("출발 도시 이름을 입력하세요.\n");
            fgets(_command, sizeof(command), input);
            __parse_command(_command, &nr_tokens, tokens);
            printf("** 도착 도시 이름을 입력하세요 (종료 시 ‘.’ 입력)\n");
            while(fgets(command, sizeof(command), input))
            {
                __parse_command(command, &_nr_tokens, _tokens);
                
                if(_nr_tokens == 0)
                    continue; /* 입력이 없는 경우 방지*/
                
                if(!strcmp(_tokens[0], "."))
                {
                    printf("** 검색을 종료합니다\n");
                    break;
                }
                
                if(search(root, _tokens[0]) == NULL){
                    printf("잘못된 도시 이름입니다\n");
                    printf("** 도착 도시 이름을 입력하세요 (종료 시 ‘.’ 입력)\n");
                    continue;
                }
                
                
                ptr0 = search(root, tokens[0]);
                ptr1 = search(root, _tokens[0]);
                
                shortestPath(ptr0->index, distance, cityNum, found, pi);
                
                if(distance[ptr1->index] == MAX_COST){
                    printf("경로가 없습니다\n");
                    printf("** 도착 도시 이름을 입력하세요 (종료 시 ‘.’ 입력)\n");
                    continue;
                }
                //최소경로와 cost 출력
                int tempIndex = ptr1->index;
                
                strcpy(e.cityName, ptr1->cityName);
                top = -1;
                push(e);
                while(tempIndex != ptr0->index)
                {
                    strcpy(e.cityName, adjListsArr[pi[tempIndex]].cityName);
                    push(e);
                    tempIndex = pi[tempIndex];
                }
                while(top != -1){
                    e = pop();
                    printf("%s-", e.cityName);
                }
                printf("%d\n",distance[ptr1->index]);
                printf("** 도착 도시 이름을 입력하세요 (종료 시 ‘.’ 입력)\n");
            }//while
            free(distance);
            free(found);
            free(pi);
        }//else if(mode == 2)
        else if(mode == 3)
        {
            printf("서비스를 종료합니다.\n");
            break;
        }//else if(mode == 3)
        else
        {
            printf("잘못된 선택입니다.\n");
        }
    }//while
    return 0;
}
