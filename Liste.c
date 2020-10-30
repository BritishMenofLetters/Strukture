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

//Upisuje elemente strukture
void EnterElements(position q) {
	
	rewind(stdin);
	printf("Unesite ime: ");
	fgets(q->ime, 10, stdin);

	rewind(stdin);
	printf("Unesite prezime: ");
	fgets(q->prezime, 20, stdin);

	rewind(stdin);
	printf("Unesite godinu rodenja: ");
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




void main() {

	position Head = (position)malloc(sizeof(list));
	Head->next = NULL;
}