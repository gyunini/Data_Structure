//
//  main.c
//  Graph
//
//  Created by 이균 on 2021/11/27.
//
/* To avoid security error on Visual Studio */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define FALSE 0
#define TRUE 1
#define MAX_VERTICES 20 /* Maximum number of city */
#define MAX_CITYNAME 10 /* Maximum length of cityname */
#define MAX_NR_TOKENS    20    /* Maximum length of tokens in a command */
#define MAX_COMMAND    220 /* Maximum length of command string */



/* create adjacency list */
typedef struct node *nodePointer;

typedef struct node {
    int vertex;
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

int cityNum;
Node adjListsArr[MAX_VERTICES];
Node visitedArr[MAX_VERTICES];
treePointer root = NULL;
short int visited[MAX_VERTICES];
nodePointer lastConnected = NULL;
nodePointer avail = NULL;

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

/* search BST */
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

/* create cajacency list */
void createAdjList(treePointer ptr0, treePointer ptr1)
{
    nodePointer temp;
    nodePointer nodePtr0;
    nodePointer nodePtr1;
    
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
    nodePtr1->vertex = ptr1->index;
    
    temp = adjListsArr[ptr0->index].link;
    adjListsArr[ptr0->index].link = nodePtr1;
    nodePtr1->link = temp;
    
    temp = adjListsArr[ptr1->index].link;
    adjListsArr[ptr1->index].link = nodePtr0;
    nodePtr0->link = temp;
}

/* scanning adjacency list and check if command is duplicated */
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

/* deapth first search adjacency list */
void dfs(int v)
{
    nodePointer w, temp;
    visited[v] = TRUE;
    
    nodePointer nodePtr0;
    if(avail)
    {
        nodePtr0 = avail;
        avail = avail->link;
    }
    else
        nodePtr0 = (nodePointer)malloc(sizeof(Node));
    
    nodePtr0->vertex = v;
    temp = lastConnected->link;
    lastConnected->link = nodePtr0;
    nodePtr0->link = temp;
    lastConnected = nodePtr0;
    
    for(w = adjListsArr[v].link; w; w = w->link){
        if(!visited[w->vertex])
            dfs(w->vertex);
    }
}

/* to determine connected component of a graph*/
void connected(void)
{
    int cnt = 0;
    for(int i = 0; i < cityNum; i++)
    {
        if(!visited[i])
        {
            lastConnected = &visitedArr[cnt];
            dfs(i);
            cnt++;
        }
    }

    if(cnt==1)
    {
        printf("성공적인 도로 구축 계획입니다\n");
    }
    else if(cnt>1)
    {
        printf("이 도로 구축은 전체 도시들을 연결하지 못하고 있습니다\n");
        for(int i = 0; i < cnt; i++)
        {
            if(visitedArr[i].link)
            {
                printf("(");
                nodePointer temp = visitedArr[i].link;
                while(temp){
                    printf(" %s ", adjListsArr[temp->vertex].cityName);
                    temp = temp->link;
                }
                printf(")");
            }
        }
        printf("로 분리됩니다\n");
        printf("다음의 도로를 추천합니다\n");
        for(int i = 0; i < cnt-1; i++)
        {
            nodePointer temp = visitedArr[i].link;
            nodePointer temp1 = visitedArr[i+1].link;
            printf("%s-%s\n", adjListsArr[temp->vertex].cityName, adjListsArr[temp1->vertex].cityName);
        }
    }
    for(int i = 0; i < MAX_VERTICES ; i++)
    {
        visited[i] = FALSE;
    }
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
    for(int i = 0; i < MAX_VERTICES; i++)
    {
        temp1 = visitedArr[i].link;
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
        visitedArr[i].link = NULL;
    }
}


/*====================================================================*/
/* main function */
int main(void) {
    FILE *input = stdin;
    int nr_tokens = 0;
    treePointer ret;
    treePointer ptr0;
    treePointer ptr1;
    printf("도시 간 경로 평가 서비스에 오신 것을 환영합니다\n");

    while(1)
    {
        char command[MAX_COMMAND] = {'\0'};
        char *tokens[MAX_NR_TOKENS] = { NULL };
        
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
            printf("제시된 도시 수보다 도시 이름의 갯수가 더 많습니다\n");
            continue;
        }
        
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
            char *_tokens[MAX_NR_TOKENS] = { NULL };
            int _nr_tokens = 0;
            
            __parse_command(command, &_nr_tokens, _tokens);
            
            if(_nr_tokens == 0)
                continue; /* 입력이 없는 경우 방지*/
            
            if(!strcmp(_tokens[0], "."))
                break;
            
            if(search(root, _tokens[0]) == NULL || search(root, _tokens[1]) == NULL)
            {
                printf("도시 이름이 잘못 입력되었습니다\n");
            }
            else{
                ptr0 = search(root, _tokens[0]);
                ptr1 = search(root, _tokens[1]);
                if(check_dup(ptr0, ptr1))
                {
                    printf("중복된 정보입니다\n");
                    continue;
                }
                createAdjList(ptr0, ptr1);
              }
        }
        connected();
        freeBST(root);
        freeAdjlist();
        root = NULL;
        printf("-------------------------------\n");
        printf("도시 간 경로 평가 서비스에 오신 것을 환영합니다\n");
    }
    printf("서비스를 종료합니다\n");
    return 0;
}
