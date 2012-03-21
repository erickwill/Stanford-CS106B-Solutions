#include "genlib.h"
#include <iostream>
#include "vector.h"
#include "simpio.h"

int CountCriticalVotes(Vector<int> & blocks, int blockIndex);
int RecVote(Vector<int> &blocks, int index, 
            int sum, int &count, int vote, int totalVotes);

int main ()
{
    Vector<int> blocks;
    int blockIndex;
    while(true)
    {
        cout << "Enter vote blocks (enter 0 when finished): ";
        int num = GetInteger();
        if (num == 0) break;
        blocks.add(num);
    }
    while(true)
    {
        cout << "Enter block index (starting at 0): ";
        blockIndex = GetInteger();
        if (blockIndex < blocks.size()) break;
        else cout << "Invalid index" << endl;
    }
    int votes = CountCriticalVotes(blocks, blockIndex);
    cout << "Block index " << blockIndex << " has " << votes
        << " critical votes." <<endl;
    return 0;
}

int CountCriticalVotes(Vector<int> & blocks, int blockIndex)
{
    int vote = blocks[blockIndex];
    blocks.removeAt(blockIndex);
    int count = 0;
    int index = 0;
    int sum = 0;
      
    int totalVotes = 0;
    for (int i = 0; i < blocks.size(); i++) 
    {
      totalVotes += blocks[i];
    }

    return RecVote(blocks, index, sum, count, vote, totalVotes);
}

int RecVote(Vector<int> &blocks, int index, 
            int sum, int &count, int vote, int totalVotes)
{
    if (index >= blocks.size())
    {
        int candidateA = sum;
        int candidateB = totalVotes-sum;
        if ((candidateB > candidateA && candidateA+vote > candidateB) ||
            (candidateA > candidateB && candidateB+vote > candidateA))
            count++;
        return count;
    }
    // cast vote for A
    RecVote(blocks, index+1, sum+blocks[index], count, vote, totalVotes);
    // cast vote for B
    RecVote(blocks, index+1, sum, count, vote, totalVotes);
}
