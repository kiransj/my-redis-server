#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "util.h"

#include <map>
#include <string>
using namespace std;
class BitArray
{
    private:
        uint32_t size;
        uint8_t  *array;

    public:
        BitArray() { size = 0; array = NULL; }
        ~BitArray() { if(!IS_NULL(array))free(array); }
        int SetString(const uint8_t *data, const uint32_t sz);
        const uint8_t *GetString(uint32_t * const length);
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

class KeyValue
{
    private:

        int num_elements;
        map<string, Value*> dict;
    public:
        KeyValue() { num_elements = 0; }
        ~KeyValue();
        bool SET(string key, string value, time_t milli_seconds, bool NX, bool XX);
        bool GET(string key, BitArray **b);
        bool GETBIT(string key, uint32_t bit_number);
        /* Returns Original Bit value stored at offset*/
        bool SETBIT(string key, uint32_t bit_number, bool bit_value);
};


