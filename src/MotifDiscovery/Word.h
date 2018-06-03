//
// Created by aocsa on 6/2/18.
//

#ifndef TSDISCOVERY_WORD_H
#define TSDISCOVERY_WORD_H

#pragma once
#include <vector>
#include <map>
#include <algorithm>
#include "WordElem.h"
#include "SAX.h"

class Word
{

public:

    std::vector<WordElem> word;
    int size;
    std::vector<int> cardinality;


public:

    Word(int i, const std::vector<int> &vector)
    {
        size = i;
        cardinality = vector;
    }
    ~Word(void) {


    }

    Word(int i, int j)
    {
        size = i;
        cardinality.resize(i);
        for(int k = 0; k < i; k++)
        {
            cardinality[ k ]  = j;
        }

    }

    Word(const std::vector<double> &ad, int wordLength, int card) {
        size = wordLength;
        for(int k = 0; k < wordLength; k++)
        {
            cardinality.push_back(card);
        }

        if(!createiSAXWord(ad))
        {
            word.clear();
        }
    }

    Word(const std::vector<double> &ad, int i, std::vector<int> vector)
    {

        size = i;
        cardinality =  vector;
        std::vector<int> vector1;

        std::vector<int>::iterator iterator = cardinality.begin();

        for (int i = 0; i < cardinality.size() ; i++)
        {
            int integer = cardinality[i];
            std::vector<int>::iterator ret = std::find(vector1.begin(), vector1.end (),  integer) ;
            if(ret == vector1.end()) {
                vector1.push_back(integer);
            }
        }

        int j = vector1.size();
        if(j == 1)
        {
            if(!createiSAXWord(ad))
            {
                word.clear();
            }
        } else
        if(!createiSAXWordMultCardinalities(ad, vector1))
        {
            word.clear();
        }
    }

    Word(std::vector<WordElem> arraylist, std::vector<int> vector, int i)
    {
        word = arraylist;
        cardinality = vector;
        size = i;
    }

    bool isValid() const {
        if (size == 0 && cardinality.size() == 0)
            return false;
        return true;
    }

    bool operator == (const Word & obj) const {
        for (int i = 0; i < word.size() ; i++)
        {
            if ( !(this->word[i] == obj.word[i] ) )
                return false;
        }
        return true;
    }

    bool operator < (const Word & obj) const {
        /*bool flag = true;
        for (int i = 0; i < word.size() ; i++)
        {
            if ( !( this->word[i] < obj.word[i] ) )
                flag = false;
        }
        return flag;*/

        return hashCode() < obj.hashCode();
    }



    bool createiSAXWordMultCardinalities(const std::vector<double> &ad, std::vector<int> vector)
    {
        std::map<int, std::vector<int> > hashmap ;

        for(int i = 0; i < vector.size(); i++)
        {
            std::vector<int> ai = SAX::timeseries2symbol(ad, ad.size(), size, vector.at(i), false);
            hashmap [ vector.at(i) ] = ai ;
        }

        for(int j = 0; j < cardinality.size(); j++)
        {
            int k = (cardinality.at(j));
            std::vector<int> val =  hashmap [k] ;
            int l = val[j];
            word.push_back(WordElem(l, k));
        }
        return true;
    }

    bool createiSAXWord(const std::vector<double> &ad)
    {
        std::vector<int> ai = SAX::timeseries2symbol(ad, ad.size(), size, (cardinality.at(0)), false);
        for(int i = 0; i < ai.size(); i++)
        {
            int j = ai[i];
            int k = (cardinality.at(0));
            word.push_back(WordElem(j, k));
        }

        return true;
    }



    bool promoteToLowerCardinality(int i)
    {
        for(std::vector<WordElem>::iterator iterator = word.begin(); iterator != word.end(); iterator++)
        {
            WordElem wordelem = *iterator;
            bool flag = wordelem.promoteToLowerCardinality(i);
            if(!flag)
            {
                return false;
            }
        }

        return true;
    }

    Word promoteToLowerCardinalityNow(int i)
    {
        int j = 1;
        for(int k = (cardinality.at(0)); k / 2 > i; k /= 2)
        {
            j++;
        }

        Word word1(size, i);
        WordElem wordelem1 (0,0);

        for(std::vector<WordElem>::iterator iterator = word.begin(); iterator != word.end(); iterator++ )
        {
            WordElem wordelem = *iterator;

            std::string s = Utils::toBinaryString(wordelem.value);
            int l = s.length();
            int i1 = Utils::numberOfBytes(wordelem.cardinality);
            std::string s1 = "";
            for(int j1 = l; j1++ < i1;)
            {
                s = (std::string("0") )  + s ;
            }

            if(i >= wordelem.cardinality)
            {
                return Word(0,0);// @todo: check
            }

            int k1 = i1 - j;
            if(k1 > 0)
            {
                s1 =  s.substr(0, k1);
            } else
            {
                s1 = s;
            }
            wordelem1.value = Utils::bin2dec(s1);

            wordelem1.cardinality = i;
            word1.word.push_back(wordelem1);
        }
        return word1;
    }

    std::vector<WordElem> promoteToHigherCardinality(int i)
    {
        int j = -1;
        j = (cardinality.at(i)) * 2;
        WordElem  wrd = word[i];
        return wrd.promoteToHigherCardinality(j);
    }

    std::vector<int> getListOfCardinalities()
    {
        std::vector<int> vector ;
        WordElem wordelem(0,0);

        //
        for(std::vector<WordElem>::iterator iterator = word.begin(); iterator != word.end(); iterator++)
        {
            wordelem = *iterator;
            vector.push_back(wordelem.cardinality);
        }

        return vector;
    }

