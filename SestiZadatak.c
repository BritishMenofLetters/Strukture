/*Napisati program koji pomoæu vezanih listi simulira rad:
a) stoga,
b) reda.
Napomena: Funkcija "push" sprema cijeli broj, sluèajno generirani u opsegu od 10 -100.*/#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node list;
typedef list* position;

struct Node {
	int el;
	position next;
};

//Stvara novi cvor s elementom random vrijednosti izmedu 10 i 100 i stavlja na pocetak liste (vrh stoga)
int push(position head) {	

	position q = (position)malloc(sizeof(list));	//alociranje memorije za novi cvor
	if (q == NULL) {
		printf("Greska prilikom alociranja memorije\n");
		return -1;
	}
	

	q->el = ((double)rand() / RAND_MAX) * (100 - 10 + 1) + 10;	//Generiranje random broja izmedu 100 i 10 koji se sprema u cvor
	q->next = NULL;												//Postavljanje pokazivaca na NULL			

	if (head->next != NULL) {			//Ukoliko stog nije prazan, postavi na pocetak stoga (pocetak liste)
		q->next = head->next;			//Novi cvor pokazuje na pocetak liste
		head->next = q;					//Novi cvor postavi na pocetak liste
	}

	else			//Ukoliko je stog prazan, postavi nakon head-a
		head->next = q;

	return 0;
}

//Stvara novi cvor s elementom random vrijednosti izmedu 10 i 100 i stavlja na kraj liste (pocetak reda)
int pushArray(position head, position* last) {

	position q = (position)malloc(sizeof(list));
	if (q == NULL) {
		printf("Greska prilikom alociranja memorije\n");
		return -1;
	}

	q->el = ((double)rand() / RAND_MAX) * (100 - 10 + 1) + 10;
	q->next = NULL;

	if (*last != NULL) {
		(*last)->next = q;
		*last = q;
	}

	else {
		head->next = q;
		*last = q;
	}

	return 0;
}

//Brisanje cvora sa pocetka liste (vrha stoga)
int pop(position ListHead) {

	position temp = (position)malloc(sizeof(list));	//Za brisanje cvorova iz stoga
	if (temp == NULL) { puts("Greska pri alociranju"); return -1; }

	temp = ListHead->next;					//spremanje cvora za brisanje u temp
	ListHead->next = ListHead->next->next;	//Novi pocetak liste spojiti na head liste

	free(temp);					//izbrisi odradeni cvor

	return 0;
}

//Ispisivanje liste
printIt(position headNext) {
	
	fputs("Pocetak Liste: ",stdout);
	while (headNext != NULL) {
		printf("%d ", headNext->el);
		headNext = headNext->next;
	}
	puts("");
}


void main() {

	//--------------------------STOG---------------------------------------//
	int N = 3;	//Odredivanje koliko ce se elemenata ubaciti i izbaciti u stog/red

	position Head = (position)malloc(sizeof(list));		//Alociranje memorije za glavu stoga
	if (Head == NULL) { puts("Greska pri alociranju"); return; }

	Head->next = NULL;	//Prazna lista

	srand(time(NULL));	//Postavljanje seed-a za random brojeve za svako novo pokretanje programa

	puts("\t\tSTOG\nUpisivanje");
	for (int i = 0; i < N; i++) {	//Ubacivanje N elemenata na stog
		push(Head);		//Upisivanje elementa na vrh stoga
		printIt(Head->next);	//Ispisivanje stoga
	}	


	puts("\nBrisanje");
	for (int i = 0; i < N; i++) {	//Brisanje N elementa sa stoga
		pop(Head);	//Brisanje elementa sa vrha stoga
		printIt(Head->next);		//Ispis trenutnog stanja stoga
	}
	free(Head);

	//---------------------------RED-----------------------------------------//
	position rHead = (position)malloc(sizeof(list));		//Alociranje memorije za glavu reda
	if (rHead == NULL) { puts("Greska pri alociranju"); return; }

	rHead->next = NULL;	//Prazna lista

	position last = rHead->next;

	puts("\n\t\tRED\nUpisivanje");
	for (int i = 0; i < N; i++){	//Ubacivanje N elemenata u red
		pushArray(rHead, &last);	//Upisivanje elementa na pocetak reda (kraj liste)

	printIt(rHead->next);			//Ispisivanje reda
	}

	puts("\nBrisanje");
	for (int i = 0; i < N; i++) {	//Brisanje N elementa iz reda
		pop(rHead);					//Brisanje elementa sa kraja reda (pocetak liste)
		printIt(rHead->next);		//Ispis trenutnog stanja reda
	}
	free(rHead);
}


