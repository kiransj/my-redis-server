#include "keyvalue.h"

#include <sys/time.h>

long long GetTime(void)
{
    long long milli_second;
    struct timeval tm;
    gettimeofday(&tm, NULL);
    milli_second = tm.tv_sec * 1000L + tm.tv_usec/1000;
    return milli_second;
}
int BitArray::SetString(const uint8_t *data, const uint32_t sz)
{
    if(size < sz)
    {
        array = (uint8_t*)realloc(array, sz);
        if(IS_NULL(array))
        {
            log_msg("realloc(%u) failed", sz);
            return 0;
        }
        size = sz;
    }

    memcpy(array, data, sz);
    memset(&array[sz], 0, size - sz);
    return 1;
}

const uint8_t * BitArray::GetString(uint32_t * const length)
{
    if(!IS_NULL(length))
        *length = size;
    return array;
}

int BitArray::SetBit(const uint32_t bit_number, const bool value)
{
    uint32_t byte = bit_number >> 8, bit = 1 << (bit_number % 8);
    if(byte >= size)
    {
        array = (uint8_t *)realloc(array, byte+1);
        memset(&array[size], 0, byte + 1 - size);
        size = byte+1;
    }
    if(value == true)
        array[byte] |= bit;
    else
        array[byte] = (array[byte] & (~bit));

    return 0;
}

int BitArray::GetBit(const uint32_t bit_number)
{
    uint32_t byte = bit_number >> 8, bit = 1 << (bit_number % 8);
    if(byte >= size)
    {
        return 0;
    }

    return array[byte] & bit ? 1 : 0;
}
bool KeyValue::SET(string key, string value, time_t milli_seconds, bool NX, bool XX)
{
    Value *v = dict[key];

    if((XX && IS_NULL(v)) || (NX && (!IS_NULL(v))))
    {
        return false;
    }

    if(IS_NULL(v))
    {
        v = new Value;
        if(IS_NULL(v))
        {
            return false;
        }
    }
    v->b.SetString((const uint8_t *)value.c_str(), value.size());
    if(milli_seconds)
        v->exp_time = GetTime() + milli_seconds;
    dict[key] = v;
    return false;
}
bool KeyValue::GET(string key, BitArray **b)
{
    Value *v = dict[key];
    if(!IS_NULL(v))
    {
        *b = &(v->b);
        if((0 != v->exp_time) && (v->exp_time < GetTime()))
        {
            dict.erase(key);
            *b = NULL;
            return false;
        }
        return true;
    }
    *b = NULL;
    return false;
}
bool KeyValue::GETBIT(string key, uint32_t bit_number)
{
    Value *v = dict[key];
    if(!IS_NULL(v))
    {
        return v->b.GetBit(bit_number);
    }
    return false;
}
/* Returns Original Bit value stored at offset*/
bool KeyValue::SETBIT(string key, uint32_t bit_number, bool bit_value)
{   
    Value *v = dict[key];
    bool flag = false;
    if(!IS_NULL(v))
    {
        flag = v->b.GetBit(bit_number);
        v->b.SetBit(bit_number, bit_value);
    }
    return flag;
}

KeyValue::~KeyValue()
{
    map<string, Value*>::iterator ii = dict.begin();
    while(ii != dict.end())
    {
        delete ii->second;
        ii++;
    }
}

