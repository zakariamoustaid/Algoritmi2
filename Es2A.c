#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct
{
	int heap_size;
    int max_size;
    int *nodo;
}heap;

int kN(int dim)
{
   int k, low;
   low = 0.5*dim;
   k = low + rand()%(dim - low);
   return k;
}

int kLog_N(int dim)                             
{
        double l = log((double)dim);
        int low = (int)l*0.5;
        int high = (int)l*20;
        int k = low + rand()%(high-low);
        //printf("%d\n", k);
        return k;
}
void swap(int *a, int *b)
{
    int s;
    s = *b;
    *b = *a;
    *a = s;
}

int parent(int i)
{
    return (i - 1) / 2;
}

int left(int i)
{
    return 2 * i+1 ;
}
int right(int i)
{
    return 2 * i +2;
}

int randomizedPartition(int arr[], int p, int r)
{
    int pivotIndex = p + rand()%(r - p + 1); //genero un pivot casuale
    int pivot;
    int i = p - 1;
    int j;
    pivot = arr[pivotIndex];
    swap(&arr[pivotIndex], &arr[r]);
    for (j = p; j < r; j++)
    {
        if (arr[j] <= pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
 
    }
    swap(&arr[i+1], &arr[r]);
    return i + 1;
}

void randomizedQuickSort(int A[], int p, int r)
{
    int q;
    if (p < r)
    {
        q = randomizedPartition(A, p, r);
        randomizedQuickSort(A, p, q - 1);
        randomizedQuickSort(A, q + 1, r);
    }
}

int kSort(int A[], int p, int r, int KEY)
{
    randomizedQuickSort(A, p, r-1);
    return A[KEY];
}

int kPartition(int A[], int p, int r, int KEY)
{
    int pivot;
    if (p < r)
    {
        pivot = randomizedPartition(A, p, r);
        if (pivot == KEY)
            return A[KEY];
        else if (pivot < KEY)
            return kPartition(A, pivot, r, KEY);
        else
            return kPartition(A, p, pivot - 1, KEY);
    }
}

void minHeap(heap *A, int i)
{
    int l = left(i);
    int r = right(i);
    int smallest = i;
    if (l < A->heap_size && A->nodo[l] < A->nodo[i])
        smallest = l;
    if (r < A->heap_size && A->nodo[r] < A->nodo[smallest])
        smallest = r;
    if (smallest != i)
    {
        swap(&A->nodo[i], &A->nodo[smallest]);
        minHeap(&*A, smallest);
    }
}

int extractMin(heap *A)
{
    int min;
    if (A->heap_size < 1)
        printf("underflow");
    min = A->nodo[0];
    A->nodo[0] = A->nodo[A->heap_size-1];
    A->heap_size--;
    minHeap(&*A, 0);
    return min;
}

void buildMinHeap(heap *A, int dim)
{
    int i;
    int n = (A->heap_size / 2);
    for (i = n; i >= 0; i--)
        minHeap(&*A, i);
}

int kMinHeap(heap *A, int KEY)
{
    int i;
    A->heap_size = A->max_size;
    buildMinHeap(A, A->heap_size);
    for (i = 0; i < KEY; i++)
        extractMin(&*A);
    return extractMin(&*A);
}

void maxHeapify(heap *B, int i)
{
    int l = left(i);
    int r = right(i);
    int max = i;
    if (l < B->heap_size && B->nodo[l] > B->nodo[i])
        max = l;
    if (r < B->heap_size && B->nodo[r] > B->nodo[max])
        max = r;
    if (max != i)
    {
        swap(&B->nodo[i], &B->nodo[max]);
        maxHeapify(B, max);
    }
}

int maxElem(heap *A)
{
    return A->nodo[0];
}

void buildMaxHeap(heap *B)
{
    int i;
    int n = (B->heap_size-1) / 2;
    for (i = n; i >= 0; i--)
        maxHeapify(B, i);
}
void decreaseKey(heap *A, int i, int priority)
{
    if (priority > A->nodo[i])
        printf("error");
    A->nodo[i] = priority;
    while (i > 0 && A->nodo[parent(i)] > A->nodo[i])
    {
        int p = parent(i);
        swap(&i, &p);
        i = p;
    }
}

int kMaxHeap(int *A, int dim, int KEY)
{
    heap B;
    B.max_size = dim;
    B.heap_size = KEY;
    B.nodo = malloc(sizeof(heap)*KEY);
    int i;
    for (i = 0; i < KEY; i++)
    {
        B.nodo[i] = A[i];
    }
    buildMaxHeap(&B);
    for (i = KEY; i < dim; i++)
    {
        if (A[i] < maxElem(&B))
        {
            decreaseKey(&B, 0, A[i]);
            maxHeapify(&B, 0);
        }
    }
    return maxElem(&B);
}

int OrdinaArray(int dim, int KEY)
{
    int array[dim];
    int array2[dim];
    int i;
    int differenza = 0;
    clock_t t1, t2;
	heap A;
    A.max_size = dim;
    A.nodo = malloc(sizeof(int)*dim);
    for (i = 0; i < dim; i++) //creo array di grandezza dim
    {
        array[i] = rand() %10000+1;
        A.nodo[i] = array[i];
    }
    t1 = clock();
    //kMaxHeap(array, dim, KEY);
    //kPartition(array, 0, dim, KEY);
    //kSort(array, 0, dim, KEY);
    kMinHeap(&A, KEY);
    /*if(check(array, 0, dim))              //verifico se array è ordinato
    printf("non ordinato\n");
	else 
	printf("ordinato\n");*/
    t2 = clock();
    differenza = t2 - t1; //valore di ritorno per la media
	free(A.nodo);
    return differenza;
}

int main()
{
    int i = 100;
    int media = 0;
    int j = 0;
	int key;

    while (i <= 10000 ) //ciclo che scorre la grandezza dell'array
    {
		//key = 60;
        //key = kN(i);
        key = kLog_N(i);
        while (j < 10) //ciclo che incrementerà il valore media
        {
            media = media + OrdinaArray(i, key); //funzione Ordina riceve grandezza array che andrà a creare nella funzione
            j++;
        }
        media = media / 10; //media
        printf("%d\n", media); //stampa tempi medi
        media = 0;
        i = i + 100; //incremento indice array
        j = 0;
    }
}
