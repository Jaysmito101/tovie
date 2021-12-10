#include "stack.hpp"

#include <cstdlib>
#include <cstring>
#include <cstdio>

namespace tovie
{

Stack::Stack(float incFactori)
{
    incFactor = incFactori;
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

void Stack::push(char* value)
{
    validate(strlen(value) + 1);
    *top = '\0';
    memcpy(top + 1, value, strlen(value));
    top += strlen(value) + 1;
    size += strlen(value) + 1;
}

void Stack::push(char* value, int sizei)
{
    validate(sizei);
    memcpy(top, value, sizei);
    top += sizei;
    size += sizei;
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

int Stack::length()
{
    return size;
}

void* Stack::get_data()
{
    return data;
}
}