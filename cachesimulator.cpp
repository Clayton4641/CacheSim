// File: CacheSimulator.cpp
// Author(s): Clayton Stuhrenberg, Blaine Britton
// Date: 04/30/2020
// Section: 508, 510
// Description:
// e.g. The content of this file implements the main loop of the cache sim
#include <iostream>
#include <fstream>
#include "CacheMemory.h"
#include "RAM.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc < 2){
        cout << "No arg given" << endl;
        return 0;
    }
    CacheMemory c(argv[1]);
	bool configSuc = c.configureCache();
    if (configSuc)
        while (c.simulateCache() != "quit") {}

	return 0;
}
