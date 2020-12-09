#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef struct poly list;
typedef struct poly* position;

struct poly {
	int koef;
	int exp;
	
	position next;
};

//Prebaciva cijelu datoteku u buffer
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

//Trazi predhodni cvor
position findPrevious(position p, position next) {
	while (p->next != next)	///sve dok sljedeci nije trazeno prezime ili kraj liste
		p = p->next;
	return p;
}

//Trazi mjesto na koje ce staviti cvor tako da ekponenti rastu pomicanjem unaprijed po listi
position findSortedPlace(position p, int exp) {
	while (p->next != NULL && p->next->exp < exp)
		p = p->next;

	return p;	//vraca prethodni cvor
}

//Sortirano stvara listu
int SortedInsert(position p, position Head, int exp) {
	position place = findSortedPlace(Head, exp);

	if (place == NULL) {
		puts("Greska. Nije moguce ubaciti u strukturu");
		return -1;
	}

	p->next = place->next;
	place->next = p;

	return 0;
}

//Upisuje polinom na poziciji nextPoly buffera u strukturu
int BufferToStruct(position Head, char* buffer, int* nextPoly) {
	int n = 0, numBytes = 0;

	while (buffer[numBytes + *nextPoly] != '\0' && buffer[numBytes + *nextPoly] != '*') {
		position p = (position)malloc(sizeof(list));
		if (p == NULL) {
			puts("Greska pri alociranju memorije za cvor");
			return -1;
		}

		sscanf(buffer + numBytes + *nextPoly, "%d %d %n", &p->koef, &p->exp, &n);
		numBytes += n;
		if (p->koef == 0) {
			free(p);
			continue;
		}

		SortedInsert(p, Head, p->exp);
	}

	(*nextPoly) += numBytes;
	return 0;
}

//zbrajanje clana polinoma sa  polinomom hMulti do pozicije curr
void NodeSumi(position hMulti, position node, position curr) {
	position temp = hMulti;	//za hodanje izmedu pocetka novog polinoma i starog polinoma (hMulti)

	while (temp != curr) {//Ubaci cvor u novi polinom na pravo mjesto. Kad ga smjestis, izadi iz petlje)
		if (temp->next == curr) {	//kad se dode na pocetak starog polinoma, ubaci cvor prije
			node->next = curr;	//novi cvor pokazuje na pocetak starog polinoma
			temp->next = node;	// kraj novog polinoma postavi novi cvor
			break;
		}

		else if (temp->next->exp > node->exp) {	//ako je sljedeci cvor (exp) veci, ubaci novi cvor prije
			node->next = temp->next;
			temp->next = node;
			break;
		}

		else if (temp->next->exp == node->exp) {//ako su isti cvorovi, zbroji ih u stari cvor, a novi izbrisi
			temp->next->koef += node->koef;
			free(node);
			break;
		}
  
		temp = temp->next; //ako je sljedeci cvor (exp) manji, prebaci temp na sljedeci cvor
	}
}

//mnozenje dosadasnjeg polinoma hMulti sa novim polinomom newPoly
int polyMultipli(position hMulti, position newPoly) {
	position Mcurr = hMulti->next;	//Trenutni postaviti na pocetak starog polinoma
	position Pcurr = NULL;

	if (Mcurr == NULL)	//ako je stari polinom umnoska prazan, ubaci novi polinom
		hMulti->next = newPoly->next;

	while (Mcurr != NULL) {
		Pcurr = newPoly->next; //Trenuti postaviti na pocetak novog polinoma
		while (Pcurr != NULL) {	//Dok ne dode na kraj polinoma
			position poly = (position)malloc(sizeof(list));
			if (poly == NULL) { puts("Greska pri alociranju"); continue; }	//Pokusaj ponovno kod neuspjesne alokacije...Moze se staviti da prekine nakon nekog vremena

			poly->next = NULL;
			poly->koef = Mcurr->koef * Pcurr->koef;	//Koeficijenti se mnoze
			poly->exp = Mcurr->exp + Pcurr->exp;		//Eksponenti se zbrajaju

			NodeSumi(hMulti, poly, Mcurr);	//Novi cvor ubaci i zbroji sa novim polinomom (polinom koji je prije starog hMulti)
			
			if (Mcurr->next == NULL) {	//ako je zadnji krug mnozenja, izbrisi odradeni clan polinoma
				position temp = (position)malloc(sizeof(list));
				if (temp == NULL) {
					puts("Greska pri alociranju"); return -1;
				}
				temp = Pcurr;	//postavi temp cvor na trenutni cvor
				Pcurr = Pcurr->next;	//Prebaci na sljedeci cvor polinoma
				free(temp);
			}
			else
				Pcurr = Pcurr->next;	//Prebaci na sljedeci cvor polinoma
		}
		
		position temp = (position)malloc(sizeof(list));
		if (temp == NULL) {
			puts("Greska pri alociranju"); return -1;
		}
		temp = Mcurr;	//postavi temp cvor na trenutni cvor starog polinoma
		Mcurr = Mcurr->next;	//prebaci na sljedeci cvor
		findPrevious(hMulti, temp)->next = Mcurr; //Prijasnji clan (prije temp) spoji sa trenutnim
		free(temp);	//izbrisi odradeni cvor starog polinoma
	}
	return 0;
}

