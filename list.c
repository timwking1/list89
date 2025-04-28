/*=============================================================================
*       list.c
*       timwking1
*       26-Mar 2025
=============================================================================*/

#include "list.h"

List* List_Create(size_t itemSize, size_t capacity)
{
    List* list = (List*)malloc(sizeof(List));
    if(!list)
    {
        return NULL;
    }
    list->itemSize = itemSize;
    list->capacity = capacity;
    list->count = 0;

    list->data = malloc(itemSize * capacity);
    if(!list->data)
    {
        free(list);
        return NULL;
    }
    return list;
}

void List_Destroy(List* list, DestroyFunc destroyFunc)
{
    List_Clear(list, destroyFunc);
    free(list->data);
    free(list);
}

int List_Resize(List* list)
{
    size_t capacity;
    char* newData;

    capacity = list->capacity * 2;
    newData = (char*)realloc(list->data, list->itemSize * capacity);
    if(!newData)
    {
        return 0;
    }

    list->data = (void*)newData;
    list->capacity = capacity;
    return 1;
}

int List_Push(List* list, const void* item)
{
    void* dest;

    if(!list || !item)
    {
        return 0;
    }
    if(list->count >= list->capacity)
    {
        if(!List_Resize(list))
        { 
            return 0;
        }
    }
    dest = (char*)list->data + (list->count * list->itemSize);
    memcpy(dest, item, list->itemSize);
    list->count++;
    return 1;
}

int List_Pop(List* list, void* item)
{
    void* src;

    if (!list || !item || list->count <= 0)
    {
        return 0;
    }

    list->count--;
    src = (char*)list->data + (list->count * list->itemSize);
    memcpy(item, src, list->itemSize);
    return 1;
}

void* List_Get(const List* list, size_t index)
{
    if(!list || index >= list->count)
    {
        return NULL;
    }
    return (char*)list->data + (index * list->itemSize);
}

int List_Set(List* list, size_t index, const void* item)
{
    void* dest;
    if(!list || !item || index >= list->count)
    {
        return 0;
    }
    dest = (char*)list->data + (index * list->itemSize);
    memcpy(dest, item, list->itemSize);
    return 1;
}

size_t List_Count(const List* list)
{
    return list ? list->count : 0;
}

void List_Sort(List* list, CompareFunc compareFunc)
{
    if(!list || !compareFunc)
    {
        return;
    }
    qsort(list->data, list->count, list->itemSize, compareFunc); 
}

/* Capacity management */
void List_Reserve(List* list, size_t newCapacity)
{
    char* newData;

    if(!list || newCapacity <= list->capacity)
    {
        return;
    }
    newData = (char*)realloc(list->data, list->itemSize * newCapacity);
    if(newData)
    {
        list->data = (void*)newData;
        list->capacity = newCapacity;
    }
}

void List_ShrinkToFit(List* list)
{
    char* newData;

    if(!list || list->count == list->capacity)
    {
        return;
    }

    newData = (char*)realloc(list->data, list->itemSize * list->count);
    if(newData)
    {
        list->data = (void*)newData;
        list->capacity = list->count;
    }
}

size_t List_Capacity(const List* list)
{
    return list ? list->capacity : 0;
}

/* Element access */
void* List_Front(const List* list)
{
    return List_Get(list, 0);
}

void* List_Back(const List* list)
{
    return list && list->count > 0 ? List_Get(list, list->count - 1) : NULL;
}

void* List_At(List* list, size_t index)
{
    if(!list || index >= list->count)
    {
        return NULL;
    }
    return List_Get(list, index);
}

/* Modifiers */
int List_Insert(List* list, size_t index, const void* item)
{
    void* dest;
    void* src;
    size_t shiftSize;

    if(!list || !item || index > list->count)
    {
        return 0;
    }

    if(list->count >= list->capacity)
    {
        if(!List_Resize(list))
        {
            return 0;
        }
    }

    /* Shift elements to make space */
    dest = (char*)list->data + ((index + 1) * list->itemSize);
    src = (char*)list->data + (index * list->itemSize);
    shiftSize = (list->count - index) * list->itemSize;
    memmove(dest, src, shiftSize);

    /* Insert new element */
    dest = (char*)list->data + (index * list->itemSize);
    memcpy(dest, item, list->itemSize);
    list->count++;
    return 1;
}

