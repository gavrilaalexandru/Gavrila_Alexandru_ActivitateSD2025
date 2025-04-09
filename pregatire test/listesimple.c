#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct StructuraMasina
{
    int id;
    int nrUsi;
    float pret;
    char* model;
    char* numeSofer;
    unsigned char serie;
} Masina;

typedef struct Nod
{
    Masina info;
    Nod* urm;
} Nod;

Masina citireMasinaDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    char* aux;
    fgets(buffer, 100, file);
    Masina m1;
    m1.id = atoi(strtok(buffer, sep));
    m1.nrUsi = atoi(strtok(NULL, sep));
    m1.pret = atof(strtok(NULL, sep));

    aux = strtok(NULL, sep);
    m1.model = (char*)malloc(strlen(aux) + 1);
    strcpy(m1.model, aux);

    aux = strtok(NULL,sep);
    m1.numeSofer = (char*)malloc(strlen(aux) + 1);

    m1.serie = *strtok(NULL,sep);
    return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(Nod* lista) {
    while (lista)
    {
        afisareListaMasini(lista);
        lista = lista->urm;
    }
}

void adaugaMasinaInLista(Nod** lista, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->urm = NULL;

    if (*lista == NULL)
    {
        *lista = nou;
    }
    else
    {
        Nod* temp = *lista;
        while(temp->urm)
        {
            temp = temp->urm;
        }
        temp->urm = nou;
    }
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
    FILE* file = fopen(numeFisier, "r");
    Nod* lista = NULL;

    while (!feof(file))
    {
        adaugaMasinaInLista(&lista, citireMasinaDinFisier(file));
        // adaugaLaInceputInLista(&lista, citireMasinaDinFisier(file));
    }

    fclose(file);
    return lista; // ori facem cast daca returnam void* in signatura
}

void adaugaLaInceputInLista(Nod** lista, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->urm = NULL;

    nou->urm = *lista;
    *lista = nou;
}

void dezalocareListaMasini(Nod** lista) {
    while (*lista)
    {
        Nod* temp = *lista;
        *lista = (*lista)->urm;
        free(temp->info.numeSofer);
        free(temp->info.model);
        free(temp);
    }
}

void stergeMasiniDinSeria(Nod** lista, char serieCautata) {
    while ((*lista) && (*lista)->info.serie == serieCautata)
    {
        Nod* temp = *lista;
        (*lista) = temp->urm;

        if (temp->info.numeSofer)
        {
            free(temp->info.numeSofer);
        }
        if (temp->info.model)
        {
            free(temp->info.model);
        }
        free(temp);
    }
    if ((*lista))
    {
        Nod* temp = *lista;
        while (temp)
        {
            while (temp->urm && temp->urm->info.serie != serieCautata)
            {
                temp = temp->urm;
            }
            if (temp->urm)
            {
                Nod* aux = temp->urm;
                temp->urm = aux->urm;
                if (aux->info.numeSofer)
                {
                    free(aux->info.numeSofer);
                }
                if (aux->info.model)
                {
                    free(aux->info.model);
                }
                free(aux);
            }
            else {
                temp = NULL;
            }
        }
    }
}


int main() {
    return 0;
}