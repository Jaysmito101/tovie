#include <stdio.h>
extern "C" void __declspec(dllexport) __stdcall print_arr(int* programStack, int stackSize) {
    int length = *(programStack + stackSize - 1);
    int* start = programStack + stackSize - length - 1;
    for(int i=0;i<length;i++)
    {
        printf("%d ", *(start + i));
    }
    printf("\n");
}

// Bubble sort
extern "C" void __declspec(dllexport) __stdcall sort_arr(int* programStack, int stackSize) {
    int length = *(programStack + stackSize - 1);
    int* start = programStack + stackSize - length - 1;
    for(int i=0;i<length;i++)
    {
        for(int j=0;j<length - 1;j++)
        {
            if(*(start + j) > *(start + j + 1))
            {
                int tmp = *(start + j);
                *(start + j) = *(start + j + 1);
                *(start + j + 1) = tmp;
            }
        }
    }
}