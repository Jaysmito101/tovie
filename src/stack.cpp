#include "stack.hpp"

#include <cstdlib>
#include <cstring>
#include <cstdio>

Stack::Stack(float incFactori)
{
    incFactor = incFactor;
    capacity = STACK_SIZE;
    size = 0;
    data = (char*)malloc(capacity);
    top = data;
}

Stack::~Stack()
{
    if(data)
        free(data);
}

void Stack::print(const char* end)
{
    printf("[ ");
    for(int i = 0; i < size; i++)
        printf("%d ", (int)data[i]);
    printf("]");
    printf("%s", end);
}

void Stack::print_int(const char* end)
{
    printf("[ ");
    int count = size / sizeof(int);
    for(int i = 0; i < count; i++)
        printf("%d ", ((int*)data)[i]);
    printf("]");
    printf("%s", end);
}

void Stack::revalidate(int sizei)
{
    if(sizei + size > capacity)
    {
        capacity = (int)(capacity * incFactor);
        char* datat = (char*)malloc(capacity);
        memcpy(datat, data, size);
        free(data);
        data = datat;
        top = data + size;
    }
}

void Stack::push(float value)
{
    validate(sizeof(float));
    memcpy(top, &value, sizeof(float));
    top += sizeof(float);
    size += sizeof(float);
}

void Stack::push(double value)
{
    validate(sizeof(double));
    memcpy(top, &value, sizeof(double));
    top += sizeof(double);
    size += sizeof(double);
}

void Stack::push(int value)
{
    validate(sizeof(int));
    memcpy(top, &value, sizeof(int));
    top += sizeof(int);
    size += sizeof(int);
}

void Stack::push(char value)
{
    validate(sizeof(char));
    memcpy(top, &value, sizeof(char));
    top += sizeof(char);
    size += sizeof(char);
}

void Stack::push(bool value)
{
    validate(sizeof(bool));
    memcpy(top, &value, sizeof(bool));
    top += sizeof(bool);
    size += sizeof(bool);
}

void Stack::push(char* value)
{
    validate(strlen(value) + 1);
    *top = '\0';
    memcpy(top + 1, value, strlen(value));
    top += strlen(value) + 1;
    size += strlen(value) + 1;
}

void Stack::push(unsigned int value)
{
    validate(sizeof(unsigned int));
    memcpy(top, &value, sizeof(unsigned int));
    top += sizeof(unsigned int);
    size += sizeof(unsigned int);
}

void Stack::push(long long value)
{
    validate(sizeof(long long));
    memcpy(top, &value, sizeof(long long));
    top += sizeof(long long);
    size += sizeof(long long);
}

void Stack::push(unsigned long long value)
{
    validate(sizeof(unsigned long long));
    memcpy(top, &value, sizeof(unsigned long long));
    top += sizeof(unsigned long long);
    size += sizeof(unsigned long long);
}

void Stack::push(char* value, int sizei)
{
    validate(sizei);
    memcpy(top, value, sizei);
    top += sizei;
    size += sizei;
}

float Stack::pop_float()
{
    float value;
    memcpy(&value, top - sizeof(float), sizeof(float));
    top -= sizeof(float);
    size -= sizeof(float);
    return value;   
}

double Stack::pop_double()
{
    double value;
    memcpy(&value, top - sizeof(double), sizeof(double));
    top -= sizeof(double);
    size -= sizeof(double);
    return value;   
}

int Stack::pop_int()
{
    int value;
    memcpy(&value, top - sizeof(int), sizeof(int));
    top -= sizeof(int);
    size -= sizeof(int);
    return value;   
}

bool Stack::pop_bool()
{
    bool value;
    memcpy(&value, top - sizeof(bool), sizeof(bool));
    top -= sizeof(bool);
    size -= sizeof(bool);
    return value;   
}

char Stack::pop_char()
{
    char value;
    memcpy(&value, top - sizeof(char), sizeof(char));
    top -= sizeof(char);
    size -= sizeof(char);
    return value;   
}

char* Stack::pop_str()
{
    char* value = top - 1;
    int sizei = 0;
    
    while(*value != '\0')
    {
        value--;
        sizei++;
    }
    char* outStr = (char*)malloc((top - value) + 1);
    memset(outStr, '\0', (top - value) + 1);
    memcpy(outStr, value + 1, top - value);
    outStr[top - value] = '\0';
    top -= sizei + 1;
    size -= sizei + 1;
    return outStr;   
}

char* Stack::pop(int sizei)
{
    char* value = top - sizei;
    char* outStr = (char*)malloc((top - value));
    memset(outStr, '\0', (top - value));
    memcpy(outStr, value, top - value);
    top -= sizei;
    size -= sizei;
    return outStr;   
}


unsigned int Stack::pop_uint()
{
    unsigned int value;
    memcpy(&value, top - sizeof(unsigned int), sizeof(unsigned int));
    top -= sizeof(unsigned int);
    size -= sizeof(unsigned int);
    return value;   
}

long long Stack::pop_llong()
{
    long long value;
    memcpy(&value, top - sizeof(long long), sizeof(long long));
    top -= sizeof(long long);
    size -= sizeof(long long);
    return value;   
}

unsigned long long Stack::pop_ullong()
{
    unsigned long long value;
    memcpy(&value, top - sizeof(unsigned long long), sizeof(unsigned long long));
    top -= sizeof(unsigned long long);
    size -= sizeof(unsigned long long);
    return value;      
}

int Stack::length()
{
    return size;
}

void* Stack::get_data()
{
    return data;
}