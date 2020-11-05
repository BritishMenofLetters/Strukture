#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct osoba list;
typedef list* position;

struct osoba{
	char ime[30];
	char prezime[30];
	int godRodenja;

	position next;
};

//--------------------Drugi zadatak--------------------//

//Upisuje elemente strukture
void EnterElements(position q) {
	
	rewind(stdin);
	fputs("Unesite ime: ", stdout);
	fgets(q->ime, 30, stdin);

	rewind(stdin);
	fputs("Unesite prezime: ", stdout);
	fgets(q->prezime, 30, stdin);

	rewind(stdin);
	fputs("Unesite godinu rodenja: ", stdout);
	scanf("%d", &q->godRodenja);
}

//trazi poziciju zadnjeg cvora liste
position FindLast(position headNext) {
	while (headNext->next != NULL)
		headNext = headNext->next;
	return headNext;
}

//d) pronalazi element u listi
position FindElement(position headNext, char elem[]) {
	if (headNext == NULL)	printf("Takav element ne postoji!");

	while (headNext != NULL && strcmp(headNext->prezime, elem))
		headNext = headNext->next;

	return headNext;
}

//trazi prijasnji cvor cvora koji sadrzi elem[] (Prezime)
position FindPrev(position head, char elem[]) {
	position prev = NULL, curr = NULL;
	prev = head;
	curr = prev->next;

	while (curr != NULL && strcmp(curr->prezime, elem)) {
		prev = curr;
		curr = prev->next;
	}
	
	if (curr == NULL)	return NULL;

	return prev;
}

//a) dinamièki dodaje novi element na poèetak liste
int addAtStart(position start) {
	position q = (position)malloc(sizeof(list));
	if (q == NULL) {
		printf("Greska prilikom alociranja memorije\n");  
		return -1;
	}
	
	EnterElements(q);

	q->next = start->next;
	start->next = q;

	return 0;
}

//b) ispisuje listu
int printList(position head) {
	if (head == NULL) {
		printf("Nema elemenata. Lista je prazna");
		return -1;
	}
	
	while (head != NULL) {
		printf("%s\t%s\t%d\n", head->ime, head->prezime, head->godRodenja);
		head = head->next;
	}
	return 0;
}

//c) dinamièki dodaje novi element na kraj liste
int addAtEnd(position head) {
	position last = NULL;
	last = FindLast(head->next);
	
	if (last == NULL) {
		printf("Greska! Nije pronaden zadnji element liste.");
		return -1;
	}

	addAtStart(last);
	return 0;
}

//e) briše odreðeni element iz liste
int deleteElement(position head, char elem[]) {
	position curr = NULL, prev = NULL;
	prev = FindPrev(head,elem);

	if (prev == NULL) {
		printf("Takav element ne postoji!");
		return -1;
	}

	curr = prev->next;
	prev->next = curr->next;
	free(curr);

	return 0;
}

//--------------------Treci zadatak--------------------//

//a) dinamièki dodaje novi element iza odreðenog elementa
int addAfter(position p, char el[]) {
	p = FindElement(p, el);

	if (p == NULL)	return -1;


	return addAtStart(p);
}

//b) dinamièki dodaje novi element ispred odreðenog elementa
int addBefore(position p, char el[]) {
	p = FindPrev(p, el);

	if (p == NULL)	return -1;

	return addAtStart(p);
}

//c) sortira listu po prezimenima osoba ..prvi nacin
void Bsort(position pocetak) {
	position p = NULL, kraj = NULL;
	while (pocetak->next != kraj) {
		p = pocetak->next;

		while (p->next != kraj) {

			if (strcmp(p->prezime, p->next->prezime) > 0) {
				strcat(p->prezime, p->next->prezime);
				strncpy(p->next->prezime, p->prezime, strlen(p->prezime) - strlen(p->next->prezime));
				strncpy(p->prezime, &p->next->prezime[strlen(p->prezime) - strlen(p->next->prezime)], strlen(p->prezime) - strlen(p->next->prezime));
			}
			p = p->next;
		}

		kraj = kraj->next;
	}
}