    /*static Word parseWord(std::string s) 	{
        Word word1  (0,0);
        try
        {
            int i = -1;
            std::vector<WordElem> arraylist ;
            std::vector<int> vector ;
            std::string as[] = s.split("_");
            i = as.length;
            std::string as1[] = as;
            int j = as1.length;
            for(int k = 0; k < j; k++)
            {
                std::string s1 = as1[k];
                std::string as2[] = s1.split("\\.");
                std::string s2 = as2[0];
                std::string s3 = as2[1];
                int l = Integer.parseInt(s2);
                int i1 = Integer.parseInt(s3);
                WordElem wordelem = new WordElem(l, i1);
                arraylist.add(wordelem);
                vector.add(Integer.valueOf(i1));
            }

            word1 = new Word(arraylist, vector, i);
        }
        catch(Exception exception)
        {
            throw new WordFormatException();
        }
        return word1;
    }*/

    bool equals(const Word &word1 )
    {
        if(word1.word.size() != word.size())
        {
            return false;
        }
        return word1.word == word;
    }

    bool maskEquals(const Word &word1, const std::vector<int> &arraylist)
    {
        bool  flag = true;
        int i = 0;
        do
        {
            if(i >= word.size())
            {
                break;
            }
            flag &= (word.at(i)) == (word1.word.at((arraylist.at(i))));
            if(!flag)
            {
                break;
            }
            i++;
        } while(true);
        return flag;
    }

    //@todo:  hash code, check, all hash codes
    int hashCode() const
    {
        /*std::string s = this->toBinCode();
        size_t h = 0;
        std::string::const_iterator p, p_end;
        for(p = s.begin(), p_end = s.end(); p != p_end; ++p)
        {
            h = 31 * h + (*p);
        }
        return h;*/

        int i = 1;
        i = 31 * i + (!cardinality.empty() ? Utils::hashCodeOfVector(cardinality) : 0);
        i = 31 * i + size;
        i = 31 * i + ( !word.empty() ? Utils::hashCodeOfString ( toString() ) : 0);
        return i;
    }

    bool binaryEquals(const Word &word1)
    {
        bool flag = true;
        bool flag1 = true;
        int i = 0;
        do
        {
            if(i >= word1.word.size())
            {
                break;
            }
            WordElem wordelem = (WordElem)word1.word.at(i);
            WordElem wordelem1 = (WordElem)word.at(i);
            std::string s = Utils::toBinaryString(wordelem.value);
            std::string s1 = Utils::toBinaryString(wordelem1.value);
            std::string s2 = Utils::padBytesToCardinality(s, wordelem.cardinality);
            std::string s3 = Utils::padBytesToCardinality(s1, wordelem1.cardinality);
            if(s2.length() > s3.length())
            {
                std::string sub =  s2.substr(0, s3.size()) ;
                flag1 &= ( sub == s3);  // @todo : check
            } else
            {
                std::string sub = s3.substr(0, s2.size());
                flag1 &= (sub == s2);
            }
            flag = flag && flag1;
            if(!flag)
            {
                break;
            }
            i++;
        } while(true);
        return flag;
    }


    std::string toString() const
    {
        std::string  stringbuffer = "";
        stringbuffer.append("{");

        for(std::vector<WordElem>::const_iterator iterator = word.begin(); iterator != word.end(); iterator++ )
        {
            WordElem wordelem = *iterator;
            stringbuffer.append( Utils::convertToStr( wordelem.value ) );
            stringbuffer.append("^");
            stringbuffer.append( Utils::convertToStr(wordelem.cardinality) );
            stringbuffer.append(" , ");
        }

        stringbuffer.append("}");
        return stringbuffer;
    }

    std::string toStringVector() const
    {
        std::string  stringbuffer = "";
        stringbuffer.append("");

        for(std::vector<WordElem>::const_iterator iterator = word.begin(); iterator != word.end(); iterator++ )
        {
            WordElem wordelem = *iterator;
            stringbuffer.append( Utils::convertToStr( wordelem.value ) );
            stringbuffer.append(" ");
        }

        //stringbuffer.append("\n");
        return stringbuffer;
    }


    std::string toBinCode() const
    {
        std::string s = "";
        s = s.append( Utils::convertToStr(cardinality[0]));
        for(std::vector<WordElem>::const_iterator iterator = word.begin(); iterator != word.end(); iterator++ )
        {
            WordElem wordelem = (WordElem)*iterator;
            s = s.append(Utils::convertToStr(wordelem.value));
        }
        return s;
    }

    std::string toBinaryString() const
    {
        std::string s = "";
        s = s.append("{");

        for(std::vector<WordElem>::const_iterator iterator = word.begin(); iterator != word.end(); iterator++ )
        {
            WordElem wordelem = (WordElem)*iterator;
            s = s.append(Utils::toBinaryString(wordelem.value));
            s = s.append("^");
            s = s.append( Utils::convertToStr(wordelem.cardinality));
            s = s.append(" , ");
        }

        s = (s).append("}");
        return s;
    }

    std::vector<int> getCardinality()
    {
        return cardinality;
    }

    void setCardinality(std::vector<int> vector)
    {
        cardinality = vector;
    }

    Word mask(const std::vector<int>& arraylist)
    {
        Word word1(arraylist.size(), cardinality);
        for(int i = 0; i < arraylist.size(); i++)
        {
            WordElem e = word.at( arraylist[i] );
            word1.word.push_back( e  );
        }

        return word1;
    }

};



#endif //TSDISCOVERY_WORD_H
