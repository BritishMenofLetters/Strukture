/*Zapis u datoteci za jednog studenta:
	brojImenaStudenta brojPrezimenaStudenta ImeStudenta[] PrezimeStudenta[] brojBodova
*/
#include "DrugiZadatak.c"


//a) dinamièki dodaje novi element iza odreðenog elementa
int addAfter(position p, char* prezime) {
	p = findElement(p, prezime);

	if (p == NULL)	return -1;	//Ukoliko ne postoji element

	return add(p);
}

//b) dinamièki dodaje novi element ispred odreðenog elementa
int  addBefore(position p, char* prezime) {
	p = findPrevious(p, prezime);

	if (p == NULL)	return -1;//Ukoliko ne postoji element

	return add(p);
}

//c) sortira listu po prezimenima osoba...Prvi nacin
void Bsort(position pocetak) {	//Repeat: Najveci element odlazi na kraj, odnosno zadnji po abecedi ide na kraj
	position p = NULL, kraj = NULL; //kraj-Zadnji sortirani element
	char temp[45];	//privremeno spremanje prezimena
	
	while (pocetak->next != kraj) {	//Sve dok pocetak iteracije nije jednak zadnjem nesortiranom elementu
		p = pocetak->next; //Pocinjemo od prvog elementa

		while (p->next != kraj) {//Sve dok ne dodemo do sortiranog elementa

			if (strcmp(p->prezime, p->next->prezime) > 0) { //Ukoliko je sljedece prezime manje od trenutnog, zamjeni
				strcpy(temp, p->prezime);
				strcpy(p->prezime, p->next->prezime);
				strcpy(p->next->prezime, temp);
			}
			p = p->next; //prebaci na sljedeci element
		}

		kraj = p;	//postavljanje kraj nove iteracije do novog(zadnjeg) sortiranog elementa
	}
}

//c) sortira listu po prezimenima osoba ...Drugi nacin
void Isort(position pocetak) {	//Repeat: Najmanji element prebaci na pocetak, odnosno prvi po abecedi na pocetak 
	position p = NULL, min = NULL;	//min - najmanji element iteracije
	char temp[45];	//privremeno spremanje prezimena

	while (pocetak->next != NULL) {
		p = pocetak->next; //postavljanje pocetak iteracije nakon sortiranih elemenata
		min = pocetak;	//novi min clan nesortiranih elemenata

		while (p != NULL) {//do kraja liste

			if (strcmp(p->prezime, min->prezime) < 0)//ako je prezime manji od min
				min = p;	//min je sad na poziciji tog prezimena
			p = p->next;	//trazi dalje do kraja liste
		}

		if (pocetak != min) {	//Ako se min promjenio tijekom iteracije (od pocetka iteracije), promjeni
			strcpy(temp, p->prezime);
			strcpy(p->prezime, p->next->prezime);
			strcpy(p->next->prezime, temp);
		}

		pocetak = pocetak->next;	//postavljanje pocetak nove iteracije nakon sortiranih elemenata
	}
}

//brojac imena i prezimena jedne osobe
void numNames(position p, int* fName, int* lName) {
	int n = 0, i=0;	//broj bitova jednog imena/prezimena // ukupno prebrojeni bitovi imena/prezimena
	char temp[45];//spremanje stringa..da se moze koristiti sscanf

	*fName = 0;
	for (i=0, n=0; i < strlen(p->ime); i += n)
		(*fName) += sscanf(p->ime + i, "%s %n", temp, &n);	//brojanje imena

	*lName = 0;
	for (i = 0, n=0; i < strlen(p->prezime); i += n)
		(*lName) += sscanf(p->prezime + i, "%s %n", temp, &n);	//brojanje prezimena
}

//d) upisuje listu u datoteku
int ListToFile(position p, char* file) {
	FILE* fp = fopen(file, "a");

	if (fp == NULL) {
		puts("Greska pri otvaranju datoteke");
		return -1;
	}

	int fName = 0, lName = 0;
	while (p != NULL) {
		numNames(p, &fName, &lName);
		
		fprintf(fp, "%d %d ", fName, lName);	//Upisi broj imena i prezimena
		fputs(p->ime, fp);	//Unesi ime/imena
		fputc(' ', fp);	//razmakni ime od prezimena
		fputs(p->prezime, fp);	//Unesi prezime/prezimena
		fprintf("%d\n", p->godRodenja);	//Unesi godinu rodenja

		p = p->next;
	}
	fclose(fp);

	return 0;
}


//Prebacivanje datoteke u buffer
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

//Upisivanje imena/prezimena iz buffera u strukturu
void WriteNames(position p, char* buffer, int* numBytes, int numNames, char* fName_lName) {
	int numNameBytes = 0, n = 0;	//broj bytova imena, broj bytova obradenih sscanf
	char temp[30], tempIme[45] = "\0";	//spremanje iz sscanf, privremeno spremanje cijeloga imena

	for (int i = 0; i < numNames; i++) {	//Kolika je memorijska velicina imena/prezimena
		sscanf(buffer + (*numBytes) + numNameBytes, "%s %n", temp, &n);
		numNameBytes += n;

		if (i > 0)	//ako je vec upisano ime/prezime, dodaj razmak
			strcat(tempIme, " ");
		strcat(tempIme, temp);	//spremi privremeno ime
	}

	if (!strcmp(fName_lName, "fName")) {	//ako su imena, ubaci u strukturu
		p->ime = (char*)malloc(numNameBytes);
		strcpy(p->ime, tempIme);
	}

	else if (!strcmp(fName_lName, "lName")) {	//ako su prezimena, ubaci u strukturu
		p->prezime = (char*)malloc(numNameBytes);
		strcpy(p->prezime, tempIme);
	}

	(*numBytes) += numNameBytes;
}

//Trazi poziciju koja je prije prosljedenog prezimena... Za abecedno sortiranje
position findSortedPlace(position p, char* prezime) { 
	while (p->next != NULL && strcmp(prezime, p->next->prezime) > 0)
	 p = p->next;
	
	return p; 
}

//Sortirano stvara listu
int fAddSorted(position p,position Head, char* prezime) {
	position place = findSortedPlace(Head, prezime);

	if (place == NULL) {
		puts("Greska. Nije moguce ubaciti u strukturu");
		return -1;
	}

	p->next = place->next;
	place->next = p;

	return 0;
}

//Iz buffera ubaci u strukturu
int bufferToStruct(position Head, char* buffer) {
	int fName, lName, numBytes = 0, n=0;	//broj imena // broj prezimena // bitovi koje je sscanf obradia
	char temp[45];	//privremena pohrana imena ili prezimena
	position p = (position)malloc(sizeof(list));
	
	while (buffer+numBytes != "\0") {
		sscanf(buffer + numBytes, "%d %d %n", &fName, &lName, &n);	//Koliko student ima imena i prezimena
		numBytes += n;

		WriteNames(p, buffer, &numBytes, fName, "fName");	//Upisi ime u strukturu
		WriteNames(p, buffer, &numBytes, lName, "lName");	//Upisi prezime u strukturu
		
		sscanf(buffer + numBytes, "%d %n", &p->godRodenja, &n);	//Upisi godinu rodenja
		numBytes += n;
		
		if (fAddSorted ( p, Head, p->prezime) == -1)	return -1;


		p = p->next;
	}
	return 0;
}

//e) èita listu iz datoteke
int FileToList(char* file, position Head) {
	char* buffer = NULL;

	if (FileToBuffer(file, &buffer) == -1)	return -1;
	
	if (bufferToStruct(Head, buffer) == -1)	return -1;

	return 0;
}