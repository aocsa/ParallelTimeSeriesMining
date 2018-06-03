

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

#include "MotifDiscovery/RandomProjection.h"
#include "MotifDiscovery/ParallelRandomProjection.h"
#include "MotifDiscovery/MotifParameters.h"

void testParallelRandomProjection(){
    clock_t begin = clock();
    std::string db = "/home/aocsa/blazingdb/CPP_TEMPLATES/ParallelTSDiscovery/data/synth16d.txt";
    ParallelRandomProjection rp;
    MotifParameters params;

    std::ifstream ifs  (db.c_str());

    std::string line;

    int iter = 0;
    while ( getline(ifs, line) ) {
        std::vector<double> ts;
        double value;

        std::stringstream ss (line);
        while ( ss >>  value)
            ts.push_back(value);

        Word isaxWord(ts, params.wordLength, params.initialCardinality);
        rp.addWord(isaxWord);

        //	std::cout << isaxWord.toString() << std::endl;
        //std::cout << iter++ << std::endl;
        iter++ ;
    }
    rp.randomProjection();
    rp.showResult();
    clock_t end = clock();
    rp.show1MotifResult();
    std::cout << "Exec time (ms): " << ( end - begin ) << std::endl;
}


void testRandomProjection() {
    clock_t begin = clock();
    std::string db = "/home/aocsa/blazingdb/CPP_TEMPLATES/ParallelTSDiscovery/data/synth16d.txt";
    RandomProjection rp;
    MotifParameters params;

    std::ifstream ifs  (db.c_str());

    std::string line;

    int iter = 0;
    while ( getline(ifs, line) ) {
        std::vector<double> ts;
        double value;
        std::stringstream ss (line);
        while ( ss >>  value) {
            ts.push_back(value);
        }
        Word isaxWord(ts, params.wordLength, params.initialCardinality);
        rp.addWord(isaxWord);

        //std::cout << isaxWord.toString() << std::endl;
        //std::cout << iter++ << std::endl;
    }
    rp.randomProjection();
    rp.showResult();
    clock_t end = clock();
    std::cout << "Exec time (ms): " << ( end - begin ) << std::endl;
}

int main(int argc, char* argv[]) {

    //testRandomProjection();

    testParallelRandomProjection();

    return 0;
}