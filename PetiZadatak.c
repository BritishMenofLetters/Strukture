/*Zapis u datoteci: jedan red predstavlja jedan skup (odvojeni enterom),
		s time da je nakon elementa stavljen razmak:

L1[] \nL2[] \n
*/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

typedef struct _list list;
typedef list* position;

struct _list {
	int el;
	position next;
};

//Datoteku prebaci u buffer
int FileToBuffer(char* file, char** buff) {

	FILE* fp = fopen(file, "r");	//Otvaranje datoteke za citanje
	if (fp == NULL) {
		puts("Greska pri otvaranju datoteke");
		return -1;	//Ukoliko je nemoguce doci do datoteke
	}

	fseek(fp, 0L, SEEK_END);	//Postavaljanje pokazivaca na kraj datoteke
	int numBytes = ftell(fp);		//Ocitavanje velicine datoteke za buffer

	char* buffer = (char*)malloc(numBytes);	//Stvaranje buffera
	if (buffer == NULL) {
		puts("Greska pri stvaranje buffera");
		return -1;
	}

	fseek(fp, 0L, SEEK_SET);	//Postavljanje pokazivaca na pocetak datoteke za buffer
	fread(buffer, sizeof(char), numBytes, fp);
	*buff = buffer;

	fclose(fp);

	return 0;
}

//Iz buffera ubaci u strukturu
int bufferToStruct(position Head, char* buffer, int* nextList) {
	int numBytes = 0, n=0;	// brojac byte-ova jedne liste
	position last = NULL;
	while ( n < 3 ){	//dok ne dode do kraja liste (jer uzima broj i razmak, inace bi sscanf uracunao enter)
		int el;
		sscanf(buffer + numBytes + *nextList, "%d %n", &el, &n);	//uzmi element iz buffera
		numBytes += n;	//

		position node = (position)malloc(sizeof(list));	//napravi novi cvor za listu
		if (node == NULL) { puts("Greska pri alociranju memorije"); return -1; }

		node->el = el;
		node->next = NULL;

		if (last == NULL)	Head->next = node;	//ako je pocetak liste, ubaci na pocetak

		else last->next = node;	//ako nije pocetak, baci na zadnji ubaceni cvor

		last = node;	//postaviti novi zadnji ubaceni cvor
	}
	*nextList += numBytes;

return 0;
}

//Prebacivanje cvora iz liste u uniju/presjek
void arrange(position Head, position ListHead, position* p, position* last) {
	if (*last == NULL) {				//ako je prazna lista, ubaci na pocetak

		Head->next = *p;			//prebaci cvor u Head (unija, presjek)
		*last = *p;					//zapamti zadnji umetnuti cvor

		ListHead->next = (*p)->next;//spoji sljedeci cvor sa headom	
		(*p)->next = NULL;			//kraj unije staviti na NULL
		*p = ListHead->next;		//prebaciti nazad pokazivac na pocetak liste L1
	}

	else {							//ubaci na kraj liste
		(*last)->next = *p;
		*last = *p;					//zapamti zadnji umetnuti cvor

		ListHead->next = (*p)->next;//spoji sljedeci cvor sa headom	
		(*p)->next = NULL;			//kraj unije staviti na NULL
		*p = ListHead->next;		//prebaciti nazad pokazivac na pocetak liste L1
	}
}

//Brisanje odradenog(nepotrebnog) cvora, spajanje glave sa sljedecim cvorom i postavljanje pokazivaca na taj cvor
int remove(position ListHead, position* p) {
	
	position temp = (position)malloc(sizeof(list));	//Za brisanje odradenih cvorova iz lista
	if (temp == NULL) { puts("Greska pri alociranju"); return -1; }

	temp = *p;					//spremanje cvora za brisanje u temp
	ListHead->next = (*p)->next;//Novi pocetak liste spojiti na head liste
	*p = ListHead->next;		//pokazivac za cvorove liste postaviti na pocetni cvor liste

	free(temp);					//izbrisi odradeni cvor

	return 0;
}

//Brisanje (ostatka) liste
int deleteAll(position ListHead, position* p) {
	
	while (*p != NULL) {
		position temp = (position)malloc(sizeof(list));	//Za brisanje odradenih cvorova iz lista
		if (temp == NULL) { puts("Greska pri alociranju"); return -1; }

		temp = *p;					//spremanje cvora za brisanje u temp
		ListHead->next = (*p)->next;//Novi pocetak liste spojiti na head liste
		*p = ListHead->next;		//pokazivac za cvorove liste postaviti na pocetni cvor liste

		free(temp);					//izbrisi odradeni cvor}
	}
	return 0;
}