//c) sortira listu po prezimenima osoba ...Drugi nacin
void Isort(position pocetak) {
	position p = NULL, min = NULL;

	while (pocetak->next != NULL) {
		p = pocetak->next;
		min = pocetak;

		while (p != NULL) {

			if (strcmp(p->prezime, min->prezime) < 0)
				min = p;
			p = p->next;
		}

		if (pocetak != min) {
			strcat(min->prezime, pocetak->prezime);
			strncpy(pocetak->prezime, min->prezime, strlen(min->prezime) - strlen(pocetak->prezime));
			strncpy(min->prezime, &pocetak->prezime[strlen(min->prezime) - strlen(pocetak->prezime)], strlen(min->prezime) - strlen(pocetak->prezime));
		}

		pocetak = pocetak->next;
	}
}



//d)
int ListToFile(position p, char file[]) {	FILE* fp = fopen(file, "a");	if (fp == NULL) {		puts("Greska pri otvaranju datoteke");		return -1;	}	while (p != NULL) {		fprintf(file, "%s %s %d\n", p->ime, p->prezime, p->godRodenja);		p = p->next;	}	fclose(fp);	return 0;}
int FileToBuffer(char file[], char buffer[]) {
	FILE* fp = NULL;
	int numBytes = 0;

	fp = fopen(file, "r");
	if (fp == NULL)	return -1;

	fseek(fp, 0, SEEK_END);
	numBytes = ftell(fp);

	fseek(fp, 0, SEEK_SET);

	buffer = (char*)calloc(numBytes, sizeof(char));

	if (buffer == NULL)	return -1;

	if (fread(buffer, sizeof(char), numBytes, fp) != numBytes)	return -1;
	fclose(fp);

	return 0;
}

int FindStudents(char buffer[]) {
	int i = 0, brStu = 0;
	while (1) {
		if (buffer[i] == '\0')	break;

		if (buffer[i++] != '\n' && buffer[i] == '\n')
			brStu++;
	}
	return brStu;
}

int fEnterElementsList(position q, char buffer[], int brStu) {

	char ime[30] = "\0", prezime[30] = "\0";

	char* prebacivac = (char*)malloc(sizeof(char));
	if (prebacivac == NULL) return -1;

	prebacivac = buffer;

	int i = 0, j = 0;
	while (i < brStu) {
		position r = (position)malloc(sizeof(list));

		*ime = "\0";
		while (prebacivac[0] != ' ' && prebacivac[0] != '\n')
			strncat(ime, prebacivac++, ++j / j);
		strcpy(r->ime, ime);


		prebacivac++;
		++j;

		*prezime = "\0";
		while (prebacivac[0] != ' ' && prebacivac[0] != '\n')
			strncat(prezime, prebacivac++, ++j / j);
		strcpy(r->prezime, prezime);
		prebacivac++;
		++j;

		char* a = (char*)malloc(4 * sizeof(char));
		if (a == NULL)	return -1;

		*a = '\0';
		while (prebacivac[0] != ' ' && prebacivac[0] != '\n')
			strncat(a, prebacivac++, ++j / j);

		r->godRodenja = atoi(a);
		free(a);

		r->next = q->next;
		q->next = r;


		if (++i == brStu)	break;
		prebacivac++;
		++j;
	}

	free(prebacivac - j);

	return 0;
}

//e) èita listu iz datoteke
int FileToList(char file[], position p) {
	char* buffer = NULL;
	int brStu = 0;

	if (FileToBuffer(file, buffer)) return -1;

	brStu = FindStudents(buffer);

	return fEnterElementsList(p, buffer, brStu);
}



void main() {

	position Head = (position)malloc(sizeof(list));
	Head->next = NULL;

}