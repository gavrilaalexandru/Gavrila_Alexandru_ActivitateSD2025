#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Alexandru Gavrila -> A si G -> articolul Magazin

typedef struct StructMagazin {
    int nrAngajati;
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
    printf("Introduceti nr de angajati ai magazinului: ");
    scanf("%d", &m.nrAngajati);
    fflush(stdin);
    printf("Introduceti codul magazinului: ");
    scanf("%c", &m.codMagazin);
    fflush(stdin);
    printf("Introduceti numarul de produse ale magazinului: ");
    scanf("%d", &m.nrProduse);
    fflush(stdin);
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
    printf("Nr. angajati: %d\n", m.nrAngajati);
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

Magazin* copiazaElementeInVectorDupaNrAngajati(Magazin* m, int nrMagazine, int prag, int* dimensiune) {
    *dimensiune = 0;
    for (int i = 0; i < nrMagazine; i++)
    {
        if (m[i].nrAngajati > prag)
        {
            (*dimensiune)++;
        }
    }
    if (*dimensiune == 0)
    {
        return NULL;
    }
    Magazin* vectorNou = (Magazin*)malloc((*dimensiune) * sizeof(Magazin));
    int index = 0;
    for (int i = 0; i < nrMagazine; i++)
    {
        if(m[i].nrAngajati > prag) {
            vectorNou[index] = m[i];
            vectorNou[index].numeMagazin = (char*)malloc(strlen(m[i].numeMagazin) + 1 * sizeof(char));
            strcpy(vectorNou[index].numeMagazin, m[i].numeMagazin);
            vectorNou[index].preturi = (float*)malloc(m[i].nrProduse * sizeof(float));
            for (int j = 0; j < m[i].nrProduse; j++)
            {
                vectorNou[index].preturi[j] = m[i].preturi[j];
            }
            index++;
        }
    }
    return vectorNou;
}

Magazin* copiazaElementeInVectorDupaNrProduse(Magazin* m, int nrMagazine, int prag, int* dimensiune) {
    *dimensiune = 0;
    for (int i = 0; i < nrMagazine; i++)
    {
        if (m[i].nrProduse >= prag)
        {
            (*dimensiune)++;
        }
    }
    if (*dimensiune == 0)
    {
        return NULL;
    }
    Magazin* vectorNou = (Magazin*)malloc((*dimensiune) * sizeof(Magazin));
    int index = 0;
    for (int i = 0; i < nrMagazine; i++)
    {
        if(m[i].nrProduse >= prag) {
            vectorNou[index] = m[i];
            vectorNou[index].numeMagazin = (char*)malloc(strlen(m[i].numeMagazin) + 1 * sizeof(char));
            strcpy(vectorNou[index].numeMagazin, m[i].numeMagazin);
            vectorNou[index].preturi = (float*)malloc(m[i].nrProduse * sizeof(float));
            for (int j = 0; j < m[i].nrProduse; j++)
            {
                vectorNou[index].preturi[j] = m[i].preturi[j];
            }
            index++;
        }
    }
    return vectorNou;
}

Magazin* concateneazaVectori(Magazin* m1, int nrElemente1, Magazin* m2, int nrElemente2, int* dimensiuneFinala) {
    *dimensiuneFinala = nrElemente1 + nrElemente2;
    Magazin* vectorNou = (Magazin*)malloc(*dimensiuneFinala * sizeof(Magazin));
    memcpy(vectorNou, m1, nrElemente1 * sizeof(Magazin));
    memcpy(vectorNou + nrElemente1, m2, nrElemente2 * sizeof(Magazin));
    return vectorNou;
}

void afiseazaVector(Magazin* m, int nrMagazine) {
    for (int i = 0; i < nrMagazine; i++)
    {
        afisareMagazin(m[i]);
    }
}

void dezalocareVector(Magazin** m, int* nrMagazine) {
    for (int i = 0; i < *nrMagazine; i++)
    {
        free((*m)[i].numeMagazin);
        free((*m)[i].preturi);
    }
    free(*m);
    *m = NULL;
    *nrMagazine = 0;
}

Magazin citireMagazinFisier(FILE* file) {
    Magazin m;
    char buffer[100];
    char* aux;
    char separator[3] = ",\n";
    fgets(buffer, 100, file);
    
    aux = strtok(buffer, separator);
    m.numeMagazin = (char*)malloc(strlen(aux) + 1 * sizeof(char));
    strcpy(m.numeMagazin, aux);

    m.nrAngajati = atoi(strtok(NULL, separator));
    m.nrProduse = atoi(strtok(NULL, separator));

    m.preturi = (float*)malloc(m.nrProduse * sizeof(Magazin));

    for (int i = 0; i < m.nrProduse; i++)
    {
        m.preturi[i] = atof(strtok(NULL, separator));
    }

    m.codMagazin = *strtok(NULL, separator);
    return m;
}

void adaugaMagazinInVector(Magazin** magazine, int* nrMagazine, Magazin magazinNou) {
    Magazin* temp = (Magazin*)malloc(((*nrMagazine) + 1) * sizeof(Magazin));
    for (int i = 0; i < *nrMagazine; i++)
    {
        temp[i] = (*magazine)[i];
    }
    temp[*nrMagazine] = magazinNou;
    (*nrMagazine)++;
    free(*magazine);
    *magazine = temp;
}

Magazin* citireVectorMagazineFisier(const char* numeFisier, int* nrMagazineCitite) {
    FILE* file = fopen(numeFisier, "r");
    Magazin* magazine = NULL;
    do
    {
        adaugaMagazinInVector(&magazine, nrMagazineCitite, citireMagazinFisier(file));
    } while (!feof(file));
    fclose(file);

    return magazine;
}

void salvareMagazinInFisier(Magazin m, FILE* file) {
    fprintf(file, "%s,%d,%d", m.numeMagazin, m.nrAngajati, m.nrProduse);
    for (int i = 0; i < m.nrProduse; i++)
    {
        fprintf(file, "%.2f", m.preturi[i]);
    }
    fprintf(file, "%c\n", m.codMagazin);
}

void salvareVectorInFisier(Magazin* m, int nrMagazine, const char* numeFisier) {
    FILE* file = fopen(numeFisier, "w");
    for (int i = 0; i < nrMagazine; i++)
    {
        salvareMagazinInFisier(m[i], file);
    }
    fclose(file);
}

int main() {
    int nrMagazine = 0;
    Magazin* vectorMagazine = citireVectorMagazineFisier("magazineEx3.txt", &nrMagazine);
    afiseazaVector(vectorMagazine, nrMagazine);

    modificaNumeMagazin(&vectorMagazine[0], "CarrefourModificat");
    modificaNumeMagazin(&vectorMagazine[1], "Kaufland 2");
    salvareVectorInFisier(vectorMagazine, nrMagazine, "magazine_modificate.txt");
    dezalocareVector(&vectorMagazine, &nrMagazine);
    return 0;
}