#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>	
#include <stdlib.h>
#include <string.h>

typedef struct _node node;
typedef node* position;
typedef union element ele;


union element {
    char ch;
    int num;
};

struct _node {

    ele el;

    position left;  //child
    position right; //sibling
};

position makeNode(int el, int chNum) {
    position p = (position)malloc(sizeof(node));

    if (!p) {
        puts("Greska pri alociranju memorije");
        return NULL;
    }

    if (chNum)
        p->el.num = el;
    else
        p->el.ch = el;


    p->left = NULL;
    p->right = NULL;

    return p;
}

int pop(position head) {

    if (!head) {
        puts("Stack_Head je NULL pokazivac");
        return -1;
    }

    position temp = NULL;
    temp = head->right;

    head->right = temp->right;

    free(temp);

    return 0;
}

position push(position head, position p, int chNum) {
    if (!head || !p)
        return NULL;

    position q = makeNode(0, 0);

    if (!q) {
        puts("Greska pri alociranju memorije");
        return NULL;
    }

    if (!chNum) {    //ako je char
        p->right = head->right->left;
        pop(head);

        p->left = head->right->left;
        pop(head);

        q->right = head->right;
        head->right = q;
        q->left = p;
    }

    else {   //ako je int
        q->right = head->right;
        head->right = q;

        q->left = p;
    }
    return q;
}



void pushInStack(position path, position dir) {
    dir->right = path->right;
    path->right = dir;
}

void popInStack(position path) {
    position temp = NULL;

    do temp = (position)malloc(sizeof(node));
    while (!temp);

    temp = path->right;
    path->right = temp->right;

    free(temp);
}

int FileToBuffer(const char* file, char** buff) {

    FILE* fp = fopen(file, "r");	//Otvaranje datoteke za citanje
    if (fp == NULL) {
        puts("Greska pri otvaranju datoteke");
        return -1;	//Ukoliko je nemoguce doci do datoteke
    }

    fseek(fp, 0L, SEEK_END);	//Postavaljanje pokazivaca na kraj datoteke
    int numBytes = ftell(fp);		//Ocitavanje velicine datoteke za buffer

    *buff = (char*)malloc(numBytes);	//Stvaranje buffera
    if (*buff == NULL) {
        puts("Greska pri stvaranje buffera");
        return -1;
    }

    fseek(fp, 0L, SEEK_SET);	//Postavljanje pokazivaca na pocetak datoteke za buffer
    fread(*buff, sizeof(char), numBytes, fp);

    fclose(fp);

    return 0;
}

int  postfixToTree(const char* buff, position hTree) {

    int n = 0, numBytes = 0, el = 0;
    do {
        if (sscanf(buff + numBytes, "%d %n", &el, &n) > 0) // ako je ocitan broj  
            push(hTree, makeNode(el, 1), 1);

        else {
            sscanf(buff + numBytes, "%c %n", &el, &n);      // ako je ocitan karakter
            push(hTree, makeNode(el, 0), 0);
        }

        numBytes += n;
    } while (buff[numBytes] != '#');

    return 0;
}

int treeToBuffer(position hTree, char** buff) {
    position stackHead = (position)malloc(sizeof(node));
    position curr = hTree->right->left;

    char izraz[256] = "";
    char temp[8] = "";

    if (!stackHead) {
        puts("Greska pri alociranju memorije");
        return -1;
    }
    stackHead->left = stackHead->right = NULL;

    pushInStack(stackHead, curr);

    while (stackHead) {


        if (curr->left) {
            curr = curr->left;
            pushInStack(stackHead, curr);
            continue;
        }

        sprintf(temp, "%d", stackHead->right->el);
        strcat(izraz, temp);

        popInStack(stackHead);

        sprintf(temp, "%d", stackHead->right->el);
        strcat(izraz, temp);

        pushInStack(stackHead, curr->right);

        if (curr->left) {
            curr = curr->left;
            pushInStack(stackHead, curr);
            continue;
        }

        sprintf(temp, "%d", stackHead->right->el);
        strcat(izraz, temp);
        popInStack(stackHead);
    }

    *buff = (char*)malloc(strlen(izraz) + 1);	//Stvaranje buffera
    if (*buff == NULL) {
        puts("Greska pri stvaranje buffera");
        return -1;
    }

    strcpy(*buff, izraz);

    return strlen(izraz);
}

int TreeToFileInfix(const char* file, position hTree) {

    char* buff = NULL;

    int n = treeToBuffer(hTree, &buff);


    FILE* fp = fopen(file, "w");	//Otvaranje datoteke za pisanje
    if (fp == NULL) {
        puts("Greska pri otvaranju datoteke");
        return -1;	//Ukoliko je nemoguce doci do datoteke
    }

    fwrite(buff, 1, n, fp);

    fclose(fp);
    free(buff);

    return 0;
}

//dealociraj sve
int deleteAll(position root) {
    if (root == NULL) return -1;

    if (root->left != NULL)	deleteAll(root->left);
    if (root->right != NULL)	deleteAll(root->right);

    free(root);

    return -1;
}

int main() {

    position hStack = NULL;
    position hTree = (position)malloc(sizeof(node));
    if (!hTree) {
        puts("Greska pri alociranju memorije");
        return -1;
    }
    hTree->left = hTree->right = NULL;


    hStack = (position)malloc(sizeof(node));
    if (!hStack) {
        puts("Greska pri alociranju memorije");
        return -1;
    }

    char* buff = NULL;
    const char* file = "Infix.txt";
    const char* izraz = "4 5 + 4 2 * + 1 6 / - # ";
    
    FILE* fp = fopen(file, "w");
    fwrite(izraz, 1, strlen(izraz) + 1, fp);
    fclose(fp);

    FileToBuffer(file, &buff);
    postfixToTree(buff, hTree);

    TreeToFileInfix(file, hTree);

    free(buff);


    return 0;
}