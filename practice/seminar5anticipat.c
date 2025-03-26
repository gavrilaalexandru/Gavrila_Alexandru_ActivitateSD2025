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
//creare structura pentru Lista Dubla 

typedef struct Nod Nod;
struct Nod {
    Masina masina;
    Nod* urmator;
    Nod* anterior;
};

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

void afisareListaMasini(Nod* lista) {
    // Nod* nou = lista; pointer auxiliar pentru claritate
    while (lista)
    {
        afisareMasina(lista->masina);
        lista = lista->urmator;
    }
}

void adaugaMasinaInLista(Nod** cap, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->masina = masinaNoua;
    nou->urmator = NULL;
    if (*cap == NULL)
    {
        nou->anterior = NULL;
        *cap = nou;
    }
    else
    {
        Nod* p = *cap;
        while (p->urmator)
        {
            p = p->urmator;
        }
        p->urmator = nou;
        nou->anterior = p;
    }
}

void adaugaLaInceputInLista(Nod** cap, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->masina = masinaNoua;
    nou->urmator = *cap;
    if (*cap)
    {
        (*cap)->anterior = nou;
    }
    nou->anterior = NULL;
    *cap = nou;
}

// void* si cast in main
Nod* citireLDMasiniDinFisier(const char* numeFisier) {
    FILE* file = fopen(numeFisier, "r");
    Nod* lista = NULL;
    while (!feof(file))
    {
        adaugaMasinaInLista(&lista, citireMasinaDinFisier(file));
    }
    fclose(file);
    return lista;
}

void dezalocareLDMasini(Nod** lista) {
    while (*lista)
    {
        Nod* temp = *lista;
        if ((*lista)->urmator)
        {
            (*lista)->urmator->anterior = NULL;
        } // daca exista urmatorul nod ii setaz nodul anterior(cel pe care il stergem acum) la NULL
        
        *lista = (*lista)->urmator;
        free(temp->masina.numeSofer);
        free(temp->masina.model);
        free(temp);
    }
}

float calculeazaPretMediu(Nod* lista) {
    if (!lista)
    {
        return 0;
    }
    int index = 0;
    int suma = 0;

    while (lista)
    {
        suma += lista->masina.pret;
        index++;
        lista = lista->urmator;
    }
    return suma / index;
}

void stergeMasinaDupaID(Nod** lista, int id) {
    Nod* temp = *lista;

    while (temp)
    {
        if (temp->masina.id == id)
        {
            if (temp->anterior == NULL)
            {
                *lista = temp->urmator;
                if (*lista) {
                    (*lista)->anterior = NULL; // daca lista nu e goala, setam anterior la NULL
                }
            }
            else if (temp->urmator == NULL)
            {
                temp->anterior->urmator = NULL;
            }
            else
            {
                temp->anterior->urmator = temp->urmator; // sarim efectiv peste nod;
                temp->urmator->anterior = temp->anterior; // setam nodul anterior sa fie legat de nodul urmator celui curent; si nodul urmator sa fie legat de nodul anterior celui curent
            }
            free(temp->masina.model);
            free(temp->masina.numeSofer);
            free(temp);
            // return daca ar fi un singur nod
        }
        temp = temp->urmator;
    }
}

char* getNumeSoferMasinaScumpa(Nod* lista) {
    float pretMax = 0;
    char* numeCautat;
    while (lista)
    {
        if (lista->masina.pret > pretMax)
        {
            pretMax = lista->masina.pret;
            numeCautat = lista->masina.numeSofer;
        }
        lista = lista->urmator;
    }
    if (pretMax > 0)
    {
        char* numeSofer = (char*)malloc(strlen(numeCautat) + 1 * sizeof(char));
        strcpy(numeSofer, numeCautat);
        return numeSofer;
    }
	return NULL;
}

int main() {
    Nod* lista = NULL;
    lista = citireLDMasiniDinFisier("masiniSeminar5anticipat.txt");
    afisareListaMasini(lista);

    printf("Pretul mediu este: %.2f\n\n", calculeazaPretMediu(lista));

    stergeMasinaDupaID(&lista, 2);
    afisareListaMasini(lista);

    char* numeSoferMasinaScumpa = getNumeSoferMasinaScumpa(lista);
    printf("Cea mai scumpa masina ii apartine soferului: %s\n\n", numeSoferMasinaScumpa);
    if (numeSoferMasinaScumpa)
    {
        free(numeSoferMasinaScumpa);
    }

    dezalocareLDMasini(&lista);
	return 0;
}