//zbrajanje dosadasnjeg polinima hSum sa novim polinomom newPoly
void polySum(position hSum, position newPoly) {
	position Scurr = hSum->next;// Pcurr = newPoly->next;

	while (Scurr != NULL && newPoly->next != NULL) {
		if (Scurr->exp > newPoly->next->exp) {	// ako je exp u sumi veci od clana polinoma, ubaci clan prije
			position previous = findPrevious(hSum, Scurr);
			previous->next = newPoly->next;
			newPoly->next = newPoly->next->next;

			previous->next->next = Scurr;

		}
		else if (Scurr->exp == newPoly->next->exp) {	//ako su exp isti, zbroji i spremi u clanu sume, a clan polinoma izbrisi
			position temp = (position)malloc(sizeof(list));
			if (temp == NULL)	continue;	//pokusaj sve dok ne uspije dinamicka alokacija

			Scurr->koef += newPoly->next->koef;
			Scurr = Scurr->next;

			temp = newPoly->next;
			newPoly->next = newPoly->next->next;
			free(temp);

		}
		else Scurr = Scurr->next; //ako je exp u sumi manji od clana polinoma, prebaci na sljedeci u sumi
	}
	if (Scurr == NULL && newPoly->next != NULL)
		findPrevious(hSum, NULL)->next = newPoly->next;	//prebaci ostatak polinoma na kraj polinoma sume
}

//-------------------ZADATAK---------------------------

//Zbrajanje polinoma iz datoteke
position sumPoly(char* file) {
	char* buffer = (char*)malloc(1);	//Kreiranje pokazivaca na buffer
	if (buffer == NULL) {
		puts("Greska pri alociranju memorije za buffer");
		return NULL;
	}

	FileToBuffer(file, &buffer);	//Prebacivanje datoteke u buffer

	
	position hSum = (position)malloc(sizeof(list));
	hSum->next = NULL;

	int nextPoly = 0;	//Pamti polozaj kraja posljednjeg polinoma u bufferu

	while (buffer[nextPoly] != '*') {//datoteka je napravljena tako da su polinomi odvojeni '*', a zadnji polinom na kraju ima "**" sto uvjet prepozna
		position newPoly = (position)malloc(sizeof(list));	//Kreiranje glave polinoma
		if (newPoly == NULL) {
			puts("Greska pri alociranju memorije");
			return NULL;
		}

		newPoly->next = NULL;

		BufferToStruct(newPoly, buffer, &nextPoly);	//Iz buffera ubaci jedan polinom

		polySum(hSum, newPoly); //sumira dosadasnji ukupni polinom sa novim

		nextPoly++;//prebaci brojac na pocetak novog polinoma(preskoci jedan '*')
		free(newPoly);	//oslobodi head prazne liste
	}

	free(buffer);

	return hSum;
}

//Mnozenje polinoma iz datoteke
position multiPoly(char* file) {
	char* buffer = (char*)malloc(1);	//Kreiranje pokazivaca na buffer
	if (buffer == NULL) {
		puts("Greska pri alociranju memorije za buffer");
		return NULL;
	}

	FileToBuffer(file, &buffer);	//Prebacivanje datoteke u buffer

	//position hMulti;
	position hMulti = (position)malloc(sizeof(list));
	hMulti->next = NULL;

	int nextPoly = 0;	//Pamti polozaj kraja posljednjeg polinoma u bufferu

	while (buffer[nextPoly] != '*') {//datoteka je napravljena tako da su polinomi odvojeni '*', a zadnji polinom na kraju ima "**" sto uvjet prepozna
		position newPoly = (position)malloc(sizeof(list));	//Kreiranje glave polinoma
		if (newPoly == NULL) {
			puts("Greska pri alociranju memorije");
			return NULL;
		}

		newPoly->next = NULL;

		BufferToStruct(newPoly, buffer, &nextPoly);	//Iz buffera ubaci jedan polinom

		polyMultipli(hMulti, newPoly); //izmnozi dosadasnji ukupni polinom sa novim

		nextPoly++;//prebaci brojac na pocetak novog polinoma(preskoci jedan '*')
		free(newPoly);	//oslobodi head prazne liste
	}
	free(buffer);

	//position Multi = hMulti->next;	//KAKO PROMJENITI DINAMICKU U STATICKU VARIJABLU 
	//free(hMulti);						//JER VRACANJEM DINAMICKE OSTAJE MI NEOSLOBODEN POKAZIVAÈ

	return hMulti;
}

//Brisanje liste
int deleteAll(position ListHead) {

	while (ListHead->next != NULL) {
		position temp = (position)malloc(sizeof(list));	//Za brisanje odradenih cvorova iz lista
		if (temp == NULL) { puts("Greska pri alociranju"); return -1; }

		temp = ListHead->next;					//spremanje cvora za brisanje u temp
		ListHead->next = ListHead->next->next;	//Novi pocetak liste spojiti na head liste

		free(temp);					//izbrisi  cvor
	}
	return 0;
}

//Ispis liste
void printIt(position headNext) {
	while (headNext != NULL) {
		printf("%d^%d ", headNext->koef, headNext->exp);
		headNext = headNext->next;

		if (headNext != NULL && headNext->koef > 0)
			printf(" +");
	}
	puts("");
}

int main() {
	char* file = "Polinomi.txt";

	position Suma = NULL, Umnozak = NULL;

	//Izracunaj sumu polinoma iz datoteke
	Suma = sumPoly(file);
	
	puts("\nSuma:");
	printIt(Suma->next);	//Ispisi sumu

	//Izracunaj umnozak polinoma iz datoteke
	Umnozak = multiPoly(file);

	puts("\nUmnozak:");
	printIt(Umnozak->next);	//Ispisi umnozak

	deleteAll(Suma);		//brisanje liste
	deleteAll(Umnozak);		//brisanje liste

	free(Suma);
	free(Umnozak);

	return 0;
}