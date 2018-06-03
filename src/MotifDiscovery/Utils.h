//
// Created by aocsa on 6/2/18.
//

#ifndef TSDISCOVERY_UTILS_H
#define TSDISCOVERY_UTILS_H

#pragma once
#include <string>
#include <sstream>

class Utils
{
public:

    Utils(void)
    {
    }

    ~Utils(void)
    {
    }

    static int numberOfBytes(int i)
    {
        int byte0 = -1;
        int ai[] = {
                0, 1, 2
        };
        int ai1[] = {
                3, 4
        };
        int ai2[] = {
                5, 6, 7, 8
        };
        int ai3[] = {
                9, 10, 11, 12, 13, 14, 15, 16
        };
        int ai4[] = {
                17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
                27, 28, 29, 30, 31, 32
        };
        int ai5[] = {
                33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
                43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
                53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
                63, 64
        };
        if(isInArray(ai, 3, i))
        {
            byte0 = 1;
        } else
        if(isInArray(ai1, 2, i))
        {
            byte0 = 2;
        } else
        if(isInArray(ai2, 4, i))
        {
            byte0 = 3;
        } else
        if(isInArray(ai3, 8, i))
        {
            byte0 = 4;
        } else
        if(isInArray(ai4, 16,  i))
        {
            byte0 = 5;
        } else
        if(isInArray(ai5, 32,  i))
        {
            byte0 = 6;
        } else
        {
            byte0 = 7;
        }
        return byte0;
    }

    static int numberOfBytes2(int i)
    {
        int byte0 = -1;
        int ai[] = {
                0, 1
        };
        int ai1[] = {
                2, 3
        };
        int ai2[] = {
                4, 5, 6, 7
        };
        int ai3[] = {
                8, 9, 10, 11, 12, 13, 14, 15
        };
        int ai4[] = {
                16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
                26, 27, 28, 29, 30, 31
        };
        int ai5[] = {
                32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
                42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
                52, 53, 54, 55, 56, 57, 58, 59, 60, 61,
                62, 63
        };
        if(isInArray(ai, 3, i))
        {
            byte0 = 1;
        } else
        if(isInArray(ai1, 2, i))
        {
            byte0 = 2;
        } else
        if(isInArray(ai2, 4, i))
        {
            byte0 = 3;
        } else
        if(isInArray(ai3, 8, i))
        {
            byte0 = 4;
        } else
        if(isInArray(ai4, 16,  i))
        {
            byte0 = 5;
        } else
        if(isInArray(ai5, 32,  i))
        {
            byte0 = 6;
        } else
        {
            byte0 = 7;
        }
        return byte0;
    }

    static std::string padBytesToCardinality(std::string s, int i)
    {
        std::string s1 = "";
        if(s.length() == numberOfBytes(i))
        {
            s1 = s;
        } else
        if(s.length() < numberOfBytes(i))
        {
            int j = numberOfBytes(i);
            int k = s.length();
            s1 = s;
            for(; k < j; k++)
            {
                s1 = std::string("0") + s1;
            }

        } else
        {
            printf("padBytesToCardinality : should not occur");
        }
        return s1;
    }

    template<class T>
    static std::string convertToStr(T number)
    {
        std::stringstream ss;//create a stringstream
        ss << number;//add number to the stream
        return ss.str();//return a string with the contents of the stream
    }

    // valid for integer values
    template <class T>
    static int hashCodeOfVector(const std::vector<T>& v) {
        int hashCode = 1;
        for (int i = 0; i < v.size(); i++) {
            hashCode = 31*hashCode + (int)v[i];
        }
        return hashCode;
    }
    static int hashCodeOfString(const std::string &val) {
        int h = 0;
        int offset = 0;
        if (h == 0) {
            int off = offset;
            int len = val.length();

            for (int i = 0; i < len; i++) {
                h = 31*h + val[off++];
            }
        }
        return h;
    }

    static int bin2dec(const std::string &bin)
    {
        int  b, k, m, n;
        int  len, sum = 0;

        len = bin.size()  - 1;
        for(k = 0; k <= len; k++)
        {
            n = (bin[k] - '0'); // char to numeric value
            if ((n > 1) || (n < 0))
            {
                puts("\n\n ERROR! BINARY has only 1 and 0!\n");
                return (0);
            }
            for(b = 1, m = len; m > k; m--)
            {
                // 1 2 4 8 16 32 64 ... place-values, reversed here
                b *= 2;
            }
            // sum it up
            sum = sum + n * b;
            //printf("%d*%d + ",n,b);  // uncomment to show the way this works
        }
        return(sum);
    }

    static std::string toBinaryString(long n)
    {
        int i=0, j=0;
        int bin=0;

        for(i=0;n>0;i++)
        {
            j=n%2;
            bin=bin+j * pow1(10,i);
            n/=2;
        }

        return convertToStr(bin);
    }
    static int pow1(int a , int b)
    {
        int i=0;
        int sum=1;
        for(i=0;i<b;i++)
            sum*=a;

        return sum;
    }


private:
    static bool isInArray(int ai[], int size_ai, int i)
    {
        bool flag = false;
        for(int j = 0; j < size_ai ; j++)
        {
            if(ai[j] == i)
            {
                flag = true;
            }
        }

        return flag;
    }



    /*static boolean deleteDirectory(File file)
    {
        boolean flag = true;
        if(file.exists())
        {
            File afile[] = file.listFiles();
            for(int i = 0; i < afile.length; i++)
            {
                if(afile[i].isDirectory())
                {
                    flag = deleteDirectory(afile[i]);
                } else
                {
                    flag = afile[i].delete();
                }
            }

        }
        flag = file.delete();
        return flag;
    }

    */
};


#endif //TSDISCOVERY_UTILS_H
