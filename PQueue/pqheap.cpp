/*
 * File: pqheap.cpp
 * ------------------
 */
 
#include "pqheap.h"
#include "genlib.h"
#include <iostream>

PQueue::PQueue()
{
	// first value blank for easier sorting
	entries.add(0);
}

PQueue::~PQueue()
{
}

void PQueue::print()
{
	for (int i = 0; i < entries.size(); i++)
	{
		cout << entries[i] << " ";
	}
	cout << endl;
}

bool PQueue::isEmpty()
{
	// adjust for empty first entry
	if (entries.size() <= 1)
		return true;
	else
		return false;
}

int PQueue::size()
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
void PQueue::enqueue(int newValue)
{
	entries.add(newValue);
	int index = entries.size()-1;
	// move entry up heap
	while (index > 1 && entries[index] > entries[index/2])
	{		
		swap(index,index/2);
		index /= 2;
	}
}


/* Implementation notes: dequeueMax
 * --------------------------------
 */
int PQueue::dequeueMax()
{	
	if (isEmpty())
		Error("Tried to dequeue max from an empty pqueue!");

	int value = entries[1];
	entries[1] = entries[entries.size()-1];
	entries.removeAt(entries.size()-1);
	int index = 1;
	while (index*2 < entries.size())
	{
		// swap with right child
		if ((index*2)+1 < entries.size() && 
			entries[(index*2)+1] > entries[index*2] && 
			entries[index] < entries[(index*2)+1])
		{
			swap(index,(index*2)+1);
			index = (index*2)+1;
		}
		// swap with left child
		else if (entries[index] < entries[index*2])
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

void PQueue::swap(int indexA, int indexB)
{
	int tmp = entries[indexA];
	entries[indexA] = entries[indexB];
	entries[indexB] = tmp;
}
		
int PQueue::bytesUsed() 
{
	return sizeof(*this) + entries.bytesUsed();
}


string PQueue::implementationName()
{
	return "heap";
}

void PQueue::printDebuggingInfo()
{
	cout << "------------------ START DEBUG INFO ------------------" << endl;
	cout << "Pqueue contains " << size() << " entries" << endl;
	for (int i = 1; i < entries.size(); i++) 
		cout << entries[i] << " ";
	cout << endl;
	cout << "------------------ END DEBUG INFO ------------------" << endl;
}
