#include "inode.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Block* getBlock(Inode* this) {
	return this->block;
}

