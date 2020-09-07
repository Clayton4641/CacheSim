// File: CacheMemory.cpp
// Author(s): Clayton Stuhrenberg, Blaine Britton
// Date: 04/30/2020
// Section: 508, 510
// Description:
// Where the magic happens. Implements CacheMemory functions, manipulates cache.
// Makes calls to RAM for necessary memory changes.
#include "CacheMemory.h"
#include <iostream>
#include <string>
#include <math.h>
#include <sstream>

int CacheMemory::cache_hits = 0;
int CacheMemory::cache_misses = 0;


/********************  CONVERSION FUNCTIONS  ********************/
int hexStoint(const string& h){
    stringstream ss;
    ss << hex << h;
    int val;
    ss >> val;
    return val;
}

string inttoShex(const int & h){
    stringstream ss;
    ss << hex << uppercase << h;
    string val;
    ss >> val;
    if (h < 16)
        val = "0"+val;
    return val;
}

string hexToBinary(string hex)
{
	string binary = "";
	hex = hex.substr(2);
	for (int i = 0; i < hex.length(); i++)
	{
		char c = hex[i];
		switch (toupper(c))
		{
		case '0': binary += "0000"; break;
		case '1': binary += "0001"; break;
		case '2': binary += "0010"; break;
		case '3': binary += "0011"; break;
		case '4': binary += "0100"; break;
		case '5': binary += "0101"; break;
		case '6': binary += "0110"; break;
		case '7': binary += "0111"; break;
		case '8': binary += "1000"; break;
		case '9': binary += "1001"; break;
		case 'A': binary += "1010"; break;
		case 'B': binary += "1011"; break;
		case 'C': binary += "1100"; break;
		case 'D': binary += "1101"; break;
		case 'E': binary += "1110"; break;
		case 'F': binary += "1111"; break;
		}
	}
	return binary;
}

string binaryToHex(string binary)
{
	string hex = "";
	while (binary.length() % 4 != 0)
		binary = "0" + binary;

	for (int i = 0; i < binary.length() / 4; i++)
	{
		string b = binary.substr(i * 4, 4);

		if (b == "0000") hex += "0";
		else if (b == "0001") hex += "1";
		else if (b == "0010") hex += "2";
		else if (b == "0011") hex += "3";
		else if (b == "0100") hex += "4";
		else if (b == "0101") hex += "5";
		else if (b == "0110") hex += "6";
		else if (b == "0111") hex += "7";
		else if (b == "1000") hex += "8";
		else if (b == "1001") hex += "9";
		else if (b == "1010") hex += "A";
		else if (b == "1011") hex += "B";
		else if (b == "1100") hex += "C";
		else if (b == "1101") hex += "D";
		else if (b == "1110") hex += "E";
		else if (b == "1111") hex += "F";
	}
	return hex;
}

int binaryToDecimal(string binary)
{
	int decimal = 0;
	for (int n = 0; n < binary.length(); n++)
	{
		char c = binary[binary.length() - 1 - n];
		if (c == '1')
			decimal += (int)pow(2, n);

	}
	return decimal;
}

string decimalToBinary(int decimal)
{
	string binary = "";
	int i = 0;
	while (decimal > 0)
	{
		binary = to_string(decimal % 2) + binary;
		decimal /= 2;
		i++;
	}
	while (binary.length() < 8)
		binary = "0" + binary;
	return binary;
}

