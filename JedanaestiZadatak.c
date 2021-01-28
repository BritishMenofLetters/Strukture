#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HASH_MAX 11

typedef struct hashTable* hashT;

typedef struct _node node;
typedef node* position;

struct hashTable {
	int sizeT;
	position* hashLists;
};

struct _node {
	
	char* ID;
	char* fname;
	char* lname;

	position next;
};


/*		Copy file data to buffer
		Return size of file				*/
int fileToBuffer(const char* file_name, char** buffer) {

	int file_size = 0;
	/*1. Open file*/
	FILE* fp = fopen(file_name, "r");

	/*2. Check whether fopen() was successful or not*/
	if (!fp) {
		perror("fopen() failed");
		return -1;
	}
	/*3. Set pointer at the end of the file*/
	if (fseek(fp, 0L, SEEK_END)) {
		perror("fseek() ");
		fprintf(stderr, "fseek() failed in file %s at line # %d\n", __FILE__, __LINE__);
		return -1;
	}

	/*4. Read position of the file pointer .... read size of the file*/
	file_size = ftell(fp);

	/*5. Check success of ftell() */
	if (file_size == -1L) {
		perror("ftell() ");
		fprintf(stderr, "ftell() failed in file %s at line # %d\n", __FILE__, __LINE__);
		return -1;
	}
	/*6. Set file pointer at beginnig of the file*/
	if (fseek(fp, 0L, SEEK_SET)) {
		perror("fseek() ");
		fprintf(stderr, "fseek() failed in file %s at line # %d\n", __FILE__, __LINE__);
		return -1;
	}

	/*7. Allocate buffer size of the file*/
	*buffer = (char*)malloc(file_size + 1);

	/*8. Check success of malloc()*/
	if (!(*buffer)) {
		perror("malloc() ");
		fprintf(stderr, "malloc() failed to allocate memory for buffer-file");
		return -1;
	}

	/*9. Copy file data into buffer*/
	if (fread(*buffer, 1, file_size, fp) != file_size) {
		perror("fread() ");
		fprintf(stderr, "fread() failed to read entire file");
		return -1;
	}

	/*10. Close file*/
	if (fclose(fp)) {
		perror("fclose() ");
		fprintf(stderr, "fclose() failed to close file");
		return -1;
	}

	return file_size;
}

int hashFunction(const char* lname, int capacity) { 

	int i = 0, key = 0;

	for (i = 0; i < 5; i++) {
		if (!lname[i] || lname[i] == '\n')
			break;
		
		if (isalpha(lname[i]))
			key += lname[i];
	}

	return key % capacity;
}

int checkPrime(int n) {
	int i = 0;
	if (n == 1 || !n)	return 0;

	for (i = 2; i < n / 2; i++)
		if (n % i == 0) return 0;

	return 1;
}

int getPrime(int n) {
	if (n % 2 == 0)
		n++;
	while (!checkPrime(n))	n++;

	return n;
}

hashT initalTabel(int tableSize) {
	hashT H = NULL;
	int i;

	H = (hashT)malloc(sizeof(struct hashTable));
	if (!H) {
		perror("malloc() ");
		fputs("Neuspjesna alokacija memorije hash tablice", stderr);
		return NULL;
	}
	
	H->sizeT = getPrime(tableSize);

	H->hashLists = (position*)malloc(sizeof(position) * H->sizeT);
	if (!H->hashLists) {
		perror("malloc() ");
		fputs("Neuspjesna alokacija memorije hash liste", stderr);
		return NULL;
	}

	for (i = 0; i < H->sizeT; i++)
		H->hashLists[i] = H->hashLists[i]->next = NULL;

	return H;
}

position findIt(const char* fullname, hashT H) {
	position L = NULL;
	const char* fname = NULL, *lname = NULL;
	sscanf(fullname, "%s %s" ,fname, lname);

	L = (H->hashLists[hashFunction(lname, H->sizeT)]);
	
	while (!L && strcmp(L->lname, lname))	L = L->next;

	return L;
}

