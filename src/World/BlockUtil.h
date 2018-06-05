#ifndef _BlockUtil_h_
#define _BlockUtil_h_

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	unsigned int type;
} Block;

#define CHUNK_SIZE 16

#define AIR_BLOCK 0
#define GRASS_BLOCK 1
#define LOG_BLOCK 2
#define LEAF_BLOCK 3
#define DIRT_BLOCK 4
#define STONE_BLOCK 5

static int BLOCK_FACES[][6] = {
	{ 0, 0, 0, 0, 0, 0 },
	{ 8, 8, 8, 8, 0, 16 },
	{ 4, 4, 4, 4, 12, 12 },
	{ 20, 20, 20, 20, 20, 20 },
	{ 16, 16, 16, 16, 16, 16 },
	{ 1, 1, 1, 1, 1, 1 }
};

#endif