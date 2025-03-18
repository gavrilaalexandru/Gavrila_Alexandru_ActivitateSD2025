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

//creare structura pentru un nod dintr-o lista simplu inlantuita
struct Nod {
    Masina masina;
    struct Nod* next;
};
typedef struct Nod Nod;

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

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(Nod* listaMasini) {
    // nu stim numarul de pasi deci folosim while(?)
    while (listaMasini)
    {
        afisareMasina(listaMasini->masina);
        listaMasini = listaMasini->next;
    }
}

void adaugaMasinaInLista(Nod** listaMasini, Masina masinaNoua) { // trebuie trimis prin adresa pt. modificarea listei DOAR in cazul in care este goala
    Nod* nodNou = (Nod*)malloc(sizeof(Nod));
    nodNou->masina = masinaNoua;
    nodNou->next = NULL;

    if (*listaMasini == NULL) // daca lista este goala
    {
        *listaMasini = nodNou;
    }
    else
    {
        Nod* temp = *listaMasini; // folosim temp ca un cursor pentru a accesa ultimul element; daca am folosi lista initiala am pierde referinta la primul element
        while (temp->next) 
        {
            temp = temp->next;
        }
        temp->next = nodNou;
    }
}

void adaugaLaInceputInLista(Nod** listaMasini, Masina masinaNoua) {
	//adauga la inceputul listei o noua masina pe care o primim ca parametru
    Nod* nodNou = (Nod*)malloc(sizeof(Nod));
    nodNou->masina = masinaNoua;
    nodNou->next = *listaMasini;
    *listaMasini = nodNou;
}

void* citireListaMasiniDinFisier(const char* numeFisier) {
    FILE* file = fopen(numeFisier, "r");
    Nod* lista = NULL;

    while (!feof(file))
    {
        adaugaMasinaInLista(&lista, citireMasinaDinFisier(file));
    }

    // do
    // {
    //     Masina m = citireMasinaDinFisier(file);
    //     adaugaMasinaInLista(&lista, m);
    // } while (!feof(file));
    
    fclose(file);
    return lista;
}

void dezalocareListaMasini(Nod** lista) {
    while(*lista) {
        Nod* temp = *lista;
        *lista = (*lista)->next; // trecem la urmatorul element din lista
        free(temp->masina.model);
        free(temp->masina.numeSofer);
        free(temp);
        temp = NULL;
    }
    *lista = NULL;
}

float calculeazaPretMediu(Nod* lista) {
    if (!lista)
    {
        return 0;
    }
    int index = 0;
    float suma = 0;

    while (lista)
    {
        suma += lista->masina.pret;
        index++;
        lista = lista->next;
    }
    return (index > 0) ? (suma / index) : 0;
}

void stergeMasiniDinSeria(Nod** lista, const char* serieCautata) {
    Nod* temp = *lista;
    Nod* prev = NULL;

    while (temp)
    {
        if (temp->masina.serie == *serieCautata)
        {
            if (prev == NULL)
            {
                *lista = temp->next;
            }
            else 
            {
                prev->next = temp->next;
            }
            free(temp->masina.model);
            free(temp->masina.numeSofer);
            free(temp);
            temp = (prev) ? prev->next : *lista;
            // A -> B -> C -> D; A este prev si B este temp --> prev->next = B si temp->next = C
            // prev->next = temp->next => prev->next = C => sarim efectiv peste B si lista devine A -> C -> D
            // temp = prev->next => temp = C si prev = A
        }
        else
        {
            prev = temp;
            temp = temp->next;
        }
    }
}

float calculeazaPretulMasinilorUnuiSofer(Nod* lista, const char* numeSofer) {
    float suma = 0;
    while (lista)
    {
        if (strcmp(lista->masina.numeSofer, numeSofer) == 0)
        {
            suma += lista->masina.pret;
        }
        lista = lista->next;
    }
	return suma;
}

int main() {
    Nod* lista = citireListaMasiniDinFisier("masiniSeminar3Anticipat.txt");
    printf("Lista de masini:\n");
    afisareListaMasini(lista);

    // float medie = calculeazaPretMediu(lista);
    printf("Pretul mediu este: %.2f\n\n", calculeazaPretMediu(lista));

    const char* numeSofer = "Ionescu";
    printf("Pretul total al masinilor pentru soferii cu numele %s este %.2f\n\n", numeSofer, calculeazaPretulMasinilorUnuiSofer(lista, numeSofer));

    const char* serie = "J";
    printf("Stergere masini cu seria %s:\n", serie);
    stergeMasiniDinSeria(&lista, serie);
    afisareListaMasini(lista);

    dezalocareListaMasini(&lista);
	return 0;
}