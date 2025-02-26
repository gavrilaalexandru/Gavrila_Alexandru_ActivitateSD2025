#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Masina {
    int id; // 4
    int nrLocuri; // 4
    char* marca; // 8
    float capacitateC; // 4
    char normaPoluare; // 1
};

struct Masina initializare(int id, int nrLocuri, const char* marca, float capacitateC, char normaPoluare)
{
    struct Masina m;
    m.id = id;
    m.nrLocuri = nrLocuri;

    m.marca = (char*)malloc(strlen(marca) + 1);
    //strcpy_s(m.marca, strlen(marca)+1 ,marca);
    strcpy(m.marca, marca);

    m.capacitateC = capacitateC;
    m.normaPoluare = normaPoluare;
    return m;
}

void afisare(struct Masina m) {
    printf("ID masina: %d\nNr Locuri: %d\nCapacitate: %f\nMarca: %s\nNorma Poluare: Euro %c\n", m.id, m.nrLocuri, m.capacitateC, m.marca, m.normaPoluare);
}

void modifica_nrLocuri(struct Masina* m, int nrLocuriNoi) {
    if (nrLocuriNoi > 0)
    {
        m->nrLocuri = nrLocuriNoi;
    }
}

void dezalocare(struct Masina* m) {
    //dezalocare campuri alocate dinamic
    free(m->marca);
    // (*m).marca = NULL;
    m->marca = NULL;
}

int main() {
    struct Masina masina; //in C trebuie precizat ca e structura
    masina = initializare(1, 5, "Dacia", 1.5, '4');
    afisare(masina);
    modifica_nrLocuri(&masina, 4);
    afisare(masina);
    dezalocare(&masina);
    afisare(masina);
    printf("%lu", (unsigned long)sizeof(masina));

    return 0;
}