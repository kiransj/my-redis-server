#ifndef _KEYVALUE_H_
#define _KEYVALUE_H_
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "util.h"

#include <map>
#include <iostream>
#include <string>
using namespace std;
class BitArray
{
    private:
        uint32_t size, length;
        uint8_t  *array;

    public:
        BitArray() { size = length = 0; array = NULL; }
        ~BitArray() { if(!IS_NULL(array))free(array); }
        int SetString(const uint8_t *data, const uint32_t sz);
        const char *GetString(uint32_t * const length);
        int GetBit(const uint32_t bit_number);
        int SetBit(const uint32_t bit_number, const bool value);
};

class Value
{
    public:
        BitArray b;
        time_t exp_time;
        Value() { exp_time = 0; }
};

/*This class is used for implementing SET, GET, SETBIT, GETBIT commands*/
class KeyValue
{
    private:
        int num_elements;
        map<string, Value*> dict;

        map<string, Value*>::iterator ii;
    public:
        KeyValue() { num_elements = 0; }
        ~KeyValue();
        bool SET(const string &key, const string &value, const time_t milli_seconds, const bool NX, const bool XX);
        bool GET(const string key, BitArray **b);
        bool GETBIT(string key, uint32_t bit_number);
        /* Returns Original Bit value stored at offset*/
        bool SETBIT(string key, uint32_t bit_number, bool bit_value);

        void BeginIterator(void) { ii = dict.begin(); }
        bool GetNext(string *key, uint8_t **value, uint32_t *value_size);
};

#endif
