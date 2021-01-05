#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct folder node;
typedef node* position;

typedef struct _string nString;
typedef nString* string;

struct folder {
	char* name;
	position child;
	position sibling;
};


struct _string{
	char ch;
	string next;
};

string findLastChar(string headS) {

	while (headS->next)	headS = headS->next;

	return headS;
}

void pushChar(char c, string headS) {

	string nC = NULL;

	do	nC = (string)malloc(sizeof(nString));
	while (!nC);
	
	nC->next = NULL;
	nC->ch = c;

	if (headS->next == NULL)
		headS->next = nC;
	else
		findLastChar(headS)->next = nC;
}

char popChar(string headS) {
	char t = 0;
	string temp = NULL;

	do temp = (string)malloc(sizeof(nString));
	while (!temp);

	if (headS->next) {
		t = headS->next->ch;

		temp = headS->next;
		headS->next = temp->next;
		free(temp);

		return t;
	}

	return '\0';
}

position findLastSibling(position theOldest) {
	while (theOldest->sibling != NULL)	theOldest = theOldest->sibling;

	return theOldest;
}

position makeFolder(char* name) {
	position folder = (position)malloc(sizeof(node));
	if (folder == NULL)	return NULL;

	int i = 0;
	while (name[i++] != '\0');
	
	folder->name = (char*)malloc(sizeof(char));

	folder->child = folder->sibling = NULL;
	strcpy(folder->name, name);

	return folder;
}

void push(position path, position dir) {
	dir->sibling = path->sibling;
	path->sibling = dir;
}

void pop(position path) {
	position temp = NULL;

	do temp = (position)malloc(sizeof(node));
	while (!temp);

	temp = path->sibling;
	path->sibling = temp->sibling;
	
	free(temp);
}

//dealociraj sve
int deleteAll(position root) { 
	if (root == NULL) return -1;
	
	if (root->child != NULL)	deleteAll(root->child);
	if (root->sibling!= NULL)	deleteAll(root->sibling);

	free(root);

	return -1;
}

position findSibling(position curr, const char* name) {
	
	if (!strcmp(curr->name, name))
		return curr;
	
	while (strcmp(curr->name, name)) {
		curr = curr->sibling;

		if (curr == NULL)
			return NULL;
	}

	return curr;
}

//2, 3 - cd, cd..
void cd(char* chDir, position* curr, position root, position path) {
	int i = 0, numByte = 0, n = 0, nPush = 0;
	char t;
	position temp = NULL;
	string sHead = NULL;
	do	sHead = (string)malloc(sizeof(nString));
	while (!sHead);
	sHead->next = NULL;

	sscanf(chDir, "%c %n", &t, &n);
	if (t == '\\') {
		temp = root->child;
		numByte += n;
	}

	if (t == '.') {
		sscanf(chDir + n, "%c", &t);
		if (t == '.') {
			pop(path);
			*curr = path->sibling;
			return;
		}
	}

	else temp = path->sibling;
	while (1) {
		while (1) {
			if (sscanf(chDir + numByte, "%c %n", &t, &n) < 0)	break;

			if ((chDir + numByte - 1)[0] == ' ') {
				for (; nPush > 0; nPush--)
					pop(path);
			}

			numByte += n;

			if (temp == '\\')	break;

			pushChar(t, sHead);
			i++;
		}

		if (!i)	break;

		char* name = (char*)calloc(i + 1, 1);
		for (i; i > 0; i--) {
			char tt[2] = { '\0' };
			tt[0] = popChar(sHead);
			strcat(name, tt);
		}

		if (!(temp = findSibling(path->sibling->child, name))) {
			for (; nPush > 0; nPush--)
				pop(path);
			//return;
		}
		if (!temp)	return;
		push(path, temp);

		if ((chDir + numByte)[0] == 0 || (chDir + numByte)[0] == '\n')	break;
	}
	*curr = path->sibling;
}

//1-md
void md(char* newDir, position curr) {
	int i = 0, numByte = 0, n = 0, sibling = 0, backslash = 0;
	char temp = 0;
	char* name = NULL;
	position pos = curr;

	string sHead = NULL;

	do	sHead = (string)malloc(sizeof(nString));
	while (!sHead);
	sHead->next = NULL;

	position newFolder = NULL;

	while (1) {
		while (1) {

			if (sscanf(newDir + numByte, "%c %n", &temp, &n) < 0)	break;

			if ((newDir + numByte - 1)[0] == ' ' && sHead->next) {
				curr = pos;
				break;
			}

			if ((newDir + numByte - 1)[0] == ' ' && !sHead->next){
				sibling++;
				curr = pos;
		}
			numByte += n;

			if (temp == '\\' && sHead->next)	break;

			else if (temp == '\\' && !sHead->next) {
				sibling++;
				continue;
			}


			pushChar(temp, sHead);
			i++;
		}

		name = (char*)calloc(i + 1, 1);
		for (i; i > 0; i--) {
			char tt[2] = {'\0'};
			tt[0] = popChar(sHead);
			strcat(name, tt);
		}

		newFolder = makeFolder(name);

		if (sibling) {
			curr = findLastSibling(curr)->sibling = newFolder;
			sibling = 0;
		}

		else if (curr->child == NULL) {
			curr->child = newFolder;
			curr = curr->child;
		}
		else
			curr = findLastSibling(curr->child)->sibling = newFolder;

		free(name);


		if ((newDir + numByte)[0] == 0 || (newDir + numByte)[0] == '\n')	break;
	}

	while (sHead->next)
		popChar(sHead);
	free(sHead);
}

//4-dir
void dir(position curr) {

	while (curr) {
		printf("\t\t<DIR>\t\t%s\n", curr->name);
		curr = curr->sibling;
	}
}

//5-exit
int izlaz(position root) {
	deleteAll(root);

	return 0;
}

void sPath(position pathN) {
	
	if (!(pathN->sibling))	return;


	sPath(pathN->sibling);
	printf("\\%s", pathN->name);

}

int main() {
	position curr = NULL;

	position root = makeFolder("root");	//head direktorija
	if (!root)	return deleteAll;

	position C = makeFolder("C:");
	if (!C)	return deleteAll;
	root->child = C;

	curr = C;

	position path = (position)malloc(sizeof(node));
	if (!path)	return deleteAll;
	path->sibling = C;

	char add[256] = "";
	char var[16] = "";
	
	while (1) {
		fputs("C:", stdout);
		sPath(path->sibling);
		fputs(">", stdout);
		fgets(add, 255, stdin);

		sscanf(add, "%s", var);

		if (!strcmp(var, "md")) {
			strcpy(add, add + 3);
			md(add, curr);
		}

		else if (!strcmp(var, "cd")) {
			strcpy(add, add + 3	);
			cd(add, &curr, root, path);
		}

		else if (!strcmp(var, "dir")) {
			strcpy(add, add + 4);
			dir(curr->child);
		}

		else if (!strcmp(var, "izlaz")) return izlaz;

		else
			printf("\'%s\' is not recognized as an internal or external command,\noperable program or batch file.\n", var);
	}

	deleteAll(root);

	return 0;
}