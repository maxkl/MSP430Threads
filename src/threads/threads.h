
#pragma once

#ifndef __ASSEMBLER__
#include <stdlib.h>
#endif

#define MAX_THREADS 4

#ifndef __ASSEMBLER__
void threads_init();
void threads_join();
int threads_create(void *fn, void *stack_ptr);
#endif
