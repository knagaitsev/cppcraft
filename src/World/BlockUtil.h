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
#define COBBLESTONE_BLOCK 6
#define PLANK_BLOCK 7
#define SAND_BLOCK 8

static int BLOCK_FACES[][7] = {
	//first 6 are face textures, next int is a boolean of whether or not this block has some transparency
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 8, 8, 8, 8, 0, 16, 0 },
	{ 4, 4, 4, 4, 12, 12, 0 },
	{ 20, 20, 20, 20, 20, 20, 1 },
	{ 16, 16, 16, 16, 16, 16, 0 },
	{ 1, 1, 1, 1, 1, 1, 0 },
	{ 2, 2, 2, 2, 2, 2, 0 },
	{ 3, 3, 3, 3, 3, 3, 0 },
	{ 5, 5, 5, 5, 5, 5, 0 }
};

#endif