bool CacheMemory::configureCache()
{
	int rep, wh, wm;
	cout << "configure the cache:" << endl;
	cout << "cache size: ";
	cin >> cache_size;
	if ((cache_size < 8 || cache_size > 256) || floor(log2(cache_size)) != ceil(log2(cache_size))) {
        cout << "Invalid input" << endl;
		return false;
	}
	cout <<	"data block size: ";
	cin >> data_block_size;
	if (floor(log2(data_block_size)) != ceil(log2(data_block_size))){
        cout << "Invalid input" << endl;
        return false;
	}
	cout << "associativity: ";
	cin >> associativity;
	if (!(associativity == 1 || associativity == 2 || associativity == 4) || data_block_size*associativity > cache_size) {
		cout << "Invalid input" << endl;
		return false;
	}
	cout << "replacement policy: ";
	cin >> rep;
	switch (rep)
	{
	case 1: replacement = 1; break;
	case 2: replacement = 2; break;
	case 3: replacement = 3; break;
	default:
	    cout << "Invalid input" << endl;
	    return false;
	}

	cout << "write hit policy: ";
	cin >> wh;
	switch (wh)
	{
	case 1: write_hit = 1; break;
	case 2: write_hit = 2; break;
	default:
	    cout << "Invalid input" << endl;
	    return false;
	}
	cout << "write miss policy: ";
	cin >> wm;
	switch (wm)
	{
	case 1: write_miss = 1; break;
	case 2: write_miss = 2; break;
	default:
	    cout << "Invalid input" << endl;
	    return false;
	}

	// set the size of cache
	set_size = (int)floor(cache_size / (associativity * data_block_size));
    for (int i = 0; i < set_size; ++i) {
        sets.push_back(set(associativity, data_block_size));
        //cout << "set made" << endl;
    }
    set_bits = (int)log2(set_size);
	block_offset_bits = (int)log2(data_block_size);
	tag_bits = 8 - set_bits - block_offset_bits;

	cout << "cache successfully configured!" << endl;
	return true;
}

string CacheMemory::simulateCache()
{
	string action;
	cout << "*** Cache simulator menu ***" << endl;
	cout << "type one command:" << endl;
	cout << "1. cache-read" << endl;
	cout << "2. cache-write" << endl;
	cout << "3. cache-flush" << endl;
	cout << "4. cache-view" << endl;
	cout << "5. memory-view" << endl;
	cout << "6. cache-dump" << endl;
	cout << "7. memory-dump" << endl;
	cout << "8. quit" << endl;
	cout << "****************************" << endl;
	cin >> action;

	if (action == "cache-read") {
		string address;
		cin >> address;
		cacheRead(address);
	}
	else if (action == "cache-write") {
		string address, byte;
		cin >> address >> byte;
		cacheWrite(address, byte);
	}
	else if (action == "cache-flush") {
		cacheFlush();
	}
	else if (action == "cache-view") {
		cacheView();
	}
	else if (action == "memory-view") {
		ram_instance.memoryView();
	}
	else if (action == "cache-dump") {
		cacheDump();
	}
	else if (action == "memory-dump") {
		ram_instance.memoryDump();
	}

	return action;
}

void CacheMemory::cacheRead(string hex_address)
{
	string b_address = hexToBinary(hex_address);
	string tag = binaryToHex(b_address.substr(0, tag_bits));
	int set_index = binaryToDecimal(b_address.substr(tag_bits, set_bits));
	int block_offset = binaryToDecimal(b_address.substr((tag_bits + set_bits)));

	int eviction_line = -3;
	bool hit = false;
	string data_returned;

	set & s = sets[set_index]; // make a reference to the current set so that it is easy to write
	for (int a = 0; a < s.size; ++a) {
        if(s[a].tag == tag && s[a].valid == 1) {
            hit = true;
            ++cache_hits;
            eviction_line = -1;
            data_returned = s[a][block_offset];
            if (replacement == 2)
                s.reorderLRU(a); // we must reorder the last recently used queue
            else if (replacement == 3)
                s.incLFUval(a); // if using LFU update the blocks value
        }
	}

	if(!hit)
	{
        ++cache_misses;
        blocks* temp;
        emptyReplacement(s, temp, eviction_line);
        if (eviction_line == -2) { // there were no empty lines in the set
            if (replacement == 1) { // random replacement
                randomReplacement(s,temp,eviction_line);
            }
            else if (replacement == 2) { // LRU
                leastRecentlyUsedReplacement(s,temp,eviction_line);
            }
            else if (replacement == 3) { // LFU
                leastFrequentlyUsedReplacement(s,temp,eviction_line);
            }

			if (temp->dirty == 1 && write_hit == 2)
				writeBack(temp, set_index);
        }

		data_returned = updateBlock(hex_address, tag, temp, block_offset);
	}

	cout << "set:" << set_index << endl;
	cout << "tag:" << tag << endl;
	cout << "hit:" << (hit ? "yes" : "no") << endl;
	cout << "eviction_line:" << eviction_line << endl;
	cout << "ram_address:" << (hit? "-1" : hex_address) << endl;
	cout << "data:" << "0x" << data_returned << endl;

}

