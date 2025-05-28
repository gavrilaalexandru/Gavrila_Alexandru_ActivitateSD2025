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
typedef struct StructuraMasina Masina;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret= atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy(m1.model, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

// arbore binar de cautare
typedef struct Nod Nod;

struct Nod
{
    Nod* st;
    Nod* dr;
    Masina info;
};


void adaugaMasinaInArboreBinar(Nod** radacina, Masina masinaNoua) {
    if ((*radacina) != NULL)
    {
        if (masinaNoua.id < (*radacina)->info.id)
        {
            adaugaMasinaInArboreBinar(&((*radacina)->st), masinaNoua);
        }
        else if (masinaNoua.id > (*radacina)->info.id)
        {
            adaugaMasinaInArboreBinar(&((*radacina)->dr), masinaNoua);
        }
        else
        {
            (*radacina)->info.nrUsi = masinaNoua.nrUsi;
            (*radacina)->info.pret = masinaNoua.pret;
            (*radacina)->info.serie = masinaNoua.serie;
            
            free((*radacina)->info.numeSofer);
            free((*radacina)->info.model); // free oricand; nu trb verificat daca e diferit de NULL

            (*radacina)->info.model = (char*)malloc(strlen(masinaNoua.model) + 1);
            strcpy((*radacina)->info.model, masinaNoua.model);
            
            (*radacina)->info.numeSofer = (char*)malloc(strlen(masinaNoua.numeSofer) + 1);
            strcpy((*radacina)->info.numeSofer, masinaNoua.numeSofer);
        }
    }
    else
    {
        (*radacina) = (Nod*)malloc(sizeof(Nod));
        (*radacina)->info = masinaNoua;

        (*radacina)->info.model = (char*)malloc(strlen(masinaNoua.model) + 1);
        strcpy((*radacina)->info.model, masinaNoua.model);
            
        (*radacina)->info.numeSofer = (char*)malloc(strlen(masinaNoua.numeSofer) + 1);
        strcpy((*radacina)->info.numeSofer, masinaNoua.numeSofer);

        (*radacina)->st = NULL;
        (*radacina)->dr = NULL;
    }    
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Nod* radacina = NULL;
	while (!feof(file)) {
		Masina m = citireMasinaDinFisier(file);
		adaugaMasinaInArboreBinar(&radacina, m);
		free(m.model);
		free(m.numeSofer);
	}
	fclose(file);
	return radacina;
}

void afisareMasiniInordine(Nod* radacina) {
    if (radacina != NULL)
    {
        afisareMasiniInordine(radacina->st);
        afisareMasina(radacina->info);
        afisareMasiniInordine(radacina->dr);
    }
}

void afisareMasiniPreordine(Nod* radacina) {
    if (radacina != NULL)
    {
        afisareMasina(radacina->info);
        afisareMasiniPreordine(radacina->st);
        afisareMasiniPreordine(radacina->dr);
    }
}

void afisareMasiniPostordine(Nod* radacina) {
    if (radacina != NULL)
    {
        afisareMasiniPostordine(radacina->st);
        afisareMasiniPostordine(radacina->dr);
        afisareMasina(radacina->info);
    }
}

void dezalocareArbore(Nod** radacina) {
    if ((*radacina) != NULL)
    {
        dezalocareArbore(&((*radacina)->st));
		dezalocareArbore(&((*radacina)->dr));
		free((*radacina)->info.model);
		free((*radacina)->info.numeSofer);
		free(*radacina);
		*radacina = NULL;
    }
}

Masina getMasinaByID(Nod* radacina, int id) {
    Masina m;
    m.id = -1; 
    if (id > radacina->info.id)
    {
        return getMasinaByID(radacina->dr, id);
    }
    else if (id < radacina->info.id)
    {
        return getMasinaByID(radacina->st, id);
    }
    else
    {
        m = radacina->info;
        m.numeSofer = (char*)malloc(strlen(radacina->info.numeSofer) + 1);
		strcpy(m.numeSofer, radacina->info.numeSofer);

		m.model= (char*)malloc(strlen(radacina->info.model) + 1);
		strcpy(m.model, radacina->info.model);
		return m;
    }
    return m;
}

int determinaNumarNoduri(Nod* radacina) {
    if (radacina != NULL)
    {
        return 1 + determinaNumarNoduri(radacina->st) + determinaNumarNoduri(radacina->dr);
    }
    else
    {
        return 0;
    }
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int calculeazaInaltimeArbore(Nod* radacina) {
	//calculeaza inaltimea arborelui care este data de 
	//lungimea maxima de la radacina pana la cel mai indepartat nod frunza

    if (radacina == NULL)
    {
        return -1;
    }
    
    if (radacina->st == NULL && radacina->dr == NULL)
    {
        return 0;
    }

    int inaltimeSt = calculeazaInaltimeArbore(radacina->st);
    int inaltimeDr = calculeazaInaltimeArbore(radacina->dr);

    return 1 + max(inaltimeSt, inaltimeDr);
}

float calculeazaPretTotal(Nod* radacina) {
	//calculeaza pretul tuturor masinilor din arbore.
    if (radacina == NULL)
    {
        return 0.0;
    }

    return radacina->info.pret + calculeazaPretTotal(radacina->st) + calculeazaPretTotal(radacina->dr);
}

float calculeazaPretulMasinilorUnuiSofer(/*arbore de masini*/ const char* numeSofer) {
	//calculeaza pretul tuturor masinilor unui sofer.
	return 0;
}

int main() {

}