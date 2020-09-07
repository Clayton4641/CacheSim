// File: RAM.h
// Author(s): Clayton Stuhrenberg, Blaine Britton
// Date: 04/30/2020
// Section: 508, 510
// Description:
// e.g. The content of this file implements the RAM class
#ifndef RAMH
#define RAMH

#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>


//Ram class to hold main memory
//built from the stl vector class
class RAM {
private:
	std::vector<std::string> mem;
	std::string file;
	int hexSToInt(const std::string & hex){
        std::stringstream ss;
        ss << std::hex << hex;
        int val;
        ss >> val;
        return val;
	}
	std::string intToSHex(const int & val){
	    std::stringstream ss;
        ss << std::hex << std::uppercase << val;
        std::string hex;
        ss >> hex;
        if (val < 16)
            hex = "0"+hex;
        return hex;
	}
public:
    //The assignment has the memory at a default size of 256 (bytes is simulation) but a option to change the size in the constructor is there
	RAM(const std::string& filepath, const int size = 256) : file(filepath) { mem.resize(size); }
	RAM() : file("") { mem.resize(256); }

	// method to initialize the RAM, uses the given file from the constructor
	// returns a boolean to flag if there was an error or not
	bool initRam() {
		std::ifstream input(file); //open file

		if (input.fail()) {
			std::cout << "Error loading file data into RAM" << std::endl; // might remove error message
			return false;
		}
        std::cout << "*** Welcome to the cache simulator ***" << std::endl;
        std::cout << "initialize the RAM:" << std::endl;
        std::cout << "init-ram 0x" << intToSHex(0) << " 0x" << intToSHex(mem.size()-1) << std::endl;
		std::string line;
		for (size_t i = 0; i<mem.size() && input >> line; ++i) { // run till either you fill the memory vector or run out of data
			//int val = hexSToInt(line);
			mem[i] = line;
		}
		std::cout << "ram successfully initialized!" << std::endl;
		return true; // memory loaded successfully
	}

	void setFile(const std::string & path){file = path;} // set a new data path for file

	// resize the memory vector
	// if i < size; the only the first i elements remain
	void resize(const int & i) {mem.resize(i);}

    void set(const std::string & address, const std::string & value){
        int index = hexSToInt(address);
        if (index >= 0 && index < mem.size())
            mem[index] = value;
        else
            std::cout << "Invalid RAM Index" << std::endl;
    }

    void set(const int & address, const std::string & value){
        int index = address;
        if (index >= 0 && index < mem.size())
            mem[index] = value;
        else
            std::cout << "Invalid RAM Index" << std::endl;
    }

    std::string read(const std::string & address){
        int index = hexSToInt(address);
        if (index >= 0 && index < mem.size())
            return mem[index];
        else
            std::cout << "Invalid RAM Index" << std::endl;
            return "";
    }

    std::string read(const int & address){
        int index = address;
        if (index >= 0 && index < mem.size())
            return mem[index];
        else
            std::cout << "Invalid RAM Index" << std::endl;
            return "";
    }

    void memoryView(){
        using namespace std;
        cout << "memory_size:" << mem.size() << endl;
        cout << "memory_content:" << endl;
        cout << "Address:Data" << endl;
        int ramsize = mem.size();
        for(int i = 0;i<ramsize;){
            cout << "0x" << intToSHex(i) << ":";
            for (int lim = i+8;i<lim && i<ramsize;++i)
                cout << mem[i] << " ";
            cout << endl;
        }
    }

    void memoryDump(){
        std::ofstream out("ram.txt");
        for(int i = 0;i<mem.size();++i)
            out << mem[i] << "\n";
    }


};

#endif // RAM
