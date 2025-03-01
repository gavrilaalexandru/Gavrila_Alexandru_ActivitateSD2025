#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Telefon {
    int imei; // 4
    int megaPixeli; // 4
    char* brand; // 8 pe x64 si 4 pe x32
    float timpIncarcare; // 4
    char culoare; // 1
};

struct Telefon initializare(int imei, int megaP, const char* brand, float timpIncarcare, char culoare) {
    struct Telefon t;
    t.imei = imei;
    t.megaPixeli = megaP;
    t.timpIncarcare = timpIncarcare;
    t.culoare = culoare;
    t.brand = (char*)malloc(strlen(brand) + 1);
    strcpy(t.brand, brand);
    // strcpy_s(t.brand, strlen(brand) + 1, brand);
    return t;
}

void afisare(struct Telefon t) {
    printf("IMEI: %d\n", t.imei);
    printf("Mega Pixeli: %d\n", t.megaPixeli);
    printf("Brand: %s\n", t.brand);
    printf("Baterie: %.2f\n", t.timpIncarcare);
    printf("Culoare: %c\n", t.culoare);
}

void modifica_imei(struct Telefon* t, int imeiNou) {
    if (imeiNou != t->imei) {
        t->imei = imeiNou;
    }
}

void modifica_brand(struct Telefon* t, const char* brandNou) {
    if (brandNou != NULL && t != NULL)
    {
        if (t->brand != NULL)
        {
            free(t->brand);
        }
        t->brand = malloc(strlen(brandNou) + 1);
        strcpy(t->brand, brandNou);
        // strcpy_s(*(t).brand, strlen(brandNou) + 1, brandNou);
    }
}

void dezalocare(struct Telefon* t) {
    free(t->brand); // nu are niciun efect daca brand e deja NULL
    t->brand = NULL;
}

int main() {
    struct Telefon telefon;
    telefon = initializare(230, 130, "Iphone", 1.5, 'R');
    afisare(telefon);
    modifica_imei(&telefon, 550);
    modifica_brand(&telefon, "Samsung");
    afisare(telefon);
    dezalocare(&telefon);
    afisare(telefon);

    printf("%lu", sizeof(struct Telefon)); // 21 + 3 din cauza padding-ului
    return 0;
}