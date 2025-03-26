#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru un nod dintr-o lista dublu inlantuita
struct Nod {
    Masina info;
    struct Nod* urmator;
    struct Nod* precedent;
};
typedef struct Nod Nod;

//creare structura pentru Lista Dubla 
struct ListaDubla
{
    Nod* prim;
    Nod* ultim;
};
typedef struct ListaDubla Lista;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy(m1.model, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);

	m1.serie = *strtok(NULL, sep);
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

void afisareInceputListaMasini(Lista lista) {
    Nod* p = lista.prim;
    while (p)
    {
        afisareMasina(p->info);
        p = p->urmator;
    }
}

void afisareSfarsitListaMasini(Lista lista) {
    Nod* p = lista.ultim;
    while (p)
    {
        afisareMasina(p->info);
        p = p->precedent;
    }
}

void adaugaMasinaInLista(Lista* const lista, Masina masinaNoua) { // se recomanda prin pointer constant
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->urmator = NULL;
    nou->precedent = lista->ultim;
    if (lista->ultim) // daca exista ultimul nod
    {
        lista->ultim->urmator = nou;
    }
    else
    {
        lista->prim = nou;
    }
    lista->ultim = nou;
}

void adaugaLaInceputInLista(Lista* list, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->urmator = list->prim;
    if ((*list).prim)
    {
        list->prim->precedent = nou;
    }
    else
    {
        list->ultim = nou;
    }
    nou->precedent = NULL;
    list->prim = nou;
}

Lista citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
    Lista list;
    list.prim = NULL;
    list.ultim = NULL;

    while (!feof(f))
    {
        // adaugaMasinaInLista(&list, citireMasinaDinFisier(f));
        adaugaLaInceputInLista(&list, citireMasinaDinFisier(f));
    }
    fclose(f);
    return list;
}

void dezalocareLDMasini(Lista* list) {
    Nod* p = list->prim;
    while (p->urmator)
    {
        free(p->info.numeSofer);
        free(p->info.model);
        p = p->urmator;
        free(p->precedent);
    }
    free(p->info.numeSofer);
    free(p->info.model);
    free(p);
    list->prim = NULL;
    list->ultim = NULL;
}

float calculeazaPretMediu(Lista list) {
    Nod* aux = list.prim;
    int suma = 0;
    int index = 0;

    while (aux)
    {
        suma += aux->info.pret;
        index++;
        aux = aux->urmator;
    }
	return suma / index;
}

void stergeMasinaDupaID(/*lista masini*/ int id) {
	//sterge masina cu id-ul primit.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
}

char* getNumeSoferMasinaScumpa(/*lista dublu inlantuita*/) {
	//cauta masina cea mai scumpa si 
	//returneaza numele soferului acestei maasini.
	return NULL;
}

int main() {
    Lista list;
    list = citireLDMasiniDinFisier("masini.txt");
    // afisareInceputListaMasini(list);
    afisareSfarsitListaMasini(list);

    printf("Pretul mediu este: %.2f", calculeazaPretMediu(list));

    dezalocareLDMasini(&list);
	return 0;
}