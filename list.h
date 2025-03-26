/*=============================================================================
*       list.h
*       timwking1
*       26-Mar 2025
=============================================================================*/

#include <stdlib.h> //size_t

typedef struct
{
    void* data;
    size_t count;
    size_t capacity;
    size_t itemSize;
} List;

typedef int (*CompareFunc)(const void* a, const void*b);
typedef void (*DestroyFunc)(void* item);

//Constructors
List* List_Create(size_t itemSize, size_t capacity);
void List_Destroy(List* list, DestroyFunc destroyFunc);

//Data manipulation
int List_Push(List* list, const void* item);
int List_Pop(List* list, void* item);
void* List_Get(List* list, size_t index);
int List_Set(List* list, size_t index, const void* item);
void List_Sort(List* list, CompareFunc compareFunc);

//Capacity management
void List_Reserve(List* list, size_t newCapacity);
void List_ShrinkToFit(List* list);
size_t List_Capacity(const List* list);
static int List_Resize(List* list);

//Data access
size_t List_Count(const List* list);
void* List_Front(const List* list);
void* List_Back(const List* list);
void* List_At(List* list, size_t index);

//Modifiers
int List_Insert(List* list, size_t index, const void* item);
int List_Erase(List* list, size_t index);
void List_Clear(List* list, DestroyFunc destroyFunc);
int List_Assign(List* list, size_t count, const void* value);

//Operations
int List_Contains(const List* list, const void* item, CompareFunc compareFunc);
int List_Remove(List* list, const void* item, CompareFunc compareFunc);
void List_Reverse(List* list);
int List_Unique(List* list, CompareFunc compareFunc);

//Iterator-like functionality
typedef struct {
    List* list;
    size_t current;
} ListIterator;

ListIterator List_Begin(List* list);
ListIterator List_End(List* list);
int ListIterator_Next(ListIterator* it);
int ListIterator_Prev(ListIterator* it);
void* ListIterator_Get(ListIterator* it);
