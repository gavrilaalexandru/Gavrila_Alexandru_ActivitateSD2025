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

typedef struct NodSimplu {
    Masina info;
    struct NodSimplu* urmator;
} NodLS;

typedef struct Nod {
    Masina info;
    struct Nod* urmator;
    struct Nod* precedent;
} Nod;

typedef struct ListaDubla
{
    Nod* prim;
    Nod* ultim;
} Lista;


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

void afisareInceputListaMasini(Lista lista) {
    Nod* p = lista.prim;
    while (p)
    {
        afisareMasina(p->info);
        p = p->urmator;
    }
}

void afisareListaSimplaMasini(NodLS* lista) {
    while (lista)
    {
        afisareMasina(lista->info);
        lista = lista->urmator;
    }
}

void stergeElement(Lista* lista, unsigned int poz) {
    if (lista->prim == NULL)
    {
        printf("Lista este goala.");
        return;
    }
    int nrNoduri = 0;
    Nod* temp = lista->prim;

    while (temp != NULL && nrNoduri < poz)
    {
        nrNoduri++; // aflam nr de noduri
        temp = temp->urmator;
    }

    if (temp == NULL)
    {
        printf("Pozitia %d este in afara listei. Nu se realizeaza stergerea!", poz);
        return;
    }
    
    if (temp == lista->prim && temp == lista->ultim)
    {
        lista->prim = NULL;
        lista->ultim = NULL;
    }
    else if (temp == lista->prim)
    {
        lista->prim = temp->urmator;
        lista->prim->precedent = NULL;
    }
    else if (temp == lista->ultim)
    {
        lista->ultim = temp->precedent;
        lista->ultim->urmator = NULL;
    }
    else
    {
        temp->precedent->urmator = temp->urmator;
        temp->urmator->precedent = temp->precedent;
    }
    free(temp->info.model);
    free(temp->info.numeSofer);
    free(temp);
}

void adaugaElement(Lista* const lista, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->precedent = NULL;
    nou->urmator = NULL;

    if (lista->prim == NULL)
    {
        lista->prim = nou;
        lista->ultim = nou;
        return;
    }
    
    if (masinaNoua.pret < lista->prim->info.pret)
    {
        nou->urmator = lista->prim;
        lista->prim->precedent = nou;
        lista->prim = nou;
        // nou->precedent = NULL; dar fac asta deja la initializare
        return;
    }
    
    if (masinaNoua.pret >= lista->ultim->info.pret)
    {
        // nou->urmator = NULL;
        nou->precedent = lista->ultim;
        lista->ultim->urmator = nou;
        lista->ultim = nou;
        return;
    }
    
    Nod* temp = lista->prim;
    while (temp && temp->info.pret < masinaNoua.pret)
    {
        temp = temp->urmator;
    }
    nou->urmator = temp;
    nou->precedent = temp->precedent;

    if (temp->precedent) // trebuie aceasta verificare deoarece avem masini cu acelasi pret si aici se duce la inceput si nu intra in bucla while;
    // sau folosim <= la verificare in primul if
    {
        temp->precedent->urmator = nou;
    }
    else
    {
        lista->prim = nou;
    }
    temp->precedent = nou;
}

Lista citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
    Lista list;
    list.prim = NULL;
    list.ultim = NULL;

    while (!feof(f))
    {
        adaugaElement(&list, citireMasinaDinFisier(f));
    }
    fclose(f);
    return list;
}

void parcurgereCrescator(Lista lista) {
    Nod* temp = lista.prim;
    while (temp)
    {
        afisareMasina(temp->info);
        temp = temp->urmator;
    }
}

void parcurgereDescrescator(Lista lista) {
    Nod* temp = lista.ultim;
    while (temp)
    {
        afisareMasina(temp->info);
        temp = temp->precedent;
    }
}

NodLS* transformareDinLDinLS(Lista ld, float prag) {
    NodLS* rez = NULL;
    Nod* temp = ld.prim;

    while (temp)
    {
        if (temp->info.pret > prag)
        {
            NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
            nou->info.id = temp->info.id;
            nou->info.nrUsi = temp->info.nrUsi;
            nou->info.pret = temp->info.pret;
            nou->info.serie = temp->info.serie;

            nou->info.model = (char*)malloc(strlen(temp->info.model) + 1 * sizeof(char));
            strcpy(nou->info.model, temp->info.model);

            nou->info.numeSofer = (char*)malloc(strlen(temp->info.numeSofer) + 1 * sizeof(char));
            strcpy(nou->info.numeSofer, temp->info.numeSofer);

            nou->urmator = NULL; // noul nod va fi mereu ultimul

            if (rez == NULL)
            {
                rez = nou;
            }
            else
            {
                NodLS* p = rez;
                while (p->urmator)
                {
                    p = p->urmator;
                }
                p->urmator = nou;
            }
        }
        temp = temp->urmator;
    }
    return rez;
}

void interschimbare(Lista* lista, unsigned int poz1, unsigned int poz2) {
    if (lista->prim == NULL)
    {
        printf("Lista este goala!");
        return;
    }
    
    if (poz1 == poz2) {
        printf("Nu este nevoie de nicio interschimbare, pozitiile sunt identice");
        return;
    }

    Nod* temp = lista->prim, *nod1 = NULL, *nod2 = NULL;
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
        temp = temp->urmator;
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

void dezalocareListaMasini(NodLS** lista) {
    while(*lista) {
        NodLS* temp = *lista;
        *lista = (*lista)->urmator; // pt. ca ne ducem mai in fata un element si il stergem pe cel din urma
        free(temp->info.model);
        free(temp->info.numeSofer);
        free(temp); // nu avem de ce sa setam temp si lista la NULL
    }
}

int main() {
    Lista lista = citireLDMasiniDinFisier("masini.txt");
    printf("Lista initiala: \n\n");
    afisareInceputListaMasini(lista);

    printf("Lista dupa interschimbarea primelor 2 pozitii (0 si 1): \n\n");
    interschimbare(&lista, 0, 1);
    afisareInceputListaMasini(lista);

    printf("Lista dupa stergere: \n\n");
    stergeElement(&lista, 4);
    afisareInceputListaMasini(lista);

    printf("Lista simpla dupa transformare din lista dubla: \n\n");
    NodLS* listaSimpla = transformareDinLDinLS(lista, 5000);
    afisareListaSimplaMasini(listaSimpla);


    dezalocareLDMasini(&lista);
    dezalocareListaMasini(&listaSimpla);
    return 0;
}