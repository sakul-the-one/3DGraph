#include "LinkedList.h"

LinkedList * CreateList() 
{
    LinkedList * ret = malloc(sizeof(LinkedList));
    ret->count = 0;
    ret->first = NULL;
    ret->last = NULL;
    return ret;
}
void AddItem(LinkedList * List, void * Data, int DataLenght) 
{
    LinkedItem * buf = malloc(sizeof(LinkedItem));
    if(List->first == NULL) List->first = buf;
    else List->last->next = buf;
    List->last = buf;
    buf->Data = Data;
    buf->DataLenght = DataLenght;
    buf->next = NULL;
    List->count++;
}
LinkedItem * GetItem(LinkedList * List, int which) 
{
    LinkedItem * result = List->first;
    if (List->count < which) return NULL;
    for (int i = 0; i < which; i++) 
    {
        if(result->next != NULL)
            result = result->next;
        return NULL;
    }
    return result;
}
void RemoveItem(LinkedList * List, uint8_t pos) 
{
    if(pos > List->count) return;
    LinkedItem * ToDelete;
    List->count--;
    if(pos == 0) 
    {
        ToDelete = List->first;
        List->first = List->first->next;
        free(ToDelete);
        return;
    }
    LinkedItem * curent = List->first;
    for(int i = 0; i > pos -1; i++) 
    {
        curent = curent->next;
        if(curent == NULL) return;
    }
    ToDelete = curent->next;
    curent->next = ToDelete; //curent->next->next; //I love C ^^^

    free(ToDelete);
}
void DestroyList(LinkedList * List) 
{
    LinkedItem * next = List->first;
    while (next != 0)
    {
        LinkedItem * buffer = next;
        next = next->next;
        free(buffer->Data); //:)
        free(buffer);
    }
}