#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// chaining

typedef struct StructuraCladire {
    int id;
    int anulConstruirii;
    float suprafata;
    char* adresa;
} Cladire;

typedef struct Nod {
    Cladire info;
    struct Nod* urm;
} Nod;

typedef struct HashTable {
    int dim;
    Nod** v;
} HashTable;


Cladire citireCladireDinFisier(FILE* file) {
    char buffer[5000];
    char sep[3] = ",\n";
    char* aux;
    fgets(buffer, 5000, file);
    Cladire c1;
    aux = strtok(buffer, sep);
    c1.id = atoi(aux);
    c1.anulConstruirii = atoi(strtok(NULL, sep));
    aux = strtok(NULL, sep);
    c1.adresa = (char*)malloc(strlen(aux) + 1);
    strcpy(c1.adresa, aux);
    c1.suprafata = atof(strtok(NULL, sep));
    return c1;
}

void afisareCladire(Cladire c1) {
    printf("Id: %d\n", c1.id);
    printf("An construire: %d\n", c1.anulConstruirii);
    printf("Adresa: %s\n", c1.adresa);
    printf("Suprafata: %f\n", c1.suprafata);
}

void afisareListaCladiri(Nod* lista) {
    while (lista)
    {
        afisareCladire(lista->info);
        lista = lista->urm;
    }
}

void adaugaCladireInLista(Nod* lista, Cladire cladireNoua) {
    if (lista == NULL) {
        return;
    }
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = cladireNoua;
    nou->urm = NULL;

    while (lista->urm) {
        lista = lista->urm;
    }
    lista->urm = nou;
}

HashTable initializareHashTable(int dim) {
    HashTable ht;
    ht.dim = dim;
    ht.v = (Nod**)malloc(sizeof(Nod*) * dim);
    for (int i = 0; i < dim; i++)
    {
        ht.v[i] = NULL;
    }
    return ht;
}

int calculeazaHash(int an, int dim) {
    if (dim == 0)
    {
        return 0;
    }
    return an % dim;
}

void inserareCladireInTabela(HashTable ht, Cladire cladire) {
    int poz = calculeazaHash(cladire.anulConstruirii, ht.dim);

    if (ht.v[poz] == NULL)
    {
        Nod* nou = (Nod*)malloc(sizeof(Nod));
        nou->info = cladire;
        nou->urm = NULL;
        ht.v[poz] = nou;
    }
    else
    {
        adaugaCladireInLista(ht.v[poz], cladire);
    }
}

HashTable citireCladiriDinFisier(const char* numeFisier, int dim) {
    FILE* file = fopen(numeFisier, "r");
    HashTable ht = initializareHashTable(dim);

    while (!feof(file))
    {
        inserareCladireInTabela(ht, citireCladireDinFisier(file));
    }
    fclose(file);
    return ht;
}

void afisareTabelaMasini(HashTable ht) {
    for (int i = 0; i < ht.dim; i++)
    {
        printf("\nCluster %d:\n", i + 1);
        afisareListaCladiri(ht.v[i]);
    }
}

void stergeCladireDupaAnSiId(HashTable ht, int id, int an) { // stim clusterul
    int hash = calculeazaHash(an, ht.dim);

    if (ht.v[hash] == NULL) {
        printf("Nu s-a gasit nicio cladire cu anul construirii %d\n", an);
    }

    if (ht.v[hash]->info.id == id && ht.v[hash]->info.anulConstruirii == an)
    {
        Nod* deSters = ht.v[hash];
        ht.v[hash] = deSters->urm;

        free(deSters->info.adresa);
        free(deSters);
        printf("Cladirea a fost stearsa!\n");
        return;
    }
    
    Nod* curent = ht.v[hash];
    Nod* anterior = NULL;
    
    while (curent != NULL)
    {
        if (curent->info.id == id && curent->info.anulConstruirii == an)
        {
            anterior->urm = curent->urm;
            free(curent->info.adresa);
            free(curent);
            printf("Cladirea a fost stearsa!\n");
            return;
        }
        anterior = curent;
        curent = curent->urm;
    }
    printf("Nu s-a gasit cladirea cu Id %d si anul construirii %d\n", id, an);
}


