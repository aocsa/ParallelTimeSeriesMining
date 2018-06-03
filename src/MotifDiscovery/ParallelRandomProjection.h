//
// Created by aocsa on 6/2/18.
//

#ifndef TSDISCOVERY_PARALLELRANDOMPROJECTION_H
#define TSDISCOVERY_PARALLELRANDOMPROJECTION_H

#pragma once

#include <vector>
#include <string>
#include <map>
#include <set>
#include "Word.h"
#include "MotifParameters.h"

#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include <tbb/concurrent_vector.h>
#include <tbb/concurrent_hash_map.h>
#include <tbb/task_scheduler_init.h>

using namespace tbb;

static tbb::concurrent_vector<Word> words_container;
typedef tbb::concurrent_hash_map< std::pair<int, int>, int> CollisionMap;
static CollisionMap collision_table;

struct OneRandomProjection{
    MotifParameters params;
    std::vector<int> cols ;

    OneRandomProjection() {
        int b = 7;
        std::set<int> cols_set;

        for (int i = 0; i < b; i++) {
            int tentativeColumn = nextInt(params.wordLength - 1);

            if ( cols_set.find(tentativeColumn) == cols_set.end() )
                cols_set.insert(tentativeColumn);
            else
                i--;
        }
        cols.insert(cols.begin(),  cols_set.begin(), cols_set.end () );

        //int vcols[] = {1, 2, 3, 4, 5, 6, 7};
        //std::vector<int> cols( vcols, vcols + sizeof(vcols) / sizeof(int) );

        printf("Mask columns: [");
        for (int i = 0; i < cols.size(); i++) {
            printf("%d, ", cols[i]);
        }
        printf("]\nWORD LIST SIZE:%d \n", words_container.size());
    }

    int nextInt(int rangeEnd) const {
        int rangeStart = 0;
        return rangeStart + rand() % ((rangeEnd - rangeStart) + 1);

    }

    void operator () (blocked_range<int> r) const {
        for (int i = r.begin(); i != r.end(); ++i) {
            Word wiMask = words_container.at(i).mask(cols);
            for (int j = 0; j < words_container.size(); j++) {
                if (i == j)
                    continue;

                Word wj  = words_container.at(j);
                if (wiMask.maskEquals(wj, cols)) {
                    // WE have a match, increment collision matrix positions
                    std::pair<int, int> match = std::make_pair(i, j);

                    CollisionMap::accessor iter;
                    //CollisionMap::iterator iter =  COLLISION.find(match);

                    if ( collision_table.find(iter, match) 	) {
                        //printf("%d, %d \n", match.first, match.second);
                        iter->second += 1;
                        //COLLISION[ match ] =  count + 1;
                    } else {
                        //COLLISION.insert( std::make_pair( match, 1 ) );
                        CollisionMap::accessor a;
                        collision_table.insert( a, match );
                        a->second += 1;
                    }
                } else {
                    // We DONT have a match, keep on
                }

            }
        }
    }
};
class ParallelRandomProjection
{
private:
    std::string db;
    MotifParameters params;
    std::vector<Word> words;
    //typedef std::map< std::pair<int, int>, int> CollisionMap;

public:
    //CollisionMap	collision;

    ParallelRandomProjection(void)
    {
    }

    ~ParallelRandomProjection(void)
    {
    }

    void addWord(const Word& iSaxWord) {
        //doStore(iSaxWord, order);
        //words.push_back(iSaxWord);

        words_container.push_back(iSaxWord);
    }



    void randomProjection() {
        for (int iterations = 0; iterations < params.iterations; iterations++) {
            //randomProjectionOneIteration();
            parallel_for(blocked_range<int>(0, params.iterations), OneRandomProjection());
        }
    }

    int nextInt(int rangeEnd) {
        int rangeStart = 0;
        return rangeStart + rand() % ((rangeEnd - rangeStart) + 1);

    }



    void show1MotifResult()
    {
        std::set<int> items;
        for (int iter = 0 ; iter < 5; iter++) {
            int WORDS = 10000;
            std::pair<int, int> maxp;
            int bestMotifSoFar = 0;
            int item = 0;
            std::vector<int> bestMotifLocationSoFar;
            for (int i = 0; i < WORDS; i++) {
                int counter = 0;
                std::multimap< int, int , std::greater <int> > pointers;
                for(int j = 0; j < WORDS; j++) {
                    CollisionMap::accessor iter;
                    if ( collision_table.find(iter, std::make_pair(i,j) ) ) {
                        int count = iter->second;
                        counter += count;
                        pointers.insert( std::make_pair(count, j) );
                    }
                }
                if(items.find(i) == items.end()) {
                    if( counter > bestMotifSoFar ) {
                        item = i;
                        bestMotifSoFar = counter;
                        bestMotifLocationSoFar.clear();
                        bestMotifLocationSoFar.push_back(i);
                        std::multimap< int, int, std::greater <int> >::iterator iter = pointers.begin();
                        for ( ; iter != pointers.end(); iter++) {
                            bestMotifLocationSoFar.push_back(iter->second);
                        }
                    }
                }
            }
            items.insert(item);
            int topK = 15;
            printf("SIZE: %d\n", WORDS * WORDS);
            printf("1-MOTIF: " );
            for (int t = 0; t < std::min((int)topK, (int)bestMotifLocationSoFar.size()); t++) {
                printf("%d, ",  bestMotifLocationSoFar[t]);
            }
            printf("\n");
        }
    }

    void showResult()
    {
        int WORDS = 10000;
        int bestMotifSoFar = 0;
        std::vector<int> bestMotifLocationSoFar;
        for (int i = 0; i < WORDS; i++) {
            int counter = 0;
            std::multimap< double, int > pointers;
            for(int j = 0; j < WORDS; j++) {
                CollisionMap::accessor iter;
                if ( collision_table.find(iter, std::make_pair(i,j) ) ) {
                    //printf("row: < %d, %d > : %d \n" ,  i,  j, iter->second);
                }
            }
        }
        std::pair<int, int> maxp;
        int max_count = 0;
        CollisionMap::iterator iter = collision_table.begin();
        for (  ; iter != collision_table.end() ; iter++) {
            if (iter->second > max_count) {
                max_count = iter->second; //col.get(pair);
                maxp = iter->first;
            }
        }
        printf("SIZE: %d\n", collision_table.size());
        printf("1-MOTIF: < %d, %d > : %d \n" ,  maxp.first,  maxp.second, max_count);
    }
};



#endif //TSDISCOVERY_PARALLELRANDOMPROJECTION_H