//Unija dva skupa iz datoteke
int unia(char* file, position unia) {

	int nextList = 0;	//brojac byte-ova u datoteci (prodenih)


	char* buffer = NULL;
	FileToBuffer(file, &buffer);	//Prebaci datoteku u buffer

	position L1 = (position)malloc(sizeof(list));	//Head prve liste
	if (L1 == NULL) { puts("Greska pri alociranju"); return -1; }

	position L2 = (position)malloc(sizeof(list));	//Head druge liste
	if (L2 == NULL) { puts("Greska pri alociranju"); return -1; }

	bufferToStruct(L1, buffer, &nextList);	//Ubacivanje liste u head
	bufferToStruct(L2, buffer, &nextList);	//Ubacivanje liste u head

	position p1 = L1->next, p2 = L2->next;	//setanje po listi
	position last = NULL;					//pamcenje zadnjeg unesenog cvora u uniju

	while (p1 != NULL && p2 != NULL) {

		if (p1->el < p2->el) 				//ubaci p1
			arrange(unia, L1, &p1, &last);	//ubaci cvor u uniju i postavi p1 na pocetak liste L1
		

		else if (p1->el == p2->el) {		//ako je prazna lista, ubaci na pocetak
			arrange(unia, L1, &p1, &last);	//ubaci cvor u uniju i postavi p1 na pocetak liste L1

			remove(L2, &p2);				//Izbrisi odradeni cvor p2 iz liste L2
		}

		else 
			if (p1->el > p2->el)				//ubaci p2
				arrange(unia, L2, &p2, &last);	//ubaci cvor u uniju i postavi p2 na pocetak liste L2
		
	}

	if (p1 == NULL && p2 != NULL)		//prebaci ostatak L2 liste u uniju
		last->next = p2;

	else if (p1 != NULL && p2 == NULL)	//prebaci ostatak L1 liste u uniju
		last->next = p1;

	L1 = L2;
	free(L1);	//brisanje praznih lista
}

//Presjek dva skupa iz datoteke
int intersection(char* file, position intersection) {

		int nextList = 0;	//brojac byte-ova u datoteci (prodenih)

		char* buffer = NULL;
		FileToBuffer(file, &buffer);	//Prebaci datoteku u buffer

		position L1 = (position)malloc(sizeof(list));	//Head prve liste
		if (L1 == NULL) { puts("Greska pri alociranju"); return -1; }

		position L2 = (position)malloc(sizeof(list));	//Head druge liste
		if (L2 == NULL) { puts("Greska pri alociranju"); return -1; }

		bufferToStruct(L1, buffer, &nextList);	//Ubacivanje liste u head
		bufferToStruct(L2, buffer, &nextList);	//Ubacivanje liste u head

		position p1 = L1->next, p2 = L2->next;	//setanje po listi
		position last = NULL;					//pamcenje zadnjeg unesenog cvora u presjek

		while (p1 != NULL && p2 != NULL) {

			if (p1->el < p2->el)
				remove(L1, &p1);	//Izbrisi odradeni cvor p1 iz liste L2


			else if (p1->el == p2->el) {
				arrange(intersection, L1, &p1, &last);	//ubaci cvor u presjek i postavi p1 na pocetak liste L1

				remove(L2, &p2);	//Izbrisi odradeni cvor p2 iz liste L2
			}

			else if (p1->el > p2->el)
					remove(L2, &p2);	//Izbrisi odradeni cvor p1 iz liste

		}

		if (p1 == NULL && p2 != NULL)		//brisanje ostatka liste L2
			deleteAll(L2, &p2);

		else if(p1 != NULL && p2 == NULL)	//brisanje ostatka liste L1
			deleteAll(L1, &p1);

		L1 = L2;
		free(L1); //brisanje praznih listi

		return 0;
}

//ispis liste
void printIt(position headNext) {
	while (headNext != NULL) {
		printf("%d ", headNext->el);
		headNext = headNext->next;
	}
	puts("");
}

void main() {
	
	position Presjek = (position)malloc(sizeof(list));
	position Unija = (position)malloc(sizeof(list));

	unia("Skupovi.txt", Unija);				//trazenje unije
	puts("Unija skupova L1 i L2:");
	printIt(Unija->next);					//ispis unije

	intersection("Skupovi.txt", Presjek);	//trazenje presjeka
	puts("\nPresjek skupova L1 i L2:");
	printIt(Presjek->next);					//ispis presjeka

	deleteAll(Presjek, &(Presjek->next));	//brisanje liste Presjeka
	deleteAll(Unija, &(Unija->next));		//brisanje liste Unija

	free(Presjek);	//oslobadanje head-a Presjeka
	free(Unija);	//oslovadanje head-a Unija
}