void stergeCladireDupaId(HashTable ht, int id) { // nu stim clusterul
    int gasit = 0;

    for (int i = 0; i < ht.dim; i++)
    {
        if (ht.v[i] == NULL) {
            continue;
        }

        if (ht.v[i]->info.id == id)
        {
            Nod* deSters = ht.v[i];
            ht.v[i] = deSters->urm;

            free(deSters->info.adresa);
            free(deSters);
            printf("Cladirea a fost stearsa!\n");
            gasit = 1;
            break;
        }
        Nod* curent = ht.v[i];
        Nod* anterior = NULL;

        while (curent != NULL)
        {
            if (curent->info.id == id)
            {
                anterior->urm = curent->urm;
                free(curent->info.adresa);
                free(curent);

                printf("Cladirea a fost stearsa!\n");
                gasit = 1;
                break;
            }
            anterior = curent;
            curent = curent->urm;
        }
        if (gasit)
        {
            break;
        }
    }
    if (!gasit)
    {
        printf("Nu s-a gasit cladirea cu Id %d\n", id);
    }
}

// ex. 5 difera complexitatea O(n) pentru prima functie, unde stim clusterul
// si O(m * n) pentru a doua functie, unde trebuie sa parcurgem toate clusterele
// in cel mai rau caz

Cladire* getCladiriDinAn(HashTable ht, int an, int* nrCladiri) {
    int hash = calculeazaHash(an, ht.dim);
    *nrCladiri = 0;

    if (ht.v[hash] == NULL)
    {
        printf("Nu exista cladiri construite in acel an!\n");
        return NULL;
    }

    Nod* temp = ht.v[hash];
    while (temp != NULL)
    {
        if (temp->info.anulConstruirii == an)
        {
            (*nrCladiri)++;
        }
        temp = temp->urm;
    }

    if (*nrCladiri == 0)
    {
        printf("Nu exista cladiri construite in acel an!\n");
        return NULL;
    }
    
    Cladire* vector = (Cladire*)malloc(sizeof(Cladire) * (*nrCladiri));
    temp = ht.v[hash];
    int index = 0;

    while (temp != NULL && index < *nrCladiri)
    {
        if (temp->info.anulConstruirii == an)
        {
            vector[index].id = temp->info.id;
            vector[index].anulConstruirii = temp->info.anulConstruirii;
            vector[index].suprafata = temp->info.suprafata;

            vector[index].adresa = (char*)malloc(strlen(temp->info.adresa) + 1);
            strcpy(vector[index].adresa, temp->info.adresa);
            index++;
        }
        temp = temp->urm;
    }
    printf("Au fost gasite %d cladiri construite in anul %d.", *nrCladiri, an);
    return vector;
}

void afisareVectorCladiri(Cladire* vector, int numCladiri) {
    printf("\n--- Vector cu cladiri ---\n");
    
    if (vector == NULL || numCladiri == 0) {
        printf("Vectorul este gol.\n");
        return;
    }
    
    for (int i = 0; i < numCladiri; i++) {
        printf("\nCladirea %d:\n", i + 1);
        afisareCladire(vector[i]);
    }
}

void dezalocareVectorMasini(Cladire** vector, int* dim) {
    for (int i = 0; i < *dim; i++)
    {
        free((*vector)[i].adresa);
    }
    free(*vector);
    *vector = NULL;
    *dim = 0;
}

void dezalocareTabelaDeMasini(HashTable *ht) {
    for (int i = 0; i < ht->dim; i++)
    {
        Nod* curent = ht->v[i];
        while (curent)
        {
            Nod* temp = curent;
            curent = curent->urm;
            free(temp->info.adresa);
            free(temp);
        }
    }
    free(ht->v);
}

void modificaAnConstruire(HashTable* ht, int id, int anVechi, int anNou) {
    int hashVechi = calculeazaHash(anVechi, ht->dim);
    
    if (ht->v[hashVechi] == NULL)
    {
        printf("Nu exista cladiri construite in acel an!\n");
        return;
    }
    Nod* curent = ht->v[hashVechi];
    Nod* anterior = NULL;
    Cladire cladireGasita;
    int gasit = 0;

    while (curent != NULL)
    {
        if (curent->info.id == id && curent->info.anulConstruirii == anVechi)
        {
            gasit = 1;
            cladireGasita = curent->info;
            cladireGasita.adresa = (char*)malloc(strlen(curent->info.adresa) + 1);
            strcpy(cladireGasita.adresa, curent->info.adresa);

            if (anterior == NULL)
            {
                ht->v[hashVechi] = curent->urm;
            }
            else
            {
                anterior->urm = curent->urm;
            }
            
            free(curent->info.adresa);
            free(curent);
            break;
        }
        anterior = curent;
        curent = curent->urm;
    }

    if (!gasit)
    {
        printf("Nu s-a gasit cladirea cu acel ID in anul respectiv!\n");
        return;
    }

    cladireGasita.anulConstruirii = anNou;

    inserareCladireInTabela(*ht, cladireGasita);
    printf("Cladirea a fost modifica cu succes!");
}

