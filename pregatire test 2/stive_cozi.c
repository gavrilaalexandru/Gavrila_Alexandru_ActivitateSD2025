#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// informatia ca si structura
struct StructuraMasina
{
    int id;
    int nrUsi;
    float pret;
    char* model;
    char* numeSofer;
    unsigned char serie;
};
typedef struct StructuraMasina Masina; // typedef (alias)

Masina citireMasinaDinFisier(FILE* f) {
    char buffer[100]; // ne luam un buffer
    char sep[3] = ",\n"; // lista cu separatori, acestia fiind virgula si newline
    char* aux; // ne luam un auxiliar pentru a putea citi atributele alocate dinamic
    fgets(buffer, 100, f); // citim tot in buffer
    
    Masina m1; // masina pe care o vom returna
    m1.id = atoi(strtok(buffer, sep));
    m1.nrUsi = atoi(strtok(NULL, sep));
    m1.pret = atof(strtok(NULL, sep));

    // citim atributele alocate dinamic
    aux = strtok(NULL, sep);
    m1.model = (char*)malloc(strlen(aux) + 1);
    strcpy(m1.model, aux);

    aux = strtok(NULL, sep);
    m1.numeSofer = (char*)malloc(strlen(aux) + 1);
    strcpy(m1.numeSofer, aux);

    // citim charul
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

// STACK (stiva) reprezentat prin LSI
struct Nod {
    Masina info;
    struct Nod* next;
};
typedef struct Nod Nod;

void pushStack(Nod** stiva, Masina masina) { // stiva retine mereu adresa varfului
    Nod* nou = (Nod*)malloc(sizeof(Nod)); // creem un nod nou
    nou->info = masina; // punem informatia in nod
    nou->next = (*stiva); // nodul urmator este varful curent
    (*stiva) = nou; // mutam stiva sa fie la adresa celui mai nou introdus nod (noul varf)
}

Masina popStack(Nod** stiva) {
    if ((*stiva) != NULL)
    {
        Masina nou = (*stiva)->info; // luam informatia din varful curent
        Nod* prev = (*stiva); // facem o copie a acestui nod
        (*stiva) = (*stiva)->next; // mutam varful 
        free(prev); // eliberam nodul
        return nou; // returnam masina/informatia
    }
    Masina m;
    m.id = -1;
    return m;
}

unsigned char emptyStack(Nod* stiva) {
    return stiva == NULL; // returneaza 1 sau 0
}

Nod* citireStackMasiniDinFisier(const char* numeFisier) {
    Nod* stiva = NULL;
    FILE* f = fopen(numeFisier, "r");
    while (!feof(f))
    {
        Masina m = citireMasinaDinFisier(f);
        pushStack(&stiva, m);
    }
    fclose(f);
    return stiva;
}

void dezalocareStiva(Nod** stiva) {
    while ((*stiva) != NULL)
    {
        Masina m = popStack(stiva); // pop pe rand si dupa free
        free(m.numeSofer);
        free(m.model);
    }
}

int stackSize(Nod* stiva) {
    int nr = 0;
    while (stiva != NULL)
    {
        nr++;
        stiva = stiva->next;
    }
    return nr; // numarul de elemente din stiva
}

// QUEUE (coada) reprezentata prin lista dubla
struct NodDublu {
    Masina info;
    struct NodDublu* next;
    struct NodDublu* prev;
};
typedef struct NodDublu NodDublu;

struct ListaDubla
{
    NodDublu* prim;
    NodDublu* ultim;
};
typedef struct ListaDubla ListaDubla;

void enqueue(ListaDubla* coada, Masina m) {
    NodDublu* nou = (NodDublu*)malloc(sizeof(NodDublu)); // creare nod nou
    nou->info = m; // punem informatia in nod
    nou->prev = coada->ultim; // nodul anterior al acestuia este ultimul nod actual din coada
    nou->next = NULL; // acesta va fi ultimul nod deci urmatoru este NULL
    if (coada->ultim != NULL)
    {
        coada->ultim->next = nou; // daca coada nu este goala, actualizam urmatorul nod al celui ultim
    }
    else {
        coada->prim = nou; // daca coada este goala noul nod va fi si primul si ultimul
    }
    coada->ultim = nou; // actualizam ultimul nod in coada care acum este nodul nou
}

// NEXT POINTEAZA CATRE URMATORUL NOD (DREAPTA SPRE SFARSITUL COZII)
// URM POINTEAZA CATRE NODUL ANTERIOR (STANGA CATRE INCEPUTUL COZII)

Masina dequeue(ListaDubla* coada) {
    Masina m;
    m.id = -1;
    if (coada->prim != NULL)
    {
        m = coada->prim->info;
        NodDublu* aux = coada->prim;
        if (coada->prim->next != NULL) // caz A cand mai sunt noduri in coada
        {
            coada->prim = coada->prim->next; // urmatorul nod devine primul
            coada->prim->prev = NULL; // nu mai are un nod catre inceputul cozii
        }
        else // Caz B cand nu mai exista noduri in coada
        {
            coada->prim = NULL; // coada e goala
            coada->ultim = NULL;
        }
        free(aux);
    }
    return m;
}

ListaDubla citireCoadaMasiniDinFisier(const char* numeFisier) {
    ListaDubla coada;
    coada.prim = NULL;
    coada.ultim = NULL;
    FILE* f = fopen(numeFisier, "r");
    while (!feof(f))
    {
        Masina m = citireMasinaDinFisier(f);
        enqueue(&coada, m);
    }
    fclose(f);
    return coada;
}

void dezalocareCoada(ListaDubla* coada) {
    NodDublu* aux = coada->ultim;
    while (aux)
    {
        NodDublu* temp = aux;
        aux = aux->prev;
        free(temp->info.model);
        free(temp->info.numeSofer);
        free(temp);
    }
    coada->prim = NULL;
    coada->ultim = NULL;
}

float calculeazaPretTotal(ListaDubla* coada) {
	float suma = 0;
	ListaDubla nou;
	nou.prim = NULL;
	nou.ultim = NULL;
	while ((*coada).prim) {
		Masina m = dequeue(coada);
		suma += m.pret;
		enqueue(&nou,m); // pastreaza lista intacta
	}
	coada->prim = nou.prim;
	coada->ultim = nou.ultim;

	return suma;
}

Masina getMasinaByID(Nod* stiva, int id) {
    Masina gasita;
    gasita.id = -1;

    Nod* curent = stiva;
    while (curent != NULL)
    {
        if (curent->info.id == id)
        {
            gasita = curent->info;
            return gasita;
        }
        curent = curent->next;
    }
    return gasita;
}


int main() {

}
