#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct StructuraTelefon {
    int id;
    float diagonala;
    float pret;
    char* producator;
    char* model;
    unsigned char imei;
} Telefon;

typedef struct Nod Nod;
struct Nod {
    Telefon info;
    Nod* urm;
};

Telefon citireTelefonDinFisier(FILE* file) {
    Telefon t;
    char buffer[100];
    char* bufferString;
    char separator[3] = ",\n";
    fgets(buffer, 100, file);
    t.id = atoi(strtok(buffer, separator));
    t.diagonala = atof(strtok(NULL, separator));
    t.pret = atof(strtok(NULL, separator));

    bufferString = strtok(NULL, separator);
    t.producator = (char*)malloc(strlen(bufferString) + 1 * sizeof(char));
    strcpy(t.producator, bufferString);

    bufferString = strtok(NULL, separator);
    t.model = (char*)malloc(strlen(bufferString) + 1 * sizeof(char));
    strcpy(t.model, bufferString);

    t.imei = *strtok(NULL, separator);
    // sau t.imei = strtok(NULL, separator)[0];

    return t;
}

void afisareTelefon(Telefon t) {
    printf("Id: %d\n", t.id);
    printf("Diagonala: %.1f\n", t.diagonala);
    printf("Pret: %.2f\n", t.pret);
    printf("Producator: %s\n", t.producator);
    printf("Model: %s\n", t.model);
    printf("IMEI: %c\n\n", t.imei);
}

void afisareListaTelefoane(Nod* lista) {
    while (lista)
    {
        afisareTelefon(lista->info);
        lista = lista->urm;
    }
}

void adaugaTelefonInLista(Nod** lista, Telefon telefonNou) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = telefonNou;
    nou->urm = NULL;

    if (*lista == NULL)
    {
        *lista = nou;
    }
    else
    {
        Nod* temp = *lista;
        while(temp->urm)
        {
            temp = temp->urm;
        }
        temp->urm = nou;
    }
}

void adaugaLaInceputInLista(Nod** lista, Telefon telefonNou) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = telefonNou;
    nou->urm = *lista;
    *lista = nou;
}

Nod* citireListaTelefoaneDinFisier(const char* numeFisier) {
    FILE* file = fopen(numeFisier, "r");
    Nod* lista = NULL;

    while (!feof(file))
    {
        adaugaTelefonInLista(&lista, citireTelefonDinFisier(file));
    }
    fclose(file);
    return lista;
}

void dezalocareListaMasini(Nod** lista) {
    while (*lista)
    {
        Nod* temp = *lista;
        *lista = (*lista)->urm;
        free(temp->info.model);
        free(temp->info.producator);
        free(temp);
    }
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
        suma += lista->info.pret;
        index++;
        lista = lista->urm;
    }
    return suma / index;
}

float calculeazaPretulTelefoanelorCuUnModelAnume(Nod* lista, const char* numeModel) {
    float suma = 0;
    while (lista)
    {
        if (strcmp(lista->info.model, numeModel) == 0)
        {
            suma += lista->info.pret;
        }
        lista = lista->urm;
    }
    return suma;
}

char* getCelMaiScumpTelefon(Nod* lista) {
    float pretMax = 0;
    char* modelCautat;
    while (lista)
    {
        if (lista->info.pret > pretMax)
        {
            pretMax = lista->info.pret;
            modelCautat = lista->info.model;
        }
        lista = lista->urm;
    }
    if (pretMax > 0)
    {
        char* modelFinal = (char*)malloc(strlen(modelCautat) + 1 * sizeof(char));
        strcpy(modelFinal, modelCautat);
        return modelFinal;
    }
    return NULL;
}

void stergeTelefonulCuUnAnumitImei(Nod** lista, char imeiCautat) {
    Nod* temp = *lista;
    Nod* prev = NULL;

    while (temp)
    {
        if (temp->info.imei == imeiCautat)
        {
            if (prev == NULL) // daca e prima masina din lista
            {
                *lista = temp->urm;
            }
            else
            {
                prev->urm = temp->urm;
            }
            free(temp->info.model);
            free(temp->info.producator);
            free(temp);
            temp = (prev) ? prev->urm : *lista;
        }
        else
        {
            prev = temp;
            temp = temp->urm;
        }
    }
}

int main() {
    Nod* lista = NULL;
    lista = citireListaTelefoaneDinFisier("telefoaneSeminar4practice.txt");
    afisareListaTelefoane(lista);

    printf("Pretul mediu este: %.2f\n\n", calculeazaPretMediu(lista));

    const char* numeModel = "Iphone 14";
    printf("Pretul total al telefoanelor cu modelul %s este: %.2f\n\n", numeModel, calculeazaPretulTelefoanelorCuUnModelAnume(lista, numeModel));

    char* model = getCelMaiScumpTelefon(lista);
    printf("Cel mai scump telefon este: %s\n", model);
    if (model)
    {
        free(model); // este safe sa dam free(model) si fara verificare, chiar daca este NULL
    }
    
    dezalocareListaMasini(&lista);
    return 0;
}