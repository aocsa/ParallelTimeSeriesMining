//
// Created by aocsa on 6/2/18.
//

#ifndef TSDISCOVERY_WORDELEM_H
#define TSDISCOVERY_WORDELEM_H

#pragma once

#include <string>
#include "TSUtils.h"
#include "Utils.h"

class WordElem
{
public:
    int value;
    int cardinality;

public:
    WordElem(int i, int j)
    {
        value = i;
        cardinality = j;
    }
    WordElem(const WordElem &copy)
    {
        value = copy.value;
        cardinality = copy.cardinality;
    }

    WordElem operator = (const WordElem &copy)
    {
        return WordElem(*this );
    }
    bool operator < (const WordElem &wordelem) const
    {
        bool flag = false;
        if(wordelem.cardinality == cardinality)
        {
            if( value < wordelem.value )
            {
                flag = true;
            } else
            {
                flag = false;
            }
        }
        return flag;
    }

    bool operator == (const WordElem &wordelem) const
    {
        bool flag = false;
        if(wordelem.cardinality == cardinality)
        {
            if(value == wordelem.value )
            {
                flag = true;
            } else
            {
                flag = false;
            }
        }
        return flag;
    }

    ~WordElem(void) {

    }



    bool promoteToLowerCardinality(int i)
    {
        std::string s = Utils::toBinaryString(value);
        int j = s.length();
        int k = Utils::numberOfBytes(i);
        if(i > cardinality)
        {
            return false;
        } else
        {
            std::string s1 =  s.substr(0, j - 1 - (j - k));
            value = Utils::bin2dec(s1);
            cardinality = i;
            return true;
        }
    }

    static WordElem promoteToHigherCardForDist(WordElem wordelem, WordElem wordelem1, int i)
    {
        int j = wordelem.cardinality;
        std::string s = Utils::toBinaryString(wordelem.value);
        int k = Utils::numberOfBytes(wordelem.cardinality);
        int l = s.length();
        std::string s1 = Utils::toBinaryString(wordelem1.value);
        int i1 = Utils::numberOfBytes(wordelem1.cardinality);
        int j1 = s1.length();
        if(k != l)
        {
            s = Utils::padBytesToCardinality(s, j);
        }
        if(i1 != j1)
        {
            s1 = Utils::padBytesToCardinality(s1, i);
        }
        std::string s2 = getPromoted(s, j, i);
        int k1 = Utils::bin2dec(s2);

        return WordElem(k1, i);
    }

public:
    static std::string getPromoted(std::string s, int i, int j)
    {
        std::vector<double> vector;
        std::string s1 = s;
        std::string s2 = s;
        int k = s1.length();
        int l = 0;
        for(int i1 = j; i1 / 2 >= i;)
        {
            i1 /= 2;
            l++;
        }

        while(l-- > 0)
        {
            s1 = std::string(s1) + ("0");
            s2 = std::string(s2) + ("1");
        }
        int j1 = Utils::bin2dec(s1);
        int k1 = Utils::bin2dec(s);
        if(j1 > k1)
        {
            return s1;
        } else
        {
            return s2;
        }
    }

    std::vector<WordElem> promoteToHigherCardinality(int i)
    {
        std::string s = Utils::toBinaryString(value);
        int j = Utils::numberOfBytes(cardinality);
        int k = s.length();
        int l = Utils::numberOfBytes(i);
        std::string s1 ;
        std::string s2 ;
        int integer ;
        int integer1 ;
        WordElem wordelem (0,0);
        WordElem wordelem1(0,0);
        if(j > k)
        {
            s = std::string("0") + s;
        }
        s1 = std::string(s)  + ("0");
        s2 = std::string(s)  + ("1");

        integer = Utils::bin2dec(s1);
        integer1 = Utils::bin2dec(s2);
        wordelem = WordElem(integer, i);
        wordelem1 = WordElem(integer1, i);

        std::vector<WordElem> result;
        result.push_back(wordelem);;
        result.push_back(wordelem1);;
        return result;
    }

    bool equals(const WordElem &obj)
    {
        bool flag = false;
        WordElem wordelem = (WordElem)obj;
        if(wordelem.cardinality == cardinality)
        {
            if(wordelem.value == value)
            {
                flag = true;
            } else
            {
                flag = false;
            }
        }
        return flag;
    }

    int hashCode()
    {
        int i = 1;
        int j = cardinality;
        int integer = value;
        i = 31 * i + j;
        //i = 31 * i + (integer != null ? integer.hashCode() : 0);
        return i;
    }



    std::string toBinaryString()
    {
        std::string s = "";
        s = s  + "{";
        s = s  + Utils::toBinaryString(value);
        s = s  + "^";
        s = s  +  Utils::convertToStr(cardinality);
        s = s  + " , ";
        s = s  + "}";
        return s;
    }


};



#endif //TSDISCOVERY_WORDELEM_H
