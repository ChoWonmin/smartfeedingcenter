#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define BCM2708_PERI_BASE 	0x3F000000
#define GPIO_BASE 		(BCM2708_PERI_BASE + 0x200000) /* GPIO controller */

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

int mem_fd; /* memory file descriptor */
void *gpio_map;

// I/O access
volatile unsigned *gpio;

// GPIO setup macros. 
// Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g)	*(gpio + ( (g) / 10) ) &= ~(7 << ( ( (g) % 10) * 3) )  
#define OUT_GPIO(g)	*(gpio + ( (g) / 10) ) |= (1 << ( ( (g) % 10) * 3) ) 
