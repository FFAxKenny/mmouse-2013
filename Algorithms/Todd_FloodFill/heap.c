
#include "heap.h"

// --- Initialization Function ---
void Heap_Init(Heap *heap)
{
	heap->size = 0;
}

// --- Insert Entry into Heap ---
void Heap_Insert(Heap *heap, HeapEntry entry, Key key)
{
	int i = heap->size++;
	int parent;
	HeapElement newEntry = {key, entry};

	heap->heap[i] = newEntry;
	while(i)
	{
		parent = (i-1)/2;
		if(heap->heap[i].key < heap->heap[parent].key)
			Heap_Swap(heap, i, parent);
		else return;
		i = parent;
	}
}

// --- Remove and Return Lowest Value ---
HeapEntry Heap_Remove(Heap *heap)
{
	// --- Initialize New Heap Size ---
	const int newSize = --heap->size;
	int V = 0, L = 1, R = 2;
	Key Vkey, Lkey, Rkey;

	// --- Remove Min from Heap ---
	HeapEntry entry = heap->heap[0].entry;
	Heap_Swap(heap, 0, newSize);


	// --- While Heap Unsorted ---
	while(R < newSize)
	{
		// --- Retrieve Keys ---
		Vkey = heap->heap[V].key;
		Lkey = heap->heap[L].key;
		Rkey = heap->heap[R].key;

		// --- Handle Heap Cases ---
		switch((Vkey<Lkey) + 2*(Vkey<Rkey) + 4*(Lkey<Rkey))
		{
			case 0: Heap_Swap(heap,V,R); V = R; break; // V > L > R
			case 1: Heap_Swap(heap,V,R); V = R; break; // L > V > R
			case 2:                             break; // V > L > R > V
			case 3: return entry;               break; // L > R > V
			case 4: Heap_Swap(heap,V,L); V = L; break; // V > R > L
			case 5:                             break; // V > R > L > V
			case 6: Heap_Swap(heap,V,L); V = L; break; // R > V > L
			case 7: return entry;               break; // R > L > V
		}

		// --- Update Children ---
		L = 2 * V + 1;
		R = 2 * V + 2;
	}

	// --- If Heap has 2 Entries ---
	if(heap->size == 2 && heap->heap[L].key < heap->heap[V].key) Heap_Swap(heap,V,L);

	// --- Return Entry ---
	return entry;
}

// --- Swap Two Elements in Heap ---
void Heap_Swap(Heap *heap, uint8_t a, uint8_t b)
{
	HeapElement temp = heap->heap[a];
	heap->heap[a] = heap->heap[b];
	heap->heap[b] = temp;
}