void printStudent(position student) {
	if (!student) {
		puts("Proslijeden NULL element");
		return;
	}

	printf("Maticni broj studenta %s %s je %s", student->fname, student->lname, student->ID);
}

hashT printTable(hashT H) {
	int i = 0;
	position temp = NULL;

	if (!H) {
		puts("Hash tablica ne postoji");
		return;
	}

	printf("%5s%20s%20s%20s\n", "key", "ID", "First name", "Last name");
	
	for (; i < H->sizeT; i++) {
		if (!H->hashLists[i]->next)	continue;

		temp = H->hashLists[i]->next;

		while (!temp) 
			printf("%5s%20s%20s%20s\n", i, temp->ID, temp->fname, temp->lname);
	}

	return H;
}

void insertIt(const char* fullnameID, hashT H) {
	position new_node = NULL, L = NULL, temp = NULL;
	char* ID = NULL, * fname = NULL, * lname = NULL;
	char t[30] = "";
	int numID = 0, numf = 0, numl = 0;

	new_node = (position)malloc(sizeof(node));
	if (!_callnewh) {
		perror("malloc() ");
		fputs("Neuspjesna alokacija memorije novog cvora", stderr);
		return NULL;
	}

	sscanf(fullnameID, "%s %n %s %n %s %n", t, &numID, t, &numf, t, &numf);

	new_node->ID = (char*)malloc(sizeof(numID));
	new_node->fname = (char*)malloc(sizeof(numf));
	new_node->lname = (char*)malloc(sizeof(numl));
	if (!new_node->ID || !new_node->fname || !new_node->lname)	return;

	sscanf(fullnameID, "%s%s%s", new_node->ID, new_node->fname, new_node->lname);

	temp = H->hashLists[hashFunction(lname, H->sizeT)];

	
	while (strcmp(lname, temp->next->lname) > 0 || !temp->next)
			temp = temp->next;

		new_node->next = temp->next;
		temp->next = new_node;
	
}

void deleteIt(hashT H) {
	int i = 0;
	position temp = NULL;

	if (!H) {
		puts("Hash tablica je prazna");
		return;
	}

	for (; i < H->sizeT; i++) {

		while (!H->hashLists[i]->next) {
			temp = H->hashLists[i]->next;
			H->hashLists[i]->next = temp->next;
			free(temp->ID);
			free(temp->fname);
			free(temp->lname);
			free(!temp);
		}
	}

	return H;
}

int main(void) {
	
	const char* fileName = "Studenti.txt";
	const char* context = "12324 Ivo Ivic\n31415 Ante Antic\n325235 Pero Peric\n5624423 Dinko Dinkovic\n573456 Eva Evanor\n24235 Ivana Ivano\n4684657 Maris Maricev\n7635765 Ina Inovic\n";
	char* buffer = NULL, temp[30] = "", fullNameID[90] = "";
	hashT H = NULL;
	int numBytes = 0, m = 0, n = 0, i = 0;

	FILE* fp = fopen(fileName, "a");
	if (!fp)	return -1;

	fwrite(context, 1, strlen(context), fp);
	fclose(fp);


	H = initalTabel(HASH_MAX);

	numBytes = fileToBuffer(fileName, &buffer);

	while (n < numBytes) {

		for (i = 0; i < 3; i++) {
			sscanf(buffer + n, "%s %n", temp, m);
			n += m;
			strcat(fullNameID, temp);
		}
		insertIt(fullNameID, H);
	}

	printTable(H);
	puts("\nPritisnite bilo koju tipku osim nule ako zelite ispis studentov maticni broj");
	scanf("%d", &i);
	
	while (i) {
		puts("\nUnesite ime i prezime studenta");
		fgets(fullNameID, 90, stdin);
		printStudent(findIt(fullNameID, H));

		puts("\nZa novo traženje pritisnite nenulu");
	}


	return 0;
}