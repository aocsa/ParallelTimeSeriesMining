//
// Created by aocsa on 6/2/18.
//

#ifndef TS_RANDOMPROJECTION_H
#define TS_RANDOMPROJECTION_H


#include <vector>
#include <string>
#include <map>
#include <set>

#include "Word.h"
#include "MotifParameters.h"

class RandomProjection
{
private:
    std::string db;
    MotifParameters params;
    std::vector<Word> words;
    typedef std::map< std::pair<int, int>, int> CollisionMap;

public:
    CollisionMap	collision;

    RandomProjection(void)
    {
    }

    ~RandomProjection(void)
    {
    }

    void addWord(const Word& iSaxWord) {
        //doStore(iSaxWord, order);
        words.push_back(iSaxWord);
    }

    void randomProjection() {

        for (int iterations = 0; iterations < params.iterations; iterations++) {
            randomProjectionOneIteration();
        }
    }

    int nextInt(int rangeEnd) {
        int rangeStart = 0;
        return rangeStart + rand() % ((rangeEnd - rangeStart) + 1);

    }

    void randomProjectionOneIteration() {
        int b = 7;
        std::set<int> cols_set;

        for (int i = 0; i < b; i++) {
            int tentativeColumn = nextInt(params.wordLength - 1);

            if ( cols_set.find(tentativeColumn) == cols_set.end() )
                cols_set.insert(tentativeColumn);
            else
                i--;
        }
        std::vector<int> cols (cols_set.begin(), cols_set.end () );

        //int vcols[] = {1, 2, 3, 4, 5, 6, 7};
        //std::vector<int> cols( vcols, vcols + sizeof(vcols) / sizeof(int) );

        printf("Mask columns: [");
        for (int i = 0; i < cols.size(); i++) {
            printf("%d, ", cols[i]);
        }
        printf("]\nWORD LIST SIZE:%d \n", words.size());

        for (int i = 0; i < words.size(); i++) {
            Word wiMask = words.at(i).mask(cols);
            for (int j = 0; j < words.size(); j++) {
                if (i == j)
                    continue;

                Word wj  = words.at(j);
                if (wiMask.maskEquals(wj, cols)) {
                    // WE have a match, increment collision matrix positions
                    std::pair<int, int> match = std::make_pair(i, j);

                    CollisionMap::iterator iter =  collision.find(match);

                    if (iter != collision.end() ) {
                        //printf("%d, %d \n", match.first, match.second);
                        int count = iter->second;
                        collision[ match ] =  count + 1;
                    } else
                        collision.insert( std::make_pair( match, 1 ) );

                } else {
                    // We DONT have a match, keep on
                }

            }
        }
    }

    void showResult()
    {
        int max_count = 0;
        std::pair<int, int> maxp;
        CollisionMap::iterator iter = collision.begin();
        for (  ; iter != collision.end() ; iter++) {

            if (iter->second > max_count) {
                max_count = iter->second; //col.get(pair);
                maxp = iter->first;
            }

        }

        printf("SIZE: %d\n", collision.size());
        printf("1-MOTIF: < %d, %d > : %d \n" ,  maxp.first,  maxp.second, max_count);
    }
};



#endif //TS_RANDOMPROJECTION_H
