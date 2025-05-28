#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date


// informatia sub forma de structura
struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina; // alias

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
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

// HEAP reprezentat sub forma de vector de elemente

typedef struct Heap {
    int lungime; // lungimea totala a vectorului
    Masina* masini; // vectorul
    int nrMasini; // numarul real de masini din vector
} Heap;

Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.masini = (Masina*)malloc(sizeof(Masina) * lungime);
	heap.nrMasini = 0;
	return heap;
}

void filtrareHeap(Heap heap, int pozitieNod) {
	// MAX-HEAP --> parintele trebuie sa aiba ID-ul mai mare decat copiii sai
	if (pozitieNod >= 0 && pozitieNod < heap.nrMasini)
	{
		int pozStanga = 2 * pozitieNod + 1; // pozitia copilului din stanga
		int pozDreapta = 2 * pozDreapta + 2; // pozitia copilului din dreapta
		int pozMaxim = pozitieNod; // presupunem ca nodul curent este nodul maxim

		if (pozStanga < heap.nrMasini && heap.masini[pozMaxim].id < heap.masini[pozStanga].id) // verificam daca copilul din stanga e mai mare (are ID-ul mai mare)
		{
			pozMaxim = pozStanga;
		}

		if (pozDreapta < heap.nrMasini && heap.masini[pozMaxim].id < heap.masini[pozDreapta].id) // verificam daca  copilul din dreapta e mai mare (are ID-ul mai mare)
		{
			pozMaxim = pozDreapta;
		}
		
		if (pozMaxim != pozitieNod) // daca un copil e mai mare, facem swap si continuam recursiv
		{
			Masina aux;
			aux = heap.masini[pozMaxim]; // metoda paharelor
			heap.masini[pozMaxim] = heap.masini[pozitieNod];
			heap.masini[pozitieNod] = aux;

			if (pozMaxim < (heap.nrMasini - 1) / 2) // continua filtrarea daca pozMaxim este nod parinte
			{
				filtrareHeap(heap, pozMaxim);
			}
		}
	}
}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
	// sunt citite toate elementele si abia apoi este filtrat vectorul
	Heap heap = initializareHeap(10);
	FILE* file = fopen(numeFisier, "r");
	int index = 0;
	while (!feof(file)) {
		heap.masini[index] = citireMasinaDinFisier(file);
		index++;
	}

	heap.nrMasini = index;
	
	fclose(file);
	
	for (int i = (heap.nrMasini - 1) / 2; i >= 0; i--)
	{
		filtreazaHeap(heap, i);
	}
	return heap;
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrMasini; i++)
	{
		afisareMasina(heap.masini[i]);
	}
}

void afisareHeapAscuns(Heap heap) {
	for (int i = heap.nrMasini; i < heap.lungime; i++)
	{
		afisareMasina(heap.masini[i]);
	}
}

Masina extrageMasin(Heap* heap) {
	Masina aux = heap->masini[0];
	heap->masini[0] = heap->masini[heap->nrMasini - 1];
	heap->masini[heap->nrMasini - 1] = aux;
	heap->nrMasini--;

	for (int i = (heap->nrMasini - 1) / 2; i >= 0; i--)
	{
		filtrareHeap(*heap, i);
	}
	return aux;
}

void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->lungime; i++)
	{
		free(heap->masini[i].numeSofer);
		free(heap->masini[i].model);
	}
	free(heap->masini);
	heap->masini = NULL;
	heap->nrMasini = 0;
	heap->lungime = 0;
}

int main() {

}
