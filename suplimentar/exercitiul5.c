#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct StructuraMasina {
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

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret= atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy(m1.model, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}


void dezalocareListaMasini(Nod** lista) {
    while(*lista) {
        Nod* temp = *lista;
        *lista = (*lista)->urm; // pt. ca ne ducem mai in fata un element si il stergem pe cel din urma
        free(temp->info.model);
        free(temp->info.numeSofer);
        free(temp); // nu avem de ce sa setam temp si lista la NULL
    }
}

void stergeElement(Nod** lista, unsigned int pozitie) {
    if (*lista == NULL)
    {
        printf("Lista este goala!");
        return;
    }

    if (pozitie == 0)
    {
        Nod* temp = *lista; // cream un nod temporar pentru a stoca adresa pe care trb sa o eliberam
        *lista = (*lista)->urm; // avansam in lista
        free(temp); // eliberam memoria
        return;
    }

    Nod* nodCurent = *lista;
    int index = 0;

    while (nodCurent != NULL && index < pozitie - 1) // avansam pana la nodul anterior celui cautat
    {
        nodCurent = nodCurent->urm;
        index++;
    }
    
    if (nodCurent == NULL || nodCurent->urm == NULL)
    {
        printf("Pozitia %d este in afara listei. Nu se realizeaza stergerea!", pozitie);
        return;
    }

    Nod* temp = nodCurent->urm;
    nodCurent->urm = temp->urm;
    free(temp);
}

void adaugaElement(Nod** lista, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->urm = NULL;

    if ((*lista) == NULL || (*lista)->info.pret >= masinaNoua.pret) {
        nou->urm = *lista;
        *lista = nou;
        return;
    }

    Nod* nodCurent = *lista;
    while (nodCurent->urm != NULL && nodCurent->urm->info.pret < masinaNoua.pret)
    {
        nodCurent = nodCurent->urm;
    }
    nou->urm = nodCurent->urm;
    nodCurent->urm = nou;
}

Masina* transformareDinListaInVector(Nod* lista, float prag, int* dim) {
    int nrMasiniCareIndeplinescConditia = 0;
    Nod* temp = lista;

    while (temp != NULL)
    {
        if (temp->info.pret > prag)
        {
            nrMasiniCareIndeplinescConditia++;
        }
        temp = temp->urm;
    }
    

    *dim = nrMasiniCareIndeplinescConditia;

    if (nrMasiniCareIndeplinescConditia == 0)
    {
        return NULL;
    }

    Masina* vector = (Masina*)malloc(nrMasiniCareIndeplinescConditia * sizeof(Masina));

    temp = lista;
    int index = 0;

    while (temp != NULL)
    {
        if (temp->info.pret > prag)
        {
            vector[index].id = temp->info.id;
            vector[index].nrUsi = temp->info.nrUsi;
            vector[index].pret = temp->info.pret;
            vector[index].serie = temp->info.serie;

            vector[index].model = (char*)malloc(strlen(temp->info.model) + 1 * sizeof(char));
            strcpy(vector[index].model, temp->info.model);

            vector[index].numeSofer = (char*)malloc(strlen(temp->info.numeSofer) + 1 * sizeof(char));
            strcpy(vector[index].numeSofer, temp->info.numeSofer);

            index++;
        }
        temp = temp->urm;
    }
    return vector;
}

void interschimbareLista(Nod* lista, unsigned int poz1, unsigned int poz2) { // putem schimba doar info intre noduri, dar nu este optim pentru obiecte mari
    if (lista == NULL) {
        printf("Lista este goala!");
        return;
    }

    if (poz1 == poz2) {
        printf("Nu este nevoie de nicio interschimbare, pozitiile sunt identice");
        return;
    }

    Nod* nod1 = NULL, *nod2 = NULL, *temp = lista;
    int index = 0;

    while (temp != NULL)
    {
        if (index == poz1)
        {
            nod1 = temp;
        }
        else if (index == poz2)
        {
            nod2 = temp;
        }
        temp = temp->urm;
        index++;
    }

    if (nod1 == NULL || nod2 == NULL)
    {
        printf("Unul dintre noduri nu exista!");
        return;
    }
    
    Masina temporar = nod1->info;
    nod1->info = nod2->info;
    nod2->info = temporar;
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
    FILE* file = fopen(numeFisier, "r");
    Nod* lista = NULL;

    while (!feof(file))
    {
        adaugaElement(&lista, citireMasinaDinFisier(file));
    }

    fclose(file);
    return lista;
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
    for (int i = 0; i < nrMasini; i++)
    {
        afisareMasina(masini[i]);
    }
}

int main() {
    Nod* lista = citireListaMasiniDinFisier("masini.txt");
    afisareListaMasini(lista);
    stergeElement(&lista, 0);
    stergeElement(&lista, 1); // grija cu pozitiile, poate sa functioneze counter intuitive
    printf("Lista dupa stergere: \n\n");
    afisareListaMasini(lista);

    int dim = 0;
    Masina* vectorMasini = transformareDinListaInVector(lista, 4500, &dim);

    printf("Vector din lista: \n\n");
    afisareVectorMasini(vectorMasini, dim);

    printf("Lista dupa interschimbarea primelor 2 pozitii (0 si 1): \n\n");

    interschimbareLista(lista, 2, 1);
    afisareListaMasini(lista);

    dezalocareListaMasini(&lista);
    return 0;
}