int List_Erase(List* list, size_t index)
{
    void* dest;
    void* src;
    size_t shiftSize;

    if(!list || index >= list->count)
    {
        return 0;
    }

    /* Shift elements to remove gap */
    dest = (char*)list->data + (index * list->itemSize);
    src = (char*)list->data + ((index + 1) * list->itemSize);
    shiftSize = (list->count - index - 1) * list->itemSize;
    memmove(dest, src, shiftSize);

    list->count--;
    return 1;
}

void List_Clear(List* list, DestroyFunc destroyFunc)
{
    size_t i;

    if(!list)
    {
        return;
    }

    if(destroyFunc)
    {
        for(i = 0; i < list->count; i++)
        {
            void* item = (char*)list->data + (i * list->itemSize);
            destroyFunc(item);
        }
    }
    list->count = 0;
}

int List_Assign(List* list, size_t count, const void* value)
{
    char* newData;
    size_t i;
    void* dest;

    if(!list || !value)
    {
        return 0;
    }

    if(count > list->capacity)
    {
        newData = (char*)realloc(list->data, list->itemSize * count);
        if(!newData)
        {
            return 0;
        }
        list->data = (void*)newData;
        list->capacity = count;
    }

    for(i = 0; i < count; i++)
    {
        dest = (char*)list->data + (i * list->itemSize);
        memcpy(dest, value, list->itemSize);
    }
    list->count = count;
    return 1;
}

int List_Contains(const List* list, const void* item, CompareFunc compareFunc)
{
    size_t i;
    void* current;

    if(!list || !item || !compareFunc)
    {
        return 0;
    }

    for(i = 0; i < list->count; i++)
    {
        current = (char*)list->data + (i * list->itemSize);
        if(compareFunc(current, item) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int List_Remove(List* list, const void* item, CompareFunc compareFunc)
{
    size_t i;
    void* current;

    if(!list || !item || !compareFunc)
    {
        return 0;
    }

    for(i = 0; i < list->count; i++)
    {
        current = (char*)list->data + (i * list->itemSize);
        if(compareFunc(current, item) == 0)
        {
            return List_Erase(list, i);
        }
    }
    return 0;
}

void List_Reverse(List* list)
{
    char* temp = malloc(list->itemSize);
    size_t i;
    void* left;
    void* right;

    if(!list || list->count <= 1)
    {
        return;
    }

    if(!temp)
    {
        return;
    }
    /* check so we don't divide by 0 */
    if(list->count > 0)
    {
        for(i = 0; i < list->count / 2; i++)
        {
            left = (char*)list->data + (i * list->itemSize);
            right = (char*)list->data + ((list->count - 1 - i) * list->itemSize);
            
            memcpy(temp, left, list->itemSize);
            memcpy(left, right, list->itemSize);
            memcpy(right, temp, list->itemSize);
        }
        free(temp);
    }
    else
    {
        free(temp);
        return;
    }
}

int List_Unique(List* list, CompareFunc compareFunc)
{
    size_t writeIndex;
    size_t readIndex;
    void* current;
    void* previous;
    void* dest;

    if(!list || !compareFunc || list->count <= 1)
    {
        return 0;
    }

    writeIndex = 1;
    for(readIndex = 1; readIndex < list->count; readIndex++)
    {
        current = (char*)list->data + (readIndex * list->itemSize);
        previous = (char*)list->data + ((writeIndex - 1) * list->itemSize);
        
        if(compareFunc(current, previous) != 0)
        {
            if(writeIndex != readIndex)
            {
                dest = (char*)list->data + (writeIndex * list->itemSize);
                memcpy(dest, current, list->itemSize);
            }
            writeIndex++;
        }
    }

    list->count = writeIndex;
    return 1;
}

/* Iterator implementation */
ListIterator List_Begin(List* list)
{
    ListIterator it;
    it.list = list;
    it.current = 0;
    return it;
}

ListIterator List_End(List* list)
{
    ListIterator it;
    it.list = list;
    it.current = list ? list->count : 0;
    return it;
}

int ListIterator_Next(ListIterator* it)
{
    if(!it || !it->list || it->current >= it->list->count)
    {
        return 0;
    }
    it->current++;
    return 1;
}

int ListIterator_Prev(ListIterator* it)
{
    if(!it || !it->list || it->current == 0)
    {
        return 0;
    }
    it->current--;
    return 1;
}

void* ListIterator_Get(ListIterator* it)
{
    if(!it || !it->list || it->current >= it->list->count)
    {
        return NULL;
    }
    return (char*)it->list->data + (it->current * it->list->itemSize);
}
