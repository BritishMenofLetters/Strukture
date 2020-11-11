/*Napisati program za zbrajanje i množenje polinoma. Koeficijenti i eksponenti se
èitaju iz datoteke.
Napomena: Eksponenti u datoteci nisu nužno sortirani. */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

typedef struct polinom list;
typedef list* position;

struct polinom {
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

//Trazi mjesto na koje ce staviti cvor tako da ekponenti rastu pomicanjem unaprijed po listi
position findSortedPlace(position p, int exp) {
	while (p->next != NULL && p->next->exp < exp)
		p = p->next;

	return p;
}

//Sortirano stvara listu
int fAddSorted(position p, position Head, int exp) {
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
int BufferToStruct(position Head, char* buffer,int* nextPoly){
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

		fAddSorted(p, Head, p->exp);
	}

	(*nextPoly) += numBytes;
	return 0;
}

//poly1 je spremljena suma, Pomocna suma
int mSum(position poly1, position poly2) {	
	position temp = (position)malloc(sizeof(list));	//Kreiranje pokazivaca za setanje po listi Sume
	if (temp == NULL) {
		puts("Greska pri alociranju memorije");
		return -1;
	}
	temp = poly1;

	position tPoly = (position)malloc(sizeof(list));	//Kreiranje pokazivaca za setanje po listi Poly
	if (tPoly == NULL) {
		puts("Greska pri alociranju memorije");
		return -1;
	}
	tPoly = poly2->next;

	while (tPoly != NULL && temp->next != NULL)
		if (tPoly->exp == temp->next->exp) {
			position f = (position)malloc(sizeof(list));	//brisanje odradenog cvora iz polinoma
			if (f == NULL) {
				puts("Greska pri alociranju memorije");
				return -1;
			}
			temp->next->koef = tPoly->koef + temp->next->koef;	//zbroji cvor sume i polinoma i spremi u isti cvor sume

			f = tPoly;	//postavi privremeni pokazivac na odradeni cvor iz polinoma
			tPoly = tPoly->next;	//prebaci se na sljedeci cvor polinoma
			temp = temp->next;	//prebaci se na sljedeci cvor Sume

			poly2->next = tPoly;	// povezi glavu polinoma sa prvim neodradenim cvorom polinoma
			free(f);	//oslobodi odradeni cvor polinoma

		}

		else if (tPoly->exp > temp->next->exp)	temp = temp->next;	//ako je eksponent polinoma veci, prebaci na sljedeci cvor sume

		else {	//ako je eksponent polinoma manji od eksponenta sume, postavi ga ispred tog cvora sume, al ujedno ga makni iz polinoma
			poly2->next = tPoly->next;	//Povezi glavu polinoma sa sljedecim cvorom (od tPoly)
			tPoly->next = temp->next;	//Trenutni cvor (tPoly) povezi sa sljedecim cvorom Sume
			temp->next = tPoly;			//Trenutni cvor sume povezi sa trenutnim cvorom polinoma (tPoly)
			tPoly = poly2->next;			//pokazivac za setanje po polinomu vrati nazad na nadolazeci cvor polinoma
		}

	if (temp->next == NULL && tPoly != NULL)
		while (tPoly != NULL) {	//Izrezi cvor iz polinoma i ubaci ga u sumu
			poly2->next = tPoly->next;	//Povezi glavu polinoma sa sljedecim cvorom (od tPoly) 
			tPoly->next = temp->next;	//Trenutni cvor (tPoly) povezi sa sljedecim cvorom Sume
			temp->next = tPoly;			//Trenutni cvor sume povezi sa trenutnim cvorom polinoma (tPoly)

			tPoly = poly2->next;	//pokazivac za setanje po polinomu vrati nazad na nadolazeci cvor polinoma
			temp = temp->next;	//dodi na kraj sume
		}
	else temp = temp->next;	//otidi na kraj sume

	tPoly = temp = poly2;	//postavi sve na isti pokazivac da ih sve odjednom oslobodi
	free(tPoly);

	return 0;
}



void polyMultipli(position poly1, position poly2) {
	position mp = (position)malloc(sizeof(list));
	mp->next = NULL;
	mp->exp = 0; mp->koef = 0;

	position p1 = (position)malloc(sizeof(list));
	p1 = poly1;

	position p2 = (position)malloc(sizeof(list));
	p2 = poly2->next;

	while (p1->next != NULL) {
		while (p2 != NULL) {
			position t = (position)malloc(sizeof(list));

			t->exp = p1->next->exp + p2->exp;
			t->koef = p1->next->koef * p2->koef;
			t->next = NULL;

			mSum(mp, t);

			if (p1->next->next == NULL) {
				position f = (position)malloc(sizeof(list));
				f = p2;
				poly2->next = p2->next;

				free(f);
				p2 = poly2->next;
			}
			else	p2 = p2->next;
		}
		position f = (position)malloc(sizeof(list));
		f = p1->next;
		poly1->next = p1->next->next;
		free(f);

		p1 = poly1;
		p2 = poly2->next;

	}
	if (p1->next == NULL)
		while (p2 != NULL) {
			poly2->next = p2->next;	//Povezi glavu polinoma sa sljedecim cvorom (od tPoly) 
			p2->next = p1->next;	//Trenutni cvor (tPoly) povezi sa sljedecim cvorom Sume
			p1->next = p2;			//Trenutni cvor sume povezi sa trenutnim cvorom polinoma (tPoly)

			p2 = poly2->next;	//pokazivac za setanje po polinomu vrati nazad na nadolazeci cvor polinoma
			p1 = p1->next;
		}
	//poly1->next = mp->next;
	mp->next = poly1->next;
}

//pomocno mnozenje, rezultat mnozenja spremljen u poly1
int mPoly(position poly1, position poly2) {
	position mp = (position)malloc(sizeof(list));	//umnozak polinoma
	if (mp == NULL) {
		puts("Greska pri alokaciji memorije");
		return -1;
	}
	mp->next = NULL;
	
	position temp = (position)malloc(sizeof(list));	//Kreiranje pokazivaca za setanje po listi poly1
	if (temp == NULL) {
		puts("Greska pri alociranju memorije");
		return -1;
	}
	temp = poly1;

	position tPoly = (position)malloc(sizeof(list));	//Kreiranje pokazivaca za setanje po listi poly2
	if (tPoly == NULL) {
		puts("Greska pri alociranju memorije");
		return -1;
	}
	tPoly = poly2->next;


	while (temp->next != NULL) {
		while (tPoly != NULL) {
			position t = (position)malloc(sizeof(list));	//Cvor umnoska elemenata
			if (t == NULL) {
				puts("Greska pri alociranju memorije");
				return -1;
			}
			position v = (position)malloc(sizeof(list));
			v->next = NULL;

			t->exp = temp->next->exp + tPoly->exp;
			t->koef = temp->next->koef * tPoly->koef;
			t->next = NULL;
			
			v->next = t;
			mSum(mp, v);


			if (temp->next->next == NULL) {//Ako je posljednje mnozenje, brisi i odradene cvorove drugog polinoma
				position f = (position)malloc(sizeof(list));	//Za brisanje odradenog temp cvora
				if (f == NULL) {
					puts("Greska pri alociranju memorije");
					return -1;
				}
				f = tPoly->next;
				tPoly = tPoly->next->next;
				poly2->next = tPoly->next;
				free(f);
			}
			else	tPoly = tPoly->next;
		}

		position f = (position)malloc(sizeof(list));	//Za brisanje odradenog temp cvora
		if (f == NULL) {
			puts("Greska pri alociranju memorije");
			return -1;
		}
		f = temp;
		
		temp = temp->next;
		poly1->next = temp;
		free(f);
		tPoly = poly2->next;
	}

	if (temp->next == NULL)
		mSum(mp, poly2);

	poly1->next = mp->next;

	return 0;
}

//Suma polinoma
position sumPoly(char* file) {
	char* buffer = (char*)malloc(1);
	if (buffer == NULL) {
		puts("Greska pri alociranju memorije za buffer");
		return NULL;
	}

	FileToBuffer(file, &buffer);	//Prebacivanje datoteke u buffer


	position hSum = (position)malloc(sizeof(list));	//Kreiranje glave sume
	if (hSum == NULL) {
		puts("Greska pri alociranju memorije");
		return NULL;
	}
	hSum->next = NULL;

	int nextPoly = 0;	//Pamti polozaj kraja posljednjeg polinoma u bufferu

	
	while (buffer[nextPoly] != '*') {
		position poly = (position)malloc(sizeof(list));	//Kreiranje glave polinoma
		if (poly == NULL) {
			puts("Greska pri alociranju memorije");
			return NULL;
		}
		poly->next = NULL;

		BufferToStruct(poly, buffer, &nextPoly);	//Iz buffera ubaci jedan polinom

		mSum(hSum, poly);

		nextPoly++;	//Prebaci na pocetak sljedeceg polinoma
		//tPoly = temp = poly;	//postavi sve na isti pokazivac da ih sve odjednom oslobodi
		//free(tPoly);
	}
	free(buffer);

	return hSum;
}


//mnozenje polinoma
position multiPoly(char* file) {
	char* buffer = (char*)malloc(1);	//Kreiranje pokazivaca na buffer
	if (buffer == NULL) {
		puts("Greska pri alociranju memorije za buffer");
		return NULL;
	}

	FileToBuffer(file, &buffer);	//Prebacivanje datoteke u buffer

	position hMulti = (position)malloc(sizeof(list));	//Kreiranje glave umnoska
	if (hMulti == NULL) {
		puts("Greska pri alociranju memorije");
		return NULL;
	}
	hMulti->next = NULL;

	int nextPoly = 0;	//Pamti polozaj kraja posljednjeg polinoma u bufferu

	while (buffer[nextPoly] != '*') {
		position poly = (position)malloc(sizeof(list));	//Kreiranje glave polinoma
		if (poly == NULL) {
			puts("Greska pri alociranju memorije");
			return NULL;
		}
		poly->next = NULL;

		BufferToStruct(poly, buffer, &nextPoly);	//Iz buffera ubaci jedan polinom

		//mPoly(hMulti,poly);	//mnozenje polinoma sa prethodnim umnoskom 
		polyMultipli(hMulti, poly);
		nextPoly++;
	}
	free(buffer);

	return hMulti;
}



void PrintIt(position sum) {
	sum = sum->next;
	fputs("Sume je:", stdout);
	while (sum != NULL) {
		printf("%dX^%d + ", sum->koef, sum->exp);
		sum = sum->next;
	}
}


void main() {
	char* file = "Polinomi.txt";

	
	//PrintIt(sumPoly(file));
	PrintIt(multiPoly(file));
}
