/*
 * File: pqheap.h
 * --------------
 * Defines the interface for the priority queue class. 
 * 
 */
 
#ifndef _pqheap_h
#define _pqheap_h

#include "genlib.h"
#include "vector.h"
#include "cmpfn.h"

/*
 * Class: PQueue
 * -------------
 * This is the class for a priority queue.  This is not
 * simple FIFO queue, it is a priority queue, where elements are
 * retrieved in order of priority, not just by longevity in queue.
 * The elements are integers and the integer is assumed to represent 
 * the priority (larger integer is higher priority).
 */
template <typename ElemType>
class PQueue 
{
  public:

    /*
     * Constructor: PQueue
     * Usage: PQueue pq;
     *        PQueue *ppq = new PQueue;
     * ---------------------------------
     * Initializes a new pqueue to have no elements.  
     */
    PQueue(int (*cmp)(ElemType,ElemType) = OperatorCmp);


    /*
     * Destructor: ~PQueue
     * Usage: delete ppq;
     * ------------------
     * Deallocates all the storage associated with this pqueue.
     */
    ~PQueue();


    /*
     * Member function: isEmpty
     * Usage: if (pq.isEmpty()) . . .
     * -------------------------------
     * Returns true if this pqueue contains no elements.
     */
    bool isEmpty();


    /*
     * Member function: size
     * Usage: nElemes = pq.size();
     * ---------------------------
     * Returns number of elements contained in this pqueue.
     */
    int size();


    /*
     * Member function: enqueue
     * Usage: pq.enqueue(val);
     * -----------------------
     * Adds the specified element to this pqueue. No effort is made to
     * avoid duplicates. 
     */
    void enqueue(ElemType newElem);


    /*
     * Member function: eequeueMax
     * Usage: maxElem = pq.dequeueMax();
     * ---------------------------------
     * Removes the largest priority element from this pqueue and returns it.
     * If this pqueue is empty, this function raises an error.
     */
    ElemType dequeueMax();

  private:
    Vector<ElemType> entries;
    void swap(int indexA, int indexB);
    int (*cmpFn)(ElemType,ElemType);
};

#include "pqheap.cpp"

#endif