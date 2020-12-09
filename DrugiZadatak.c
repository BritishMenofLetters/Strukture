#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct osoba list;
typedef list* position;

struct osoba {
	char* ime;
	char* prezime;
	int godRodenja;

	position next;
};

//Unosi elemente u cvor p
void enterElements(position p) {
	char temp[45];

	rewind(stdin);	//Ocisti od prijasnjeg unosa
	fputs("Unesite ime: ", stdout);
	fgets(temp, 45, stdin);	
	
	p->ime = (char*)malloc(strlen(temp));
	strcpy(p->ime, temp);//Unesi ime

	rewind(stdin);	//Ocisti od prijasnjeg unosa
	fputs("Unesite prezime: ", stdout);
	fgets(temp, 45, stdin);

	p->prezime = (char*)malloc(strlen(temp));
	strcpy(p->prezime, temp);//Unesi prezime

	rewind(stdin);	//Ocisti od prijasnjeg unosa
	fputs("Unesite godinu rodenja: ", stdout);
	scanf("%d", &p->godRodenja);	//Unesi godinu rodenja

	p->next = NULL;
}

//a) dinamièki dodaje novi element na poèetak liste
int add(position previous){
	position p = (position)malloc(sizeof(list));
	if (p == NULL) {
		puts("Greska pri alociranju cvora");
		return -1;
	}

	enterElements(p);

	p->next = previous->next;
	previous->next = p;

	return 0;
}

//b)ispisuje listu
int printList(position p) {
	if (p == NULL) {
		puts("Lista je prazna");
		return -1;
	}

	while (p != NULL) {
		printf("\n%s %s %d", p->ime, p->prezime, p->godRodenja);
		p = p->next;
	}
	return 0;
}

//trazi zadnji cvor u listi
position findLast(position p) {
	while (p->next != NULL)
		p = p->next;

	return p;
}

//c) dinamièki dodaje novi element na kraj liste
int addAtEnd(position p) {
	position last = findLast(p);

	if (last == NULL) {
		puts("Greska! Nije pronaden zadnji element liste.");
		return -1;
	}

	return add(last);
}

//d) pronalazi element u listi (po prezimenu)
position findElement(position p, const char* prezime) {
	while (p != NULL && strcmp(p->prezime, prezime))
		p = p->next;
	
	if (p == NULL)
		puts("Takav element ne postoji");
	return p;
}

//Trazi predhodni element
position findPrevious(position p, char* prezime) {
	while (p->next != NULL && strcmp(p->next->prezime, prezime))	///sve dok sljedeci nije trazeno prezime ili kraj liste
		p = p->next;
	if (p->next == NULL) {
		printf("Ne postoji traženo prezime %s", prezime);
		return NULL;
	}

	return p;
}

//e) briše odreðeni element iz liste
int deleteElement(position p, char* prezime) {
	p = findPrevious(p, prezime);

	if (p == NULL)	return -1;

	position temp = NULL;
	
	temp = p->next;
	p->next = temp->next;
	free(temp);

	return 0;
}

//Komentirano zbog include-anja ovog file-a u TreciZadatak
/*
int main() {
	position head = NULL;
	
	head = (position)malloc(sizeof(list));
	if (head == NULL) {
		puts("Greska pri alociranju cvora");
		return -1;
	}
	head->next = NULL;

	//a) dinamièki dodaje novi element na poèetak liste
	add(head);

	//c) dinamièki dodaje novi element na kraj liste
	addAtEnd(head);

	//b) ispisuje listu
	printList(head->next);
	

	char prezime[30] = "Peric";

	//d) pronalazi element u listi (po prezimenu)
	findElement(head->next, prezime);

	//e)briše odreðeni element iz liste
	deleteElement(head, prezime);

	printList(head->next);

	free(head); //Brisanje cijele liste pa brisanje glave je napravljeno u 3. Zadatku

	return 0;
}*/