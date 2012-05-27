/*
 * File: pqheap.cpp
 * ------------------
 */
 
#include "pqheap.h"
#include <iostream>

template <typename ElemType>
PQueue<ElemType>::PQueue(int (*cmp)(ElemType,ElemType))
{
    cmpFn = cmp;
}

template <typename ElemType>
PQueue<ElemType>::~PQueue()
{
}

template <typename ElemType>
bool PQueue<ElemType>::isEmpty()
{
    // adjust for empty first entry
    if (entries.size() <= 1)
        return true;
    else
        return false;
}

template <typename ElemType>
int PQueue<ElemType>::size()
{
    // adjust for empty first entry
    if (entries.size() == 0)
        return 0;
    else
        return entries.size()-1;
}


/* Implementation notes: enqueue
 * -----------------------------
 */
template <typename ElemType>
void PQueue<ElemType>::enqueue(ElemType newValue)
{
    if (entries.isEmpty())
        entries.add(newValue); // dummy value
        
    entries.add(newValue);
    int index = entries.size()-1;
    // move entry up heap
    while (index > 1 && cmpFn(entries[index],entries[index/2]) > 0)
    {        
        swap(index,index/2);
        index /= 2;
    }
}


/* Implementation notes: dequeueMax
 * --------------------------------
 */
template <typename ElemType>
ElemType PQueue<ElemType>::dequeueMax()
{    
    if (isEmpty())
        Error("Tried to dequeue max from an empty pqueue!");

    ElemType value = entries[1];
    entries[1] = entries[entries.size()-1];
    entries.removeAt(entries.size()-1);
    int index = 1;
    while (index*2 < entries.size())
    {
        // swap with right child
        if ((index*2)+1 < entries.size() && 
            cmpFn(entries[(index*2)+1],entries[index*2]) > 0 && 
            cmpFn(entries[index],entries[(index*2)+1]) < 0)
        {
            swap(index,(index*2)+1);
            index = (index*2)+1;
        }
        // swap with left child
        else if (cmpFn(entries[index],entries[index*2]) < 0)
        {
            swap(index,index*2);
            index = index*2;
        }
        // no more swapping necessary
        else
            break;
    }

    return value;
}
template <typename ElemType>
void PQueue<ElemType>::swap(int indexA, int indexB)
{
    ElemType tmp = entries[indexA];
    entries[indexA] = entries[indexB];
    entries[indexB] = tmp;
}