#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Alexandru Gavrila -> A si G -> articolul Magazin

typedef struct StructMagazin {
    int nrProduse;
    float* preturi;
    char* numeMagazin;
    char codMagazin;
} Magazin;

Magazin citireMagazinTastatura() {
    Magazin m;
    char buff[50];
    printf("Introduceti numele magazinului: ");
    scanf("%s", buff); // sau fgets
    m.numeMagazin = (char*)malloc(strlen(buff) + 1 * sizeof(char));
    strcpy(m.numeMagazin, buff);
    fflush(stdin);
    printf("Introduceti codul magazinului: ");
    scanf("%c", &m.codMagazin);
    printf("Introduceti numarul de produse ale magazinului: ");
    scanf("%d", &m.nrProduse);
    m.preturi = (float*)malloc(m.nrProduse * sizeof(float));
    for (int i = 0; i < m.nrProduse; i++)
    {
        printf("Introduceti pretul produsului %d: ", i + 1);
        scanf("%f", &m.preturi[i]);
    }
    printf("\n");
    return m;
}

void afisareMagazin(Magazin m) {
    printf("Nume magazin: %s\n", m.numeMagazin);
    printf("Cod magazin: %c\n", m.codMagazin);
    printf("Numar produse magazin: %d\n", m.nrProduse);
    for (int i = 0; i < m.nrProduse; i++)
    {
        printf("Pretul produsului %d este %.2f\n", i + 1, m.preturi[i]);
    }
    printf("\n");
}

float calculPretMediu(Magazin m) {
    float suma;
    for (int i = 0; i < m.nrProduse; i++)
    {
        suma += m.preturi[i];
    }
    if (m.nrProduse > 0) {
        return suma / m.nrProduse;
    }
    return 0;
}

void modificaNumeMagazin(Magazin* m, const char* numeNou ) {
    if (m && numeNou) // daca exista m si numeNou
    {
        m->numeMagazin = realloc(m->numeMagazin, strlen(numeNou) + 1);
        strcpy(m->numeMagazin, numeNou);
    }
}

void dezalocare(Magazin* m) {
    free(m->numeMagazin);
    free(m->preturi);
    m->numeMagazin = NULL;
    m->preturi = NULL;
    m->nrProduse = 0;
}

int main() {
    Magazin m = citireMagazinTastatura();
    afisareMagazin(m);
    modificaNumeMagazin(&m, "Carrefour");
    afisareMagazin(m);
    float medie = calculPretMediu(m);
    printf("Media produselor este: %f", medie);
    dezalocare(&m);
    return 0;
}
