#pragma once

namespace tovie
{

#define STACK_SIZE 4096 * 4096

#ifdef TOVIE_SAFE
#define validate(x) revalidate(x) 
#else
#define validate(x)
#endif
  
class Stack
{
    public:
    Stack(float incFactor = 1.5f);
    ~Stack();

    template<typename T>
    inline void push(T value)
    {
        validate(sizeof(T));
        *(reinterpret_cast<T*>(top)) = value;
        top += sizeof(T);
        size += sizeof(T);
    }

    void push(char* value);
    void push(char* value, int size);

    template<typename T>
    inline T pop()
    {
        validate(sizeof(T));
        top -= sizeof(T);
        size -= sizeof(T);
        return *(reinterpret_cast<T*>(top));
    }

    char* pop_str();
    char* pop(int size = 1);

    void print(const char* end = "\n");
    void print_int(const char* end = "\n");
    
    int length();
    void* get_data();
    void revalidate(int size);

    private:

    float incFactor;
    char* top;
    char* data;
    int size;
    int capacity;
};

}