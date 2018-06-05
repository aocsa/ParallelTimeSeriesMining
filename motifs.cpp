

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdio>

#include "MotifDiscovery/RandomProjection.h"
#include "MotifDiscovery/ParallelRandomProjection.h"
#include "MotifDiscovery/MotifParameters.h"

void testParallelRandomProjection(std::string db){
    clock_t begin = clock();
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
    //rp.showResult();
    clock_t end = clock();
    //rp.show1MotifResult();
    double time = (end - begin) / 1000.0;
    std::cout << "Exec time (ms): " << ( time ) << std::endl;

    char buffer_tmp[100];
    std::string nameFile = db + "." + "SERIAL.MOTIFS";
    std::ofstream save_file(nameFile.c_str());
    sprintf(buffer_tmp,"Random Projections Motifs time exec: %0.6lf\n", (double)time);
    printf(buffer_tmp);
    save_file << buffer_tmp;
    save_file.close();
}


void testRandomProjection(std::string db) {
    clock_t begin = clock();
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
    //rp.showResult();
    clock_t end = clock();
    double time = (end - begin) / 1000.0;

    std::cout << "Exec time (ms): " << ( time ) << std::endl;

    char buffer_tmp[100];
    std::string nameFile = db + "." + "SERIAL.MOTIFS";
    std::ofstream save_file(nameFile.c_str());
    sprintf(buffer_tmp,"Random Projections Motifs time exec: %0.6lf\n", (double)time);
    printf(buffer_tmp);
    save_file << buffer_tmp;
    save_file.close();
}


void usage(char *programName) {
    printf("Usage: %s   serial|parallel data_set_file\n", programName);
}

int main(int nargs, char* args[]) {

    if (nargs < 2) {
        usage(args[0]);
        exit(1);
    }
    std::string type =  args[1];
    std::string db =  args[2];

    if (type == "serial")
        testRandomProjection(db);
    else
        testParallelRandomProjection(db);

    return 0;
}