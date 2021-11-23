#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include <stdint.h>

#if __cplusplus
extern "C" {
#endif

uint8_t* parse_asm(const char* content);
void	 run_asm(const uint8_t* program);

#if __cplusplus
}
#endif

#endif