int main() {
    // 1. Inițializăm tabela hash și citim clădirile din fișier
    printf("\n===== INIȚIALIZARE ȘI CITIRE DATE =====\n");
    HashTable ht = citireCladiriDinFisier("cladiri.txt", 5);
    
    // 2. Afișăm toate clădirile din tabela hash
    printf("\n===== AFIȘARE TABELA HASH INIȚIALĂ =====\n");
    afisareTabelaMasini(ht);
    
    // 3. Creăm și inserăm manual o clădire nouă
    printf("\n===== ADĂUGARE CLĂDIRE NOUĂ =====\n");
    Cladire cladireNoua;
    cladireNoua.id = 100;
    cladireNoua.anulConstruirii = 2000;
    cladireNoua.suprafata = 750.5;
    cladireNoua.adresa = (char*)malloc(strlen("Strada Nouă 10") + 1);
    strcpy(cladireNoua.adresa, "Strada Nouă 10");
    
    inserareCladireInTabela(ht, cladireNoua);
    printf("Clădire nouă adăugată cu succes!\n");
    
    // 4. Ștergem o clădire folosind ID-ul și anul construirii
    printf("\n===== ȘTERGERE CLĂDIRE CU ID ȘI AN =====\n");
    stergeCladireDupaAnSiId(ht, 100, 2000); // Ștergem clădirea adăugată anterior
    
    // 5. Afișăm tabela după ștergere
    printf("\n===== AFIȘARE TABELA DUPĂ ȘTERGERE =====\n");
    afisareTabelaMasini(ht);
    
    // 6. Adăugăm din nou clădirea pentru a o folosi în testele următoare
    Cladire cladireNoua2;
    cladireNoua2.id = 100;
    cladireNoua2.anulConstruirii = 2000;
    cladireNoua2.suprafata = 750.5;
    cladireNoua2.adresa = (char*)malloc(strlen("Strada Nouă 10") + 1);
    strcpy(cladireNoua2.adresa, "Strada Nouă 10");
    inserareCladireInTabela(ht, cladireNoua2);
    
    // 7. Ștergem o clădire folosind doar ID-ul
    printf("\n===== ȘTERGERE CLĂDIRE DOAR CU ID =====\n");
    stergeCladireDupaId(ht, 10); // Ștergem clădirea adăugată anterior
    
    // 8. Obținem toate clădirile dintr-un an specific
    printf("\n===== OBȚINERE CLĂDIRI DINTR-UN AN SPECIFIC =====\n");
    int nrCladiri = 0;
    int anCautat = 2010; // Presupunem că avem clădiri din acest an
    Cladire* vectorCladiri = getCladiriDinAn(ht, anCautat, &nrCladiri);
    
    // 9. Afișăm vectorul de clădiri
    printf("\n===== AFIȘARE VECTOR DE CLĂDIRI DIN ANUL %d =====\n", anCautat);
    afisareVectorCladiri(vectorCladiri, nrCladiri);
    
    // 10. Modificăm anul de construire al unei clădiri
    printf("\n===== MODIFICARE AN CONSTRUIRE =====\n");
    // Presupunem că există o clădire cu ID-ul 3 construită în 1985
    modificaAnConstruire(&ht, 3, 1985, 1990);
    
    // 11. Afișăm tabela după modificare
    printf("\n===== AFIȘARE TABELA DUPĂ MODIFICARE AN =====\n");
    afisareTabelaMasini(ht);
    
    // 12. Verificăm noul cluster (anul 1990)
    printf("\n===== VERIFICARE CLĂDIRI DIN ANUL NOU (1990) =====\n");
    int nrCladiriNouAn = 0;
    Cladire* vectorCladiriNouAn = getCladiriDinAn(ht, 1990, &nrCladiriNouAn);
    afisareVectorCladiri(vectorCladiriNouAn, nrCladiriNouAn);
    
    // 13. Dezalocăm memoria
    printf("\n===== DEZALOCARE MEMORIE =====\n");
    if (vectorCladiri != NULL) {
        dezalocareVectorMasini(&vectorCladiri, &nrCladiri);
        printf("Vector clădiri din anul %d dezalocat.\n", anCautat);
    }
    
    if (vectorCladiriNouAn != NULL) {
        dezalocareVectorMasini(&vectorCladiriNouAn, &nrCladiriNouAn);
        printf("Vector clădiri din anul 1990 dezalocat.\n");
    }
    
    dezalocareTabelaDeMasini(&ht);
    printf("Tabela hash dezalocată.\n");
    
    return 0;
}