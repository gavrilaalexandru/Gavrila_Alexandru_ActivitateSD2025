#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct StructuraMasina
{
    int id;
    int nrUsi;
    float pret;
    char* model;
    char* numeSofer;
    unsigned char serie;
} Masina;


typedef struct Nod {
    Masina info;
    struct Nod* urm;
} Nod;

typedef struct HashTable {
    Nod** tabela;
    int dim;
} HashTable;

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
        afisareMasina(lista->info);
        lista = lista->urm;
    }
}

void adaugaMasinInLista(Nod* lista, Masina masinaNoua) { // aici presupunem ca lista nu este goala; daca ar fi goala ar trb sa fie lista transmisa prin adresa
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->urm = NULL;
    nou->info = masinaNoua;
    Nod* temp = lista;
    while (temp->urm)
    {
        temp = temp->urm;
    }
    temp->urm = nou;
}

void initializareHashTable(int dim) {
    HashTable ht;
    ht.dim = dim;
    ht.tabela = (Nod**)malloc(sizeof(Nod*) * dim);
    for (int i = 0; i < ht.dim; i++)
    {
        ht.tabela[i] = NULL;
    }
    return ht;
}

int calculeazaHash(const char* nume, int dim) {
    int suma = 0;
    for (int i = 0; i < strlen(nume); i++)
    {
        suma += nume[i];
    }
    return suma % dim;
}

void inserareInTabela(HashTable ht, Masina masinaNoua) {
    int hash = calculeazaHash(masinaNoua.numeSofer, ht.dim);

    if (ht.tabela[hash] == NULL)
    {
        ht.tabela[hash] = (Nod*)malloc(sizeof(Nod));
        ht.tabela[hash]->info = masinaNoua;
        ht.tabela[hash]->urm = NULL;
    }
    else {
        adaugaMasinInLista(ht.tabela[hash], masinaNoua);
    }
}

void afisareTabelaMasini(HashTable ht) {
    for (int i = 0; i < ht.dim; i++)
    {
        if (ht.tabela[i] != NULL)
        {
            printf("Clusterul %d:\n", i + 1);
            afisareListaMasini(ht.tabela[i]);
        }
        else {
            printf("\nIn clusterul %d nu avem masini\n", i + 1);
        }
    }
}

void dezalocareListaMasini(Nod** lista) {
    Nod* parcurgere = *lista;
    while (parcurgere)
    {
        Nod* temp = parcurgere;
        parcurgere = temp->urm;
        free(temp->info.numeSofer);
        free(temp->info.model);
        free(temp);
    }
    *lista = NULL;
}

void dezalocareHashTable(HashTable* ht) {
    for (int i = 0; i < ht->dim; i++)
    {
        dezalocareListaMasini(&(ht->tabela[i]));
    }
    free(ht->tabela);
    ht->tabela = NULL;
    ht->dim = 0;
}

float calculeazaMedieLista(Nod* lista) {
    float suma = 0;
    int nrElemente = 0;
    while (lista)
    {
        suma += lista->info.pret;
        nrElemente++;
        lista = lista->urm;
    }
    return (nrElemente > 0 ? (suma / nrElemente) : 0);
}

float* calculeazaPreturiiMediiPerClustere(HashTable ht, int* nrClustere) {
    float* preturi = NULL;
    *nrClustere = 0;
    for (int i = 0; i < ht.dim; i++)
    {
        if (ht.tabela[i] != NULL)
        {
            (*nrClustere)++;
        }
    }
    preturi = (float*)malloc(sizeof(float) * (*nrClustere));
    int index = 0;
    for (int i = 0; i < ht.dim; i++)
    {
        if (ht.tabela[i] != NULL)
        {
            preturi[index] = calculeazaMedieLista(ht.tabela[i]);
            index++;
        }   
    }
    return preturi;
}

Masina getMasinaDinLista(Nod* lista, const char* nume) {
    Masina m;
    m.id = -1;
    while (lista && strcmp(lista->info.numeSofer, nume) != 0)
    {
        lista = lista->urm;
    }
    if (lista)
    {
        m = lista->info;
        m.model = (char*)malloc(strlen(lista->info.model) + 1);
        strcpy(m.model, lista->info.model);

        m.numeSofer = (char*)malloc(strlen(lista->info.numeSofer) + 1);
        strcpy(m.numeSofer, lista->info.numeSofer);
    }
    
    return m;
}

Masina getMasinaDupaNumeSofer(HashTable ht, const char* numeCautat) {
    Masina m;
    m.id = -1;
    int hash = calculeazaHash(numeCautat, ht.dim);
    if (hash >= 0 && hash < ht.dim)
    {
        return getMasinaDinLista(ht.tabela[hash], numeCautat);
    }
    return m;
}



int main() {
    return 0;
}