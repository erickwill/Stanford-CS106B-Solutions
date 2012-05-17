/*
 * File: pqueue.h
 * --------------
 * Defines the interface for the priority queue class. 
 * 
 * Julie Zelenski, CS106, Fall 2007
 */
 
 
#ifndef _pqheap_h
#define _pqheap_h

#include "genlib.h"
#include "vector.h"
#include "disallowcopy.h"

/*
 * Class: PQueue
 * -------------
 * This is the class for a priority queue.  This is not
 * simple FIFO queue, it is a priority queue, where elements are
 * retrieved in order of priority, not just by longevity in queue.
 * The elements are integers and the integer is assumed to represent 
 * the priority (larger integer is higher priority).
 */
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
    PQueue();


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
    void enqueue(int newElem);


    /*
     * Member function: eequeueMax
     * Usage: maxElem = pq.dequeueMax();
     * ---------------------------------
     * Removes the largest priority element from this pqueue and returns it.
     * If this pqueue is empty, this function raises an error.
     */
    int dequeueMax();


    /*
     * Member function: bytesUsed
     * Usage: numBytes = pq.bytesUsed();
     * ----------------------------------
     * This function would not usually be included as part of the class,
     * but this is here as part of evaluating the tradeoffs betweem
     * implementations. Given a pqueue, this function counts up
     * and return the total amount of space used given its
     * current contents.
     */
    int bytesUsed();
    
    
    /*
     * Member function: implementationName
     * Usage: cout << pq.implementationName();
     * ---------------------------------------
     * This operation would not usually be included as part of the class
     * class, but is included to help with analyzing and reporting results.
     * This member function returns a string that describes the PQueue
     * implementation strategy ("sorted linked list", "vector", etc.).
     */
    string implementationName();


    /*
     * Member function: printDebuggingInfo
     * Usage: pq.printDebuggingInfo();
     * -------------------------------
     * This operation would not usually be included as part of the class,
     * but is included to give you a hook to put any helpful debugging
     * print code (for example, something that prints the goopy details of
     * the internal structure). You don't need to implement this routine and can
     * ignore it entirely, but you may find it helpful to use as you are
     * doing development.
     */
    void printDebuggingInfo();
    void print();

  private:
  
    // If implemented using Vector data mamber, default memberwise copy 
    // works fine, but if implemented as linked list, (ie pointer data member)
    // copying would create unintended sharing.
    // It's fine is to disallow copying for all implementations as
    // a precaution
    DISALLOW_COPYING(PQueue)

      /* This is the representation for the unsorted vector.
       * You will need to update this as you change representations. */
    Vector<int> entries;
    void swap(int indexA, int indexB);

};


#endif