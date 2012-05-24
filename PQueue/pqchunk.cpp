#include "pqchunk.h"
#include "genlib.h"
#include <iostream>

int const MaxElemsPerBlock = 4; // chunk size; must be greater than 0

PQueue::PQueue()
{
    head = NULL;
}

PQueue::~PQueue()
{
    while (head != NULL) 
    {
        Cell *next = head->next;
        delete[] head->values;
        delete head;
        head = next;
    }
}

void PQueue::print()
{
    for (Cell *cur = head; cur != NULL; cur = cur->next)
    {
        for (int i = 0; i < cur->BlocksInUse; i++)
        {
            cout << cur->values[i] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

bool PQueue::isEmpty()
{
    return (head == NULL);  
}

int PQueue::size()
{
    int count = 0;
    for (Cell *cur = head; cur != NULL; cur = cur->next)
        count += cur->BlocksInUse;
    return count;
}

void PQueue::enqueue(int newValue)
{
    if (head == NULL)
    {
        Cell *newChunk = initCell(newValue, NULL);
        head = newChunk;
        return;
    }
    
    Cell *cur, *prev, *newChunk = new Cell;
    for (prev = NULL, cur = head; cur != NULL; prev=cur, cur = cur->next)
    {
        for (int i = 0; i < cur->BlocksInUse; i++)
        {
            if (newValue > cur->values[i])
            {
                // put in this cell if there is room
                if (cur->BlocksInUse < MaxElemsPerBlock)
                {
                    shiftAdd(cur,newValue);
                    return;
                }
                // create a new Cell at the head and insert value
                else if (i == 0 && prev == NULL)
                {
                    Cell *newChunk = initCell(newValue, cur);
                    head = newChunk;
                    return;
                }
                // put in previous cell if there is room
                else if (i == 0 && prev != NULL && prev->BlocksInUse < MaxElemsPerBlock)
                {
                    shiftAdd(prev,newValue);
                    return;
                }
                // current and previous cells full, must split
                else
                {
                    if (MaxElemsPerBlock > 1)
                    {
                        splitAdd(cur,newValue);
                        return;
                    } 
                    else // can't split a single block, so just insert into list
                    {
                        Cell *newChunk = initCell(newValue,cur);
                        prev->next = newChunk;
                        return;
                    }
                }
            }
        }
    }

    // value is the smallest; insert in last Cell if there is room
    if (prev->BlocksInUse < MaxElemsPerBlock)
    {
        prev->values[prev->BlocksInUse] = newValue;
        prev->BlocksInUse++;
    }
    // create new Cell at the end of the list and add value
    else
    {
        Cell *newChunk = initCell(newValue, NULL);
        prev->next = newChunk;
    }
}

/* 
* Searches cell to find appropriate insert point.
* Shifts values to make room, then inserts value.
*/
void PQueue::shiftAdd(Cell *cell, int newValue)
{
    int index; // point of insertion
    for (index = 0; index < MaxElemsPerBlock; index++)
    {
        if (newValue > cell->values[index])
            break;
    }

    for (int i = MaxElemsPerBlock-1; i >= index; i--)
    {
        cell->values[i] = cell->values[i-1]; // shift values
    }

    cell->values[index] = newValue; // insert
    cell->BlocksInUse++;
}

/*
* Splits current Cell values between two Cells.
* Searches for insertion point, then adds value.
*/
void PQueue::splitAdd(Cell *cur, int newValue)
{
    Cell *newChunk = new Cell;
    newChunk->values = new int[MaxElemsPerBlock];
    for (int i = MaxElemsPerBlock/2; i < MaxElemsPerBlock; i++)
    {
        newChunk->values[i-(MaxElemsPerBlock/2)] = cur->values[i]; // copy half of the old values into new Cell
    }

    // update blocks in use; check for odd MaxElemsPerBlock
    cur->BlocksInUse = MaxElemsPerBlock/2;
    if (MaxElemsPerBlock % 2 == 0)
    {
        newChunk->BlocksInUse = MaxElemsPerBlock/2;    
    }
    else
    {
        newChunk->BlocksInUse = (MaxElemsPerBlock/2)+1;        
    }
    
    // insert into list
    newChunk->next = cur->next;
    cur->next = newChunk;

    // check older Cell for insertion point
    for (int j = 0; j < cur->BlocksInUse; j++)
    {
        if (newValue > cur->values[j])
        {
            shiftAdd(cur,newValue);
            return;
        }
    }

    // insert into new Cell
    for (int k = 0; k < newChunk->BlocksInUse; k++)
    {
        if (newValue > newChunk->values[k])
        {
            shiftAdd(newChunk,newValue);
            return;
        }        
    }
}

/*
* Creates a new Cell and returns a pointer. Must pass a NULL pointer if the cell will be
* placed at the end of the list.
*/
PQueue::Cell *PQueue::initCell(int newValue, Cell *nextCell)
{
    Cell *newChunk = new Cell;
    newChunk->values = new int[MaxElemsPerBlock];
    newChunk->values[0] = newValue;
    newChunk->BlocksInUse = 1;
    newChunk->next = nextCell;
    return newChunk;
}

/*
* Returns and removes the largest (first) value in the list.
*/
int PQueue::dequeueMax()
{
    if (isEmpty())
        Error("Tried to dequeue max from an empty pqueue!");

    int num = head->values[0]; // value to return

    // shift remaining values
    if (head->BlocksInUse > 1)
    {
        for (int i = 0; i < head->BlocksInUse-1; i++)
        {
            head->values[i] = head->values[i+1];
        }
    }

    head->BlocksInUse--;

    // if no remaining values, delete/remove Cell
    if (head->BlocksInUse == 0)
    {
         Cell *toBeDeleted = head;
         head = head->next;     // splice head cell out
        delete[] toBeDeleted->values;
        delete toBeDeleted;
    }

    return num;
}

int PQueue::bytesUsed()
{
    int total = sizeof(*this);
     for (Cell *cur = head; cur != NULL; cur = cur->next)
         total += sizeof(*cur);
     return total;
}

string PQueue::implementationName()
{
    return "chunk list";
}

void PQueue::printDebuggingInfo()
{
   int count = 0;

    cout << "------------------ START DEBUG INFO ------------------" << endl;
    for (Cell *cur = head; cur != NULL; cur = cur->next) {
        cout << "Cell #" << count << " (at address " << cur << ") values = ";
        for (int i = 0; i < cur->BlocksInUse; i++)
        {
            cout << cur->values[i] << " ";
        }
       cout << " next = " << cur->next << endl;
       count++;
    }
    cout << "------------------ END DEBUG INFO ------------------" << endl;
}
