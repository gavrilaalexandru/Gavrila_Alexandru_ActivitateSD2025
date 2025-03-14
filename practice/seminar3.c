#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct StructuraTelefon {
    int id;
    float diagonala;
    float pret;
    char* producator;
    char* model;
    unsigned char imei;
} Telefon;

void afisareTelefon(Telefon t) {
    printf("Id: %d\n", t.id);
    printf("Diagonala: %.1f\n", t.diagonala);
    printf("Pret: %.2f\n", t.pret);
    printf("Producator: %s\n", t.producator);
    printf("Model: %s\n", t.model);
    printf("IMEI: %c\n\n", t.imei);
}

void afisareVectorTelefoane(Telefon* telefoane, int nrTelefoane) {
    for (int i = 0; i < nrTelefoane; i++)
    {
        afisareTelefon(telefoane[i]);
    }
}

void adaugaTelefonInVector(Telefon** telefoane, int* nrTelefoane, Telefon telefonNou) {
    Telefon* temp = (Telefon*)malloc(((*nrTelefoane) + 1) * sizeof(Telefon));
    for (int i = 0; i < *nrTelefoane; i++)
    {
        temp[i] = (*telefoane)[i];
    }
    temp[*nrTelefoane] = telefonNou;
    (*nrTelefoane)++;
    free(*telefoane);
    *telefoane = temp;
}

Telefon citireTelefonFisier(FILE* file) {
    Telefon t;
    char buffer[100];
    char* bufferString;
    char separator[3] = ",\n";
    fgets(buffer, 100, file);
    t.id = atoi(strtok(buffer, separator));
    t.diagonala = atof(strtok(NULL, separator));
    t.pret = atof(strtok(NULL, separator));

    bufferString = strtok(NULL, separator);
    t.producator = (char*)malloc(strlen(bufferString) + 1 * sizeof(char));
    strcpy(t.producator, bufferString);

    bufferString = strtok(NULL, separator);
    t.model = (char*)malloc(strlen(bufferString) + 1 * sizeof(char));
    strcpy(t.model, bufferString);

    t.imei = *strtok(NULL, separator);
    // sau t.imei = strtok(NULL, separator)[0];

    return t;
}

Telefon* citireVectorTelefoaneFisier(const char* numeFisier, int* nrTelefoaneCitite) {
    FILE* file = fopen(numeFisier, "r");
    Telefon* telefoane = NULL;
    do
    {
        adaugaTelefonInVector(&telefoane, nrTelefoaneCitite, citireTelefonFisier(file));
    } while (!feof(file));

    fclose(file);
    
    return telefoane;
}

void dezalocare(Telefon** vector, int* nrTelefoane) {
    for (int i = 0; i < *nrTelefoane; i++)
    {
        free((*vector)[i].producator);
        free((*vector)[i].model);
    }
    free(*vector);
    *vector = NULL;
    *nrTelefoane = 0;
}

float pretMediuDupaDiagonala(Telefon* vector, int nrTelefoane, float diagonala) {
    float suma = 0;
    int index = 0;
    for (int i = 0; i < nrTelefoane; i++)
    {
        if(vector[i].diagonala == diagonala) {
            suma += vector[i].pret;
            index++;
        }
    }
    if (index > 0) {
        return suma / index; // pentru a evita impartirea la 0
    }
    return 0;
}

int main() {
    Telefon* telefoane = NULL;
    int nrTelefoane = 0;
    float diagonala = 5.7;
    telefoane = citireVectorTelefoaneFisier("telefoane.txt", &nrTelefoane);
    afisareVectorTelefoane(telefoane, nrTelefoane);
    float medie = pretMediuDupaDiagonala(telefoane, nrTelefoane, diagonala);
    printf("Pretul mediu al telefoanelor cu diagonala de %.1f inchi este %.2f.", diagonala, medie);
    dezalocare(&telefoane, &nrTelefoane);
    return 0;
}