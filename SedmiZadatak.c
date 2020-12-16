/*Napisati program koji iz datoteke čita postfiks izraz i zatim korištenjem
stoga računa rezultat. Stog je potrebno realizirati preko vezane liste.*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node list;
typedef list* position;

struct Node {
    int el;
    position next;
};

//Brisanje cvora sa pocetka liste (vrha stoga)
int pop(position ListHead) {

    position temp = (position)malloc(sizeof(list));	//Za brisanje cvorova iz stoga
    if (temp == NULL) { puts("Greska pri alociranju"); return -1; }

    temp = ListHead->next;					//spremanje cvora za brisanje u temp
    ListHead->next = ListHead->next->next;	//Novi pocetak liste spojiti na head liste

    free(temp);					//izbrisi odradeni cvor

    return 0;
}

//Prebacivanje sadrzaja datoteke u buffer
int FileToBuffer(const char* file, char** buff) {

    FILE* fp = fopen(file, "r");	//Otvaranje datoteke za citanje
    if (fp == NULL) {
        puts("Greska pri otvaranju datoteke");
        return -1;	//Ukoliko je nemoguce doci do datoteke
    }

    fseek(fp, 0L, SEEK_END);	//Postavaljanje pokazivaca na kraj datoteke
    int numBytes = ftell(fp);		//Ocitavanje velicine datoteke za buffer

    //char* buffer = (char*)malloc(numBytes);	//Stvaranje buffera
    *buff = (char*)malloc(numBytes);	//Stvaranje buffera
    if (*buff == NULL) {
        puts("Greska pri stvaranje buffera");
        return -1;
    }

    fseek(fp, 0L, SEEK_SET);	//Postavljanje pokazivaca na pocetak datoteke za buffer
    fread(*buff, sizeof(char), numBytes, fp);
    //*buff = buffer;

    fclose(fp);

    return 0;
}

int operation(position Head, const char operator) {

    switch (operator) {
    case '+':
        if (Head->next->next == NULL || Head->next == NULL)  return -1; //vrati gresku ako nema dovoljno clanova
        Head->next->next->el += Head->next->el; //zbroji na prvi operand
        return pop(Head); //popaj zadnji operand
        break;

    case '-':
        if (Head->next->next == NULL || Head->next == NULL)  return -1; //vrati gresku ako nema dovoljno clanova
        Head->next->next->el -= Head->next->el; //Oduzmi na prvi operand
        return pop(Head); //popaj zadnji operand
        break;

    case '*':
        if (Head->next->next == NULL || Head->next == NULL)  return -1; //vrati gresku ako nema dovoljno clanova
        Head->next->next->el *= Head->next->el; //Pomnozi na prvi operand
        return pop(Head); //popaj zadnji operand
        break;

    case '/':
        if (Head->next->next == NULL || Head->next == NULL)  return -1; //vrati gresku ako nema dovoljno clanova
        if (Head->next->next->el == 0) return -2;
        Head->next->next->el /= Head->next->el; //Podijeli na prvi operand
        return pop(Head); //popaj zadnji operand
        break;

    case '%':
        if (Head->next->next == NULL || Head->next == NULL)  return -1; //vrati gresku ako nema dovoljno clanova
        Head->next->next->el %= Head->next->el; //Ostatak na prvi operand
        return pop(Head); //popaj zadnji operand
        break;

    default:
        puts("Ne prepoznajem operator");
        return -1;
    }
}

//prebacivanje buffera u strukturu...push fja
int bufferToStruct(position Head, const char* buffer) {
    int numBytes = 0, n, itemp = 0;
    char ctemp = '0';

    while (1) {
        n = 0;
        sscanf(buffer + numBytes, " %d %n", &itemp, &n);

        if (n != 0) { //ako je broj, ubaci na vrh stoga
            position p = (position)malloc(sizeof(list));
            if (p == NULL) return -1;
            p->el = itemp;

            p->next = Head->next;
            Head->next = p;

            numBytes += n;
            continue;
        }
        //inace pogledaj koja je operaciju i odradi je
        sscanf(buffer + numBytes, " %c %n", &ctemp, &n);

        if (buffer[numBytes] == '#')    break;

        numBytes += n;

        if (operation(Head, ctemp) != 0) {
            while (Head->next != NULL) pop(Head); //Izbrisi listu ako je doslo do pogreske
            return -1;
        }
    }
    if (Head->next != NULL && Head->next->next == NULL) {
        printf("\nRezultat je %d\n", Head->next->el);
        return 0;
    }
    else
        return -1;
}

int main() {
    char file[20] = "Stog.txt";  //naziv datoteke u kojoj se spremljen postfiks izraz
    char* buffer = NULL; //buffer u koji ce se spremiti datoteka
    position Head = NULL;  //Glava liste (stoga)

    Head = (position)malloc(sizeof(list));
    if (Head == NULL) return -1;
    Head->next = NULL;

    FileToBuffer(file, &buffer);  //prebaciva datoteku u buffer

    // racuna postfiks izraz
    if (bufferToStruct(Head, buffer) != 0) {  //Ukoliko je greska pri radu sa stogom
        puts("Greska pri radu sa stogom");
        return -1;
    }
    return 0;
}