void CacheMemory::cacheWrite(string hex_address, string byte)
{
	byte = byte.substr(2); // get rid of "0x"
	string b_address = hexToBinary(hex_address);
	string tag = binaryToHex(b_address.substr(0, tag_bits));
	int set_index = binaryToDecimal(b_address.substr(tag_bits, set_bits));
	int block_offset = binaryToDecimal(b_address.substr(tag_bits + set_bits, block_offset_bits));

	int eviction_line = -3;
	bool hit = false;
	int dirty = 0;

	set & s = sets[set_index]; // make a reference to the current set so that it is easy to write
	for (int e = 0; e < s.size; ++e) {
		if (s[e].tag == tag && s[e].valid == 1) {
			hit = true;
			++cache_hits;
			eviction_line = -1;
			if (replacement == 2)
				s.reorderLRU(e); // we must reorder the last recently used queue
			else if (replacement == 3)
				s.incLFUval(e); // if using LFU update the blocks value

			// update cache
			s[e][block_offset] = byte;

			if (write_hit == 1) {	// write through
				// update ram
				ram_instance.set(hex_address, byte);
			}
			else if (write_hit == 2) {	// write back
				// set dirty bit
				s[e].setDirty(1);
				dirty = 1; // for printing purposes
			}
		}
	}

	if (!hit)
	{
		++cache_misses;
		if (write_miss == 1) // allocate
		{
			// fetch block from RAM and store in cache
			blocks* temp;
			emptyReplacement(s, temp, eviction_line);
			if (eviction_line == -2)
			{
				if (replacement == 1) { // random replacement
					randomReplacement(s, temp, eviction_line);
				}
				else if (replacement == 2) { // LRU
					leastRecentlyUsedReplacement(s, temp, eviction_line);
				}
				else if (replacement == 3) { // LFU
					leastFrequentlyUsedReplacement(s, temp, eviction_line);
				}

				if (temp->dirty == 1 && write_hit == 2)
					writeBack(temp, set_index);
			}
			// update cache block
			updateBlock(hex_address, tag, temp, block_offset);
			// update cache single byte
			temp->block[block_offset] = byte;

			if (write_hit == 1) {	// write through
				// update ram
				ram_instance.set(hex_address, byte);
			}
			else if (write_hit == 2) {	// write back
				// set dirty bit
				temp->setDirty(1);
				dirty = 1; // for printing purposes
			}
		}
		if (write_miss == 2) // no allocate
		{
			// update ram
			ram_instance.set(hex_address, byte);
			eviction_line = -1;
		}
	}


	cout << "set:" << set_index << endl;
	cout << "tag:" << tag << endl;
	cout << "write_hit:" << (hit ? "yes" : "no") << endl;
	cout << "eviction_line:" << eviction_line << endl;
	cout << "ram_address:" << (hit? "-1" : hex_address) << endl;
	cout << "data:" << "0x" << byte << endl;
	cout << "dirty_bit:" << dirty << endl;
}


string CacheMemory::updateBlock(string hex_address, string tag, blocks *& temp, int block_offset)
{
	int decimal_address = hexStoint(hex_address);
	decimal_address -= decimal_address % data_block_size;
	for (int cache_i = 0; cache_i < data_block_size; cache_i++) {
		temp->block[cache_i] = ram_instance.read(decimal_address + cache_i);
	}
	temp->valid = 1;
	temp->dirty = 0; // dirty gets set back to 0 since cache memory is the same as RAM
	temp->tag = tag;

	return temp->block[block_offset]; // the block to be returned
}

