
#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>

// --- Maximum Heap Size ---
#define MAX_HEAP 256

// --- Heap Data Types ---
typedef uint16_t Key;
typedef uint8_t HeapEntry;

typedef struct
{
	Key key;
	HeapEntry entry;
}
HeapElement;

typedef struct
{
	uint8_t size;
	HeapElement heap[MAX_HEAP];
}
Heap;

// --- Initialization Function ---
void Heap_Init(Heap *heap);

// --- Basic Heap Functions ---
void Heap_Insert(Heap *heap, HeapEntry entry, Key key);
HeapEntry Heap_Remove(Heap *heap);

// --- Heap Helper Function ---
void Heap_Swap(Heap *heap, uint8_t a, uint8_t b);

#endif

