#include "cstr.h"
#include "stdint.h"

#define DECIMAL 10

char uIntToStringOutput[128];

const char *ToString(uint64_t value)
{
    uint8_t size = 0;
    uint64_t sizeTest = value;
    while(sizeTest / DECIMAL > 0)
    {
        sizeTest /= 10;
        size++;   
    }
    uint8_t index = 0;
    while(value / DECIMAL > 0)
    {
        uint8_t remainder = value % DECIMAL;
        value /= DECIMAL;
        uIntToStringOutput[size - index] = remainder + '0';
        index++;
    }
    uint8_t remainder = value % DECIMAL;
    uIntToStringOutput[size - index] = remainder + '0';
    uIntToStringOutput[size + 1] = 0;
    return uIntToStringOutput;
}


char IntToStringOutput[128];

const char *ToString(int64_t value)
{
    uint8_t IsNegative = 0;
    if(value < 0)
    {
        IsNegative = 1; //as an offset
        value *= -1; //to set the value > 0
        IntToStringOutput[0] = '-';
    }
    uint8_t size = 0;
    uint64_t sizeTest = value;
    while(sizeTest / DECIMAL > 0)
    {
        sizeTest /= 10;
        size++;   
    }
    uint8_t index = 0;
    while(value / DECIMAL > 0)
    {
        uint8_t remainder = value % DECIMAL;
        value /= DECIMAL;
        IntToStringOutput[IsNegative + size - index] = remainder + '0';
        index++;
    }
    uint8_t remainder = value % DECIMAL;
    IntToStringOutput[IsNegative + size - index] = remainder + '0';
    IntToStringOutput[IsNegative + size + 1] = 0;
    return IntToStringOutput;
}

char HexToStringOutput64[128];

const char *ToHexString(uint64_t value)
{
    uint64_t *ValuePtr = &value;
    uint8_t *ptr;
    uint8_t tmp;
    uint8_t size = 8 * 2 - 1;
    for(uint8_t i = 0; i < size; i++)
    {
        ptr = ((uint8_t*)ValuePtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        HexToStringOutput64[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        HexToStringOutput64[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }
    HexToStringOutput64[size + 1] = 0;
    return HexToStringOutput64;
}

char HexToStringOutput32[128];

const char *ToHexString(uint32_t value)
{
    uint32_t *ValuePtr = &value;
    uint8_t *ptr;
    uint8_t tmp;
    uint8_t size = 4 * 2 - 1;
    for(uint8_t i = 0; i < size; i++)
    {
        ptr = ((uint8_t*)ValuePtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        HexToStringOutput32[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        HexToStringOutput32[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }
    HexToStringOutput32[size + 1] = 0;
    return HexToStringOutput32;
}

char HexToStringOutput16[128];

const char *ToHexString(uint16_t value)
{
    uint16_t *ValuePtr = &value;
    uint8_t *ptr;
    uint8_t tmp;
    uint8_t size = 2 * 2 - 1;
    for(uint8_t i = 0; i < size; i++)
    {
        ptr = ((uint8_t*)ValuePtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        HexToStringOutput16[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        HexToStringOutput16[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }
    HexToStringOutput16[size + 1] = 0;
    return HexToStringOutput16;
}

char HexToStringOutput8[128];

const char *ToHexString(uint8_t value)
{
    uint8_t *ValuePtr = &value;
    uint8_t *ptr;
    uint8_t tmp;
    uint8_t size = 1 * 2 - 1;
    for(uint8_t i = 0; i < size; i++)
    {
        ptr = ((uint8_t*)ValuePtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        HexToStringOutput8[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        HexToStringOutput8[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }
    HexToStringOutput8[size + 1] = 0;
    return HexToStringOutput8;
}

char DoubleToStringOutput[128];

const char *ToString(double value, uint8_t DecimalPlaces)
{
    if(DecimalPlaces > 20) DecimalPlaces = 20;
    char *IntPartPtr = (char*)ToString((int64_t)value);
    char *DoublePartPtr = DoubleToStringOutput;
    if(value < 0)
    {
        value *= -1;
    }
    while(*IntPartPtr != 0)
    {
        *DoublePartPtr = *IntPartPtr;
        IntPartPtr++;
        DoublePartPtr++;
    }
    *DoublePartPtr = '.';
    DoublePartPtr++;
    double newValue = value - (int)value;
    for(uint8_t i = 0; i < DecimalPlaces; i++)
    {
        newValue *= DECIMAL;
        *DoublePartPtr = (int)newValue + '0';
        newValue -= (int)newValue;
        DoublePartPtr++;
    }
    *DoublePartPtr = 0;
    return DoubleToStringOutput;
}

const char *ToString(double value)
{
    return ToString(value, 2);
}