#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct student stu;

struct student{
	char ime[30];
	char prezime[30];
	int	brBodova;
};


void main() {
	
	FILE* fp = NULL;
	int numBytes=0, i = 0, brStu = 0;

	fp = fopen("studenti.txt", "r");
	if (fp == NULL)	return -1;

	fseek(fp, 0, SEEK_END);
	numBytes = ftell(fp);

	fseek(fp, 0, SEEK_SET);

	char* buffer = (char*)calloc(numBytes, sizeof(char));
	if (buffer == NULL)	return 1;

	fread(buffer, sizeof(char), numBytes, fp);
	fclose(fp);

	while (1) {
		if (buffer[i] == '\0')	break;
		
		if (buffer[i++] != '\n' && buffer[i] == '\n')
			brStu++;
	}

	stu* nizStu = (stu*)malloc(sizeof(stu)*brStu);
	if (nizStu == NULL) return -1;
	
	char* prebacivac = (char*)malloc(sizeof(char));
	if (prebacivac == NULL) return -1;

	prebacivac = buffer;
	i = 0;
	int addressDistance = 0;
	while (i < brStu){
		strcpy(nizStu[i].ime,"\0");
		strcpy(nizStu[i].prezime,"\0");

			while (prebacivac[0] != ' ' && prebacivac[0] != '\n')
				strncat( nizStu[i].ime,prebacivac++, ++addressDistance/addressDistance);
			prebacivac++;
			addressDistance++;

			while (prebacivac[0] != ' ' && prebacivac[0] != '\n')
				strncat( nizStu[i].prezime,prebacivac++, ++addressDistance / addressDistance);
			prebacivac++;
			addressDistance++;

			char* a = (char*)malloc(4 * sizeof(char));
			if (a == NULL)	return -1;

			*a = '\0';
			while (prebacivac[0] != ' ' && prebacivac[0] != '\n')
				strncat(a, prebacivac++, ++addressDistance / addressDistance);

			nizStu[i].brBodova = atoi(a);
			free(a);
			if (++i == brStu)	break;
			prebacivac++;
			addressDistance++;
			
	}
	
	free(prebacivac-addressDistance);	//oslobadanje i prebacivaca i buffera


	int maxBod = 120;
	printf("Ime\tPrezime\tAp.Bod\tRe.Bod\n");
	for (i = 0; i < brStu; i++)
		printf("%s\t%s\t%d\t%.2f\n", nizStu[i].ime, nizStu[i].prezime, nizStu[i].brBodova,nizStu[i].brBodova/(float)maxBod*100);

	free(nizStu);

	return 0;
}