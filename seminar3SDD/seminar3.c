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

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
    printf("Nr usi: %d\n", masina.nrUsi);
    printf("Pret: %.2f\n", masina.pret);
    printf("Model: %s\n", masina.model);
    printf("Numar sofer: %s\n", masina.numeSofer);
    printf("Serie: %c\n\n", masina.serie);
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
    for (int i = 0; i < nrMasini; i++)
    {
        afisareMasina(masini[i]);
    }
}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {
    Masina* temp = (Masina*)malloc(((*nrMasini) + 1) * sizeof(Masina));
    for (int i = 0; i < *nrMasini; i++)
    {
        temp[i] = (*masini)[i];
    }
    temp[*nrMasini] = masinaNoua; // aici merge si deep copy si shallow copy
    (*nrMasini)++;

    free(*masini);
    *masini = temp;
}

Masina citireMasinaFisier(FILE* file) {
    Masina m;
    char buffer[100];
    char* aux;
    char sep[3] = ",\n";
    fgets(buffer, 100, file);
    aux = strtok(buffer, sep);
    m.id = atoi(aux);

    m.nrUsi = atoi(strtok(NULL, sep)); // NULL ca sa continue de unde a ramas strtok
    m.pret = atof(strtok(NULL, sep));

    aux = strtok(NULL, sep);
    m.model = (char*)malloc(strlen(aux) + 1 * sizeof(char));
    strcpy(m.model, aux);

    aux = strtok(NULL, sep);
    m.numeSofer = (char*)malloc(strlen(aux) + 1 * sizeof(char));
    strcpy(m.numeSofer, aux);
    
    m.serie = *strtok(NULL, sep); // cu indexare[0]

    return m;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
    FILE* file = fopen(numeFisier, "r");
    Masina* masini = NULL;
    do
    {
        adaugaMasinaInVector(&masini, nrMasiniCitite, citireMasinaFisier(file));
    } while (!feof(file));
    
    fclose(file);
    return masini;
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	for (int i = 0; i < *nrMasini; i++)
    {
        free((*vector)[i].model);
        free((*vector)[i].numeSofer);
    }
    free(*vector);
    *vector = NULL;
    *nrMasini = 0;
}

float pretMediuDupaNrUsi(Masina* vector, int nrMasini, int nrUsi) {
    float suma = 0;
    int index = 0;
    for (int i = 0; i < nrMasini; i++)
    {
        if (vector[i].nrUsi == nrUsi) {
            suma += vector[i].pret;
            index++;
        }
    }
    if (index > 0) {
        return suma / index;
    }
    
    return 0;
}

int main() {
    Masina* masini = NULL;
    int nrMasini = 0;
    masini = citireVectorMasiniFisier("masini.txt", &nrMasini);
    afisareVectorMasini(masini, nrMasini);
    float medie = pretMediuDupaNrUsi(masini, nrMasini, 5);
    printf("Pretul mediu al masinilor este: %.2f", medie);
    dezalocareVectorMasini(&masini, &nrMasini);
	return 0;
}