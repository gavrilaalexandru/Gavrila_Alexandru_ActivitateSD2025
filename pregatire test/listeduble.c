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
    struct Nod* ant;
} Nod;

typedef struct ListaDubla {
    Nod* prim;
    Nod* ultim;
    int nrNoduri;
} ListaDubla;

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaDeLaInceput(ListaDubla lista) {
    Nod* p = lista.prim;
    while (p)
    {
        afisareMasina(p->info);
        p = p->urm;
    }
}

void afisareListaDeLaSfarsit(ListaDubla lista) {
    Nod* p = lista.ultim;
    while (p)
    {
        afisareMasina(p->info);
        p = p->ant;
    }
}

void adaugaLaSfarsitInLista(ListaDubla* lista, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->urm = NULL;
    nou->ant = lista->ultim;

    if (lista->ultim != NULL)
    {
        lista->ultim->urm = nou;
    }
    else
    {
        lista->prim = nou;
    }
    lista->ultim = nou;
    lista->nrNoduri++;
}

void adaugaLaInceputInLista(ListaDubla* lista, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->ant = NULL;
    nou->urm = lista->prim;
    if (lista->prim != NULL)
    {
        lista->prim->ant = nou;
    }
    else
    {
        lista->ultim = nou;
    }
    lista->prim = nou;
    lista->nrNoduri++;
}

void dezalocareListaDubla(ListaDubla* lista) {
    Nod* temp = lista->prim; // se poate si fara temp; direct in while cu lista->prim
    while (temp)
    {
        Nod* aux = temp;
        temp = aux->urm;
        if (aux->info.model)
        {
            free(aux->info.model);
        }
        if(aux->info.numeSofer)
        {
            free(aux->info.numeSofer);
        }
        free(aux);
    }
    lista->prim = NULL;
    lista->ultim = NULL;
    lista->nrNoduri = 0;
}

void stergeMasinaDupaId(ListaDubla* lista, int id) {
    if (lista->prim == NULL)
    {
        return;
    }
    Nod* temp = lista->prim;
    while (temp != NULL && temp->info.id != id)
    {
        temp = temp->urm;
    }
    if (temp == NULL)
    {
        return;
    }
    if (temp->ant == NULL)
    {
        lista->prim = temp->urm; // daca este primul nod
        if (lista->prim)
        {
            lista->prim->ant = NULL;
        }
    }
    else {
        temp->ant->urm = temp->urm; // se seteaza pt nodul din spate se seteaza nodul urmator
    }
    if (temp->urm != NULL)
    {
        temp->urm->ant = temp->ant; // se seteaza pt nodul din fata se noteaza nodul anterior, doar daca nu suntem pe ultimul nod
    }
    else {
        lista->ultim = temp->ant; // daca nodul urmator este null (adica suntem pe ultimul nod)
    }                           // setam ultimul nod ca fiind cel anterior celui curent (pt. ca pe cel curent il vom sterge)
    if (temp->info.model)
    {
        free(temp->info.model);
    }
    if (temp->info.numeSofer)
    {
        free(temp->info.numeSofer);
    }
    free(temp);
    lista->nrNoduri--;
}

char* getNumeSoferCeaMaiScumpaMasina(ListaDubla lista) {
    if (lista.prim)
    {
        Nod* max = lista.prim;
        Nod* temp = lista.prim->urm;

        while (temp)
        {
            if (temp->info.pret > max->info.pret)
            {
                max = temp;
            }
            temp = temp->urm;
        }
        char* nume = (char*)malloc(strlen(max->info.numeSofer) + 1);
        strcpy(nume, max->info.numeSofer);
        return nume;
    }
    else
    {
        return NULL;
    }
}

int main() {
    return 0;
}