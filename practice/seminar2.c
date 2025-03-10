#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Carte {
    int id;
    int nrPagini;
    char* titlu;
};

struct Carte initializare(int id, int nrPagini, const char* titlu) {
    struct Carte c;
    c.id = id;
    c.nrPagini = nrPagini;
    
    if (titlu == NULL) {
        c.titlu = NULL;
    }
    else {
        c.titlu = (char*)malloc(strlen(titlu) + 1);
        strcpy(c.titlu, titlu);
    }

    return c;
}

void afisare(struct Carte c) {
    printf("Id-ul cartii: %d\nNr. de pagini: %d\nTitlul cartii: %s\n", c.id, c.nrPagini, c.titlu);
}

void afisareVector(struct Carte* vector, int nrElemente) {
    for (int i = 0; i < nrElemente; i++)
    {
        afisare(vector[i]);
    }
}

struct Carte* copiazaPrimeleCarti(struct Carte* vector, int nrElemente, int nrElementeCopiate) {
    if (nrElementeCopiate > nrElemente) {
        nrElementeCopiate = nrElemente; // probabil mai bine transmis prin valoarea pentru a putea fi modificat
    }

    struct Carte* vectorNou = NULL;
    vectorNou = malloc(nrElementeCopiate * sizeof(struct Carte));

    for (int i = 0; i < nrElementeCopiate; i++)
    {
        vectorNou[i] = initializare(vector[i].id, vector[i].nrPagini, vector[i].titlu);
        // vectorNou[i] = vector[i];
        // vectorNou[i].titlu = (char*)malloc(strlen(vector[i].titlu) + 1);
        // strcpy(vectorNou[i].titlu, vector[i].titlu);
    }
    return vectorNou;
}

void dezalocare(struct Carte** vector, int* nrElemente) {
    for (int i = 0; i < *nrElemente; i++)
    {
        free((*vector)[i].titlu);
    }
    free(*vector);
    *vector = NULL;
    *nrElemente = 0;
}

void copiazaCartiCuNrPaginiMare(struct Carte* vector, char nrElemente, int prag, struct Carte** vectorNou, int* dimensiune) {
    *dimensiune = 0;
    for (char i = 0; i < nrElemente; i++)
    {
        if (vector[i].nrPagini > prag)
        {
            (*dimensiune)++;
        }
    }

    if (*dimensiune == 0) {
        *vectorNou = NULL;
        return;
    }

    *vectorNou = (struct Carte*)malloc((*dimensiune) * sizeof(struct Carte));
    char index = 0;
    
    for (char i = 0; i < nrElemente; i++)
    {
        if (vector[i].nrPagini > prag)
        {
            (*vectorNou)[index] = vector[i];
            (*vectorNou)[index].titlu = (char*)malloc(strlen(vector[i].titlu) + 1);
            strcpy((*vectorNou)[index].titlu, vector[i].titlu);
            index++;
        }
    }
}

struct Carte getPrimaCarteDupaTitlu(struct Carte* vector, int nrElemente, const char* conditie) {
    for (int i = 0; i < nrElemente; i++)
    {
        if (strcmp(vector[i].titlu, conditie) == 0)
        {
            return vector[i]; // shallow copy; cand se eliberea memorie din vectorul initial se dezaloca si aici, pointand la aceeasi adresa
        }
    }
    return initializare(-1, 0, NULL);
}

int main() {
    // struct Carte c1;
    // c1 = initializare(1, 50, "Abecedar");
    // afisare(c1);

    int nrCarti = 4;
    struct Carte* vectorCarti = (struct Carte*)malloc(nrCarti * sizeof(struct Carte));
    vectorCarti[0] = initializare(1, 75, "Vrajitorul din Oz");
    vectorCarti[1] = initializare(2, 60, "Jocurile Foamei");
    vectorCarti[2] = initializare(3, 110, "The Art of Mac Malware Volume 1");
    vectorCarti[3] = initializare(4, 105, "Vrajitorul din Oz");

    printf("\nAfisare vector initial\n");
    printf("----------------------\n");
    afisareVector(vectorCarti, nrCarti);

    struct Carte* vectorCartiCopiate;
    int nrCartiCopiate = 2; // daca aici punem un nr mai mare decat nr de carti 
    // in vectorul initial, functia care copiaza merge corect
    // dar crapa functia de afisare.
    // probabil e bine sa il transmitem prin adresa si sa il modificam
    // daca este mai mare decat nr. de elemente 
    // din vectorul initial 
    // (*nrElementeCopiate > nrElemente --> *nrElementeCopiate = nrElemente)

    vectorCartiCopiate = copiazaPrimeleCarti(vectorCarti, nrCarti, nrCartiCopiate);
    printf("\nAfisare vector copiat\n");
    printf("---------------------\n");
    afisareVector(vectorCartiCopiate, nrCartiCopiate);

    struct Carte* vectorCartiCuPaginiPestePrag;
    int dimensiune = 0; // este setata la 0 oricum la inceputul functiei
    int prag = 100; // pagini
    copiazaCartiCuNrPaginiMare(vectorCarti, nrCarti, prag, &vectorCartiCuPaginiPestePrag, &dimensiune);

    printf("\nAfisare vector copiat peste prag\n");
    printf("----------------------------------\n");
    afisareVector(vectorCartiCuPaginiPestePrag, dimensiune);

    const char* conditie = "Vrajitorul din Oz";
    struct Carte carteJocurileFoamei = getPrimaCarteDupaTitlu(vectorCarti, nrCarti, conditie); // nu trebuie dezalocata

    printf("\nAfisare carte cautata\n");
    printf("-----------------------\n");
    afisare(carteJocurileFoamei);

    dezalocare(&vectorCartiCuPaginiPestePrag, &dimensiune);
    dezalocare(&vectorCartiCopiate, &nrCartiCopiate);
    dezalocare(&vectorCarti, &nrCarti);

    return 0;
}