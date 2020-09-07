// File: CacheMemory.h
// Author(s): Clayton Stuhrenberg, Blaine Britton
// Date: 04/30/2020
// Section: 508, 510
// Description:
// The header file for the CacheMemory data structure.
// Implements structs for the sets and lines of the cache.
// Has a private RAM data member that accesses memory.
#ifndef CACHEMEMORY_H
#define CACHEMEMORY_H

#include <iostream>
#include <string>
#include "RAM.h"
#include <queue>

using namespace std;

class CacheMemory
{
private:
	RAM ram_instance;

	struct blocks
	{
	    int size;
	    string tag;
	    int valid, dirty;
	    vector<string> block;
	    blocks() {}
        blocks(int s) : size(s), tag("00"), valid(0), dirty(0) {
            //cout << "new block" << endl;
            for (int i = 0; i < size; ++i){
                block.push_back("00");
            }
            //cout << "made block" << endl;
        }

		void setValid(int v) { valid = v; }
		void setDirty(int d) { dirty = d; }
		void setTag(string t) { tag = t; }

        string & operator[](int i){
            return block[i];
        }
	};

	struct set
	{
        int size;
        vector<blocks> lines;
        vector<int> leastFrequentlyUsed;
        queue<int> leastRecentlyUsed;
        //static const int LFU_WAIT_TIME = 4;
        set(int s, int b) : size(s)
		{
            //cout << "new set" << endl;
            //lines = new blocks[size];
            for (int i = 0; i < size; ++i)
			{
                lines.push_back(blocks(b));
                leastRecentlyUsed.push(i);
                leastFrequentlyUsed.push_back(0);
            }
            //cout << "made set" << endl;
        }

		void flushLFU()
		{
			for (int i = 0; i < leastFrequentlyUsed.size(); i++)
				leastFrequentlyUsed[i] = 0;
		}

		void flushLRU()
		{
			for (int i = 0; i < leastRecentlyUsed.size(); i++){
				leastRecentlyUsed.pop();
				leastRecentlyUsed.push(i);
			}
		}

        blocks& getLFU(){
            int min = 99999, index = -1;
            for (int s = 0; s < size; ++s){
                if (leastFrequentlyUsed[s] < min){
                    index = s;
                    min = leastFrequentlyUsed[s];
                }
            }
            return lines[index];
        }

        int getLFUindex(){
            int min = 99999, index = -1;
            for (int s = 0; s < size; ++s){
                if (leastFrequentlyUsed[s] < min){
                    index = s;
                    min = leastFrequentlyUsed[s];
                }
            }
            return index;
        }

        void incLFUval(const int & i){
            ++leastFrequentlyUsed[i];
        }

        void resetLFU(const int & i){
            leastFrequentlyUsed[i] = 0;
        }

        blocks& getLRU()
		{
            int pos = leastRecentlyUsed.front();
            leastRecentlyUsed.pop();
            leastRecentlyUsed.push(pos);
            return lines[pos];
        }

        int getLRUindex(){
            return leastRecentlyUsed.front();
        }

        blocks& operator[](int i)
		{
            return lines[i];
        }

        void reorderLRU(const int & i)
		{
            queue<int> tempQ;
		    while (!leastRecentlyUsed.empty()){
                int val = leastRecentlyUsed.front();
                leastRecentlyUsed.pop();
                if (val == i) {
                    continue;
                }
                tempQ.push(val);
		    }
		    tempQ.push(i);
		    leastRecentlyUsed = move(tempQ);
        }
	};

    vector<set> sets;
	// Cache size: the actual size or capacity of a cache, is the aggregate size of all the cache blocks.
	// The unit for the cache size is in Bytes. The allowed cache size should be 8 to 256 Bytes.
	int cache_size;

	// Data block size: the size of a data block is the number of bytes that a block can contain.
	int data_block_size;

	// set size: the number of sets contained in cache
	int set_size;

	// Associativity: A n-way set associative cache holds n lines per set.
	// The user can only choose from a  1-way, 2-way or 4-way set-associative cache
	int associativity;

	// Replacement policy: a cache replacement policy defines how to replace a cache entry following a cache miss.
	// The user can only choose from the Random replacement (RR) (1) or the Least Recently Used (LRU) (2)  policy.
	// BONUS: added Least Frequently Used (LFU) (3)
	int replacement;

	// Write hit policy: a write hit policy defines where to write the data when an address is a hit.
	// The user can only choose from a write-through or write-back policy.
	//    * write-through (1): write the data in both the block in the cache and the block in RAM.
	//    * write-back (2): write the data only in the block in the cache.
	int write_hit;

	// Write miss policy: a write miss policy defines where to write the data when an address is a miss.
	// The user can only choose from a write-allocate or no write-allocate policy.
	//    * write-allocate (1): load the block from RAM and write it in the cache.
	//    * no write-allocate (2): write the block in RAM and do not load it in the cache.
	int write_miss;

	int set_bits, block_offset_bits, tag_bits;

	static int cache_hits;
	static int cache_misses;

public:

	CacheMemory(const string & filepath)
	{
        ram_instance.setFile(filepath);
        ram_instance.initRam();
	}
	bool configureCache();
	string simulateCache();
	void cacheRead(string hex_address);
	void cacheWrite(string hex_address, string byte);
	void cacheFlush();
	void cacheView();
	void emptyReplacement(set & active_set,blocks *& tempBlock,int & eviction_line);
	void randomReplacement(set & active_set,blocks *& tempBlock,int & eviction_line);
	void leastRecentlyUsedReplacement(set & active_set,blocks *& tempBlock,int & eviction_line);
	void leastFrequentlyUsedReplacement(set & active_set,blocks *& tempBlock,int & eviction_line);
	void writeBack(blocks *& block, int set_index);
	void cacheDump();
	string updateBlock(string hex_address, string tag, blocks *& tempBlock, int block_offset);
};

#endif