void CacheMemory::cacheFlush()
{
    for(int s = 0; s < set_size; ++s){
        for (int a = 0; a < sets[s].size; ++a) {
			blocks & line = sets[s][a];
            int valid = line.valid;
            int dirty = line.dirty;
            if (valid == 1 && dirty == 1) {
                string tag = line.tag;
                int address = hexStoint(tag) + s;

                for (int b = 0; b < data_block_size; ++b){
                    ram_instance.set(b+address, line[b]);
                }
            }
			line.setValid(0);
			line.setDirty(0);
			line.setTag("00");

            for (int b = 0; b < data_block_size; ++b)
                line[b] = "00";
        }
		if (replacement == 2) // LRU
			sets[s].flushLRU();
		if (replacement == 3) // LFU
			sets[s].flushLFU();
    }
	cout << "cache_cleared" << endl;
}

void CacheMemory::cacheView()
{
	cout << "cache_size:" << cache_size << endl;
	cout << "data_block_size:" << data_block_size << endl;
	cout << "associativity:" << associativity << endl;
	cout << "replacement_policy:" << replacement << endl;
	cout << "write_hit_policy:" << write_hit << endl;
	cout << "write_miss_policy:" << write_miss << endl;
	cout << "number_of_cache_hits:" << cache_hits << endl;
	cout << "number_of_cache_misses:" << cache_misses << endl;
	cout << "cache_content:" << endl;
	for(int s = 0; s < set_size; ++s) {
        for(int e = 0; e < associativity; ++e) {
			// tag should be a byte (two characters)
			string tag = sets[s][e].tag;
			if (tag.length() == 1)
				tag = "0" + tag;
            cout << sets[s][e].valid << " " << sets[s][e].dirty << " " << tag << " ";
            for(int b = 0; b < data_block_size; ++b){
                cout << sets[s][e][b] << " ";
            }
            cout << endl;
        }
	}
}

void CacheMemory::emptyReplacement(set & active_set, blocks *& tempBlock, int & eviction_line){
    for (int a = 0; a < active_set.size; ++a){
        if (active_set[a].valid == 0){
            eviction_line = a;
            tempBlock = &active_set[a];
            if (replacement == 2)
                active_set.reorderLRU(a); // reorder if LRU is used
            if (replacement == 3)
                active_set.resetLFU(a); // since it is gonna be a new block reset it if LFU
            return;
        }
    }
    eviction_line = -2;
}

void CacheMemory::randomReplacement(set & active_set, blocks *& tempBlock, int & eviction_line)
{
    int index = rand() % active_set.size;
    eviction_line = index;
    tempBlock = &active_set[index];
}

void CacheMemory::leastRecentlyUsedReplacement(set & active_set,blocks *& tempBlock,int & eviction_line)
{
    eviction_line = active_set.getLRUindex();
    tempBlock = &active_set.getLRU();
    //active_set.reorderLRU(eviction_line); //getLRU does this for us
}

void CacheMemory::leastFrequentlyUsedReplacement(set & active_set,blocks *& tempBlock,int & eviction_line)
{
    eviction_line = active_set.getLFUindex();
    tempBlock = &active_set.getLFU();
    active_set.resetLFU(eviction_line);
}

void CacheMemory::writeBack(blocks *& block, int set_index)
{
	string b_address = hexToBinary(block->tag) + decimalToBinary(set_index);
	// the purpose of the while loop is to start the hex_address at the lowest block index (0)
	// while maintaining the correct number of block offset bits.
	while (b_address.length() < 8)
		b_address += "0";
	int decimal_address = binaryToDecimal(b_address);
	for (int cache_i = 0; cache_i < data_block_size; cache_i++)
	{
		ram_instance.set(decimal_address + cache_i, block->block[cache_i]);
	}
	block->setDirty(0);
}

void CacheMemory::cacheDump()
{
	ofstream out("cache.txt");
	for (int s_index = 0; s_index < set_size; s_index++)	{
		for (int e_index = 0; e_index < associativity; e_index++)	{
			for (int b_index = 0; b_index < data_block_size; b_index++)	{
				out << sets[s_index][e_index][b_index] << " ";
			}
			out << endl;
		}
	}
}
