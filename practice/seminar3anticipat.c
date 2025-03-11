#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct StructuraMasina {
    int id;
    int nrUsi;
    float pret;
    char* model;
    char* numeSofer;
    unsigned char serie;
};
typedef struct StructuraMasina Masina; // typedef este un alias pt structura ca sa putem scrie direct numele nu si struct

void afisareMasina(Masina m) {
    printf("Id: %d\nNr usi: %d\nPret: %.2f\nModel: %s\nNume sofer: %s\nSeria masinii: %c\n", m.id, m.nrUsi, m.pret, m.model, m.numeSofer, m.serie);
} // unsigned char

void afisareVectorMasini(Masina* vector, int nrMasini) {
    for (int i = 0; i < nrMasini; i++)
    {
        afisareMasina(vector[i]);
    }
}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {
    // realloc(?) sau malloc din nou pentru a aloca spatiu in vector pentru inca o masina
    Masina* temp = realloc(*masini, ((*nrMasini) + 1) * sizeof(Masina));
    *masini = temp;
    (*masini)[*nrMasini] = masinaNoua;
    (*nrMasini)++; // sau incrementam nr de masini si facem (*masini)[(*nrMasini) - 1] = masinaNoua;
}

Masina citireMasinaFisier(FILE* f) {
    Masina masinaNoua;
    char bufferModel[50], bufferNumeSofer[50];
    if (f == NULL) {
        printf("Fisierul nu s-a putut deschide!");
        return masinaNoua; // nu stiu ce trebuie returnat
        // sau return; si atat
    }
    // trebuie alocata memorie pentru model si numeSofer
    // daca fscanf esueaza memoria alocata mai sus trebuie dezalocata

    if (fscanf(f, "%d,%d,%f,%s,%s,%c", &masinaNoua.id, &masinaNoua.nrUsi, &masinaNoua.pret, bufferModel, bufferNumeSofer, &masinaNoua.serie) == 6) {
        masinaNoua.model = strdup(bufferModel);
        masinaNoua.numeSofer = strdup(bufferNumeSofer);
    }
    else {
        masinaNoua.model = NULL;
        masinaNoua.numeSofer = NULL;
    }
    // fscanf(f, "%d,%d,%f,%s,%s,%c", &masinaNoua.id, &masinaNoua.nrUsi, &masinaNoua.pret, masinaNoua.model, masinaNoua.numeSofer, &masinaNoua.serie);
    // %dim_buf[^,] specificator pentru a citi corect un sir de caractere pana la virgula
    // %s citeste doar un singur cuvant, deoarece se opreste la primul whitespace (de ex din Dacia Logan va citi doar Dacia)
    return masinaNoua;
}

Masina* citireVectorMasini(const char* numeFisier, int* nrMasiniCitite) {
    FILE* fisier = fopen(numeFisier, "r");
    if (fisier == NULL) {
        printf("Fisierul nu s-a putut deschide");
        return NULL;
    }

    Masina* masini = NULL;
    *nrMasiniCitite = 0;

    // while(!feof(fisier)) {
    //     Masina masinaNoua = citireMasinaFisier(fisier);
    //     if (masinaNoua.id == -1) {
    //         adaugaMasinaInVector(&masini, nrMasiniCitite, masinaNoua);
    //     }
    // }

    // while(1) {
    //     Masina masinaNoua = citireMasinaFisier(fisier);
    //     if (masinaNoua.id == -1) {
    //         break;
    //     }
    //     adaugaMasinaInVector(&masini, nrMasiniCitite, masinaNoua); // aceasta functie incrementeaza nr de masini citite, deci nu mai trb. sa o facem manual
    // }

    while(!feof(fisier)) {
        Masina masinaNoua = citireMasinaFisier(fisier);
        adaugaMasinaInVector(&masini, nrMasiniCitite, masinaNoua);
    }

    fclose(fisier);
    return masini;
}

void dezalocare(Masina** vector, int* nrMasini) {
    for (int i = 0; i < *nrMasini; i++)
    {
        free((*vector)[i].model);
        free((*vector)[i].numeSofer);
    }
    free(*vector);
    *vector = NULL;
    *nrMasini = 0;
}

int main() {
    const char* numeFisier = "masiniSeminar3Anticipat.txt";
    int nrMasini = 0;
    Masina* masini = citireVectorMasini(numeFisier, &nrMasini);

    if (masini == NULL)
    {
        printf("Eroare la citirea masinilor din fisier in vector\n");
        return 1;
    }

    printf("\nMasinile citite din fisier in vector\n");
    afisareVectorMasini(masini, nrMasini);
    
    dezalocare(&masini, &nrMasini);
    return 0;
}