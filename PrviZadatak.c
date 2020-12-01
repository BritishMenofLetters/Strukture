/*Zapis u datoteci za jednog studenta:
	brojImenaStudenta brojPrezimenaStudenta ImeStudenta[] PrezimeStudenta[] brojBodova
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>


struct student {
	char ime[45];
	char prezime[45];
	int brBodova;
};

typedef struct student stu;


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

int numStudents(char* buffer) {
	int numBytes = 0, numStu = 0, i = 0;

	while (buffer[i] != '\0')	//Pretrazi cijeli string
		if (buffer[i++] == '\n' && buffer[i] != '\n')	//Provjera je li student upisan, tj. da nije prazan red
			numStu++;

	return numStu;
}

stu* DataToStruct(char* buffer, int numStu) {
	int fName, lName, numBytes = 0;	//broj imena // broj prezimena // bitovi koje je sscanf obradia
	char temp[30];	//privremena pohrana imena ili prezimena

	stu* nizStu = (stu*)malloc(sizeof(stu) * numStu);	//dinamicki al. niz struktura studenata
	if (nizStu == NULL) {
		puts("Greska pri alociranju niza struktura");
		return nizStu;
	}

	int n;	//obradeni bitovi jednog sscanf
	for (int i = 0; i < numStu; i++) {	//upisivanje studenata u strukturu

		sscanf(buffer + numBytes, "%d %d %n", &fName, &lName, &n);	//Koliko student ima imena i prezimena
		numBytes += n;

		strcpy(nizStu[i].ime, "\0");	//Da se moze koristi strcat zbog vise imena
		for (int j = 0; j < fName; j++) {	//Upisivanje imena
			if (j > 0)
				strcat(nizStu[i].ime, " ");	//Dodaj razmak ako je vec upisano ime

			sscanf(buffer + numBytes, "%s %n", temp, &n);	//citanje imena iz buffera
			numBytes += n;

			strcat(nizStu[i].ime, temp);	//Upisi ime
		}

		strcpy(nizStu[i].prezime, "\0");	//Da se moze koristi strcat zbog vise prezimena
		for (int j = 0; j < lName; j++) {	//Upisivanje prezimena
			if (j > 0)
				strcat(nizStu[i].prezime, " ");	//Dodaj razmak ako je vec upisano prezime

			sscanf(buffer + numBytes, "%s %n", temp, &n);//citanje prezimena iz buffera
			numBytes += n;

			strcat(nizStu[i].prezime, temp);//Upisi prezime
		}

		sscanf(buffer + numBytes, "%d %n", &nizStu[i].brBodova, &n);	//Upisivanje bodova
		numBytes += n;
	}

	return nizStu;
}

void tab(char* niz) {	//postavljanje do 3 tabulatora. Ako je max 3 prezimena
	for (int i = strlen(niz); i < 24; i += 8)
		fputs("\t", stdout);
}

int printStudents(stu nizStu[], int numStu, int maxBod) {

	if (nizStu == NULL) {
		puts("Greska pri ispisivanju studenata");
		return -1;
	}

	fputs("Ime\t\t\tPrezime\t\t   Broj bodova\t Postotak\n", stdout);

	for (int i = 0; i < numStu; i++) {
		fputs(nizStu[i].ime, stdout);
		tab(nizStu[i].ime);	//ubacivanje tabulatora

		fputs(nizStu[i].prezime, stdout);
		tab(nizStu[i].prezime);	//ubacivanje tabulatora


		printf("%d\t   %g\n", nizStu[i].brBodova, (float)nizStu[i].brBodova / maxBod * 100);
	}

	return 0;
}

int main() {
	char* file = "studenti.txt";
	int numStu = 0, maxBod = 120;

	char* buffer = (char*)malloc(sizeof(char));
	if (buffer == NULL)
		return -1;

	FileToBuffer(file, &buffer);
	numStu = numStudents(buffer);

	stu* nizStu = (stu*)malloc(sizeof(stu) * numStu);
	if (nizStu == NULL)
		return -1;

	nizStu = DataToStruct(buffer, numStu);
	free(buffer);

	printStudents(nizStu, numStu, maxBod);

	free(nizStu);

	return 0;
}