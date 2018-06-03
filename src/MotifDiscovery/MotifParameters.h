//
// Created by aocsa on 6/2/18.
//

#ifndef TSDISCOVERY_MOTIFPARAMETERS_H
#define TSDISCOVERY_MOTIFPARAMETERS_H

#pragma once

enum ETSDBInputType {
    FULL, //each time series in the DB is scanned, no subsequence
    FULLONETSPERLINE,
    SLIDING //A sliding window approach is followed.
};


class MotifParameters  {
public:
    int wordLength ;
    int initialCardinality ;
    int maxCardinality;
    ETSDBInputType inputType ;
    int motifLength ;
    double memoryThreshold ;
    bool locationOn ;
    int iterations;
    int bMin;
    int topk;
    bool spaceSaving;


    MotifParameters(void)
    {
        wordLength = 8;
        initialCardinality = 4;
        inputType = ETSDBInputType::FULLONETSPERLINE;
        motifLength = 32;
        iterations  = 10;
        memoryThreshold = 0.8;
        bMin= 2;

        maxCardinality= 64;
        topk =  10;
        spaceSaving = false;
    }

    ~MotifParameters(void)
    {
    }
};



#endif //TSDISCOVERY_MOTIFPARAMETERS_H
