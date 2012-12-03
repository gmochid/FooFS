#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/superblock.h"

void read_input() {
	printf(">> ");

	char input[256];
	fgets(input, sizeof(input), stdin);
	input[strlen(input) - 1] = '\0'; // remove new line
}

int main() {
	SuperBlock* sb = (SuperBlock*) malloc(sizeof(SuperBlock));
	sb->sizeBlock = 10;
	Inode* in = (Inode*) malloc(sizeof(Inode));
	in->fileSize = 20;
	Block* b = createBlock(10, NULL);
	Block* a = createBlock(10, b);
	in->block = a;

	setBlocksData(sb, in, "abcdefghijabcdefgh");
	printf("%s - %s\n", a->data, b->data);
	printf("%s\n", getBlocksData(sb, in));

	return 0;
}
