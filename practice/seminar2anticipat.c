#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student {
    int id;
    int varsta;
    char* nume;
    float medie;
    char anStudiu;
};

struct Student initializare(int id, int varsta, const char* nume, float medie, char anStudiu) {
    struct Student s;
    s.id = id;
    s.varsta = varsta;
    s.medie = medie;
    s.anStudiu = anStudiu;
    s.nume = (char*)malloc(strlen(nume) + 1);
    strcpy(s.nume, nume);
    // strcpy_s(s.nume, strlen(nume) + 1, nume);
    return s;
}

void afisare(struct Student s) {
    printf("Id: %d\nVarsta: %d\nNume: %s\nMedie: %.2f\nAn Studiu: %c\n", s.id, s.varsta, s.nume, s.medie, s.anStudiu);
}

void afisareVector(struct Student* s, int nrElemente) {
    for (int i = 0; i < nrElemente; i++)
    {
        afisare(s[i]);
    }
}

struct Student* copiazaPrimeleNElemente(struct Student* vector, int nrElemente, int nrElementeCopiate) {
    if (nrElementeCopiate > nrElemente) {
        nrElementeCopiate = nrElemente; // in caz ca cineva da un nr mai mare decat nr de elemente din vector
    }

    struct Student* vectorNou = malloc(nrElementeCopiate * sizeof(struct Student));

    for (int i = 0; i < nrElementeCopiate; i++)
    {
        vectorNou[i] = initializare(vector[i].id, vector[i].varsta, vector[i].nume, vector[i].medie, vector[i].anStudiu);
    }
    return vectorNou;
}

void dezalocare(struct Student** s, int* nrElemente) {
    for (int i = 0; i < *nrElemente; i++)
    {
        free((*s)[i].nume);
    }
    free(*s);
    *s = NULL;
    *nrElemente = 0;
}

//void copiazaAnumiteElemente(struct Sablon* vector, char nrElemente, float prag, struct Sablon** vectorNou, int* dimensiune); ?

void copiazaAnumiteElemente(struct Student* vector, int nrElemente, float pragMedie, struct Student** vectorNou, int* dim) {
    *dim = 0;
    for (int i = 0; i < nrElemente; i++)
    {
        if (vector[i].medie > pragMedie) {
            (*dim)++;
        }
    }

    if (*dim == 0) {
        *vectorNou = NULL;
        return;
    }

    *vectorNou = (struct Student*)malloc((*dim) * sizeof(struct Student));
    int index = 0;
    for (int i = 0; i < nrElemente; i++)
    {
        if (vector[i].medie > pragMedie) {
            (*vectorNou)[index] = initializare(vector[i].id, vector[i].varsta, vector[i].nume, vector[i].medie, vector[i].anStudiu);
            index++;
        }
    }
}

struct Student getPrimulElementConditionat(struct Student* vector, int nrElemente, const char* conditie) {
    for (int i = 0; i < nrElemente; i++)
    {
        if (strcmp(vector[i].nume, conditie) == 0) {
            return vector[i];
        }
    }
    return initializare(-1, -1, "Necunoscut", 0, '-');
}

int main() {
    // struct Student s;
    // s = initializare(1, 19, "Alexandru", 8.55, '2');
    // afisare(s);

    int nrElemente = 4;
    struct Student* vector = (struct Student*)malloc(nrElemente * sizeof(struct Student));
    vector[0] = initializare(1, 19, "Alexandru", 8.55, '2');
    vector[1] = initializare(2, 20, "Andrei", 10, '3');
    vector[2] = initializare(3, 17, "Ioana", 7.5, '1');
    vector[3] = initializare(4, 18, "Mihai", 9.20, '4');
    afisareVector(vector, nrElemente);

    struct Student* vectorFiltat = NULL;
    int dimFiltrata = 0;
    float prag = 9;
    copiazaAnumiteElemente(vector, nrElemente, prag, &vectorFiltat, &dimFiltrata);
    printf("\n");
    afisareVector(vectorFiltat, dimFiltrata);
    printf("\n");

    struct Student student = getPrimulElementConditionat(vector, nrElemente, "Alexandru");
    afisare(student);

    dezalocare(&vector, &nrElemente);
    dezalocare(&vectorFiltat, &dimFiltrata);

    return 0;
}