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

void testCopyBinary() {
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
}

void testInodetoPath() {
    SuperBlock* sb = createSuperBlock("Njajal", 1024, 4, 256);
    Inode* root = createInode(sb, NULL, "root", 0, 0);
    Inode* home = createInode(sb, root, "home", 0, 0);
    Inode* rosyid = createInode(sb, home, "rosyid", 0, 0);
    //printInode(root);
    //printInode(home);
    //printInode(rosyid);
    printf("%s\n", getPathFromInode(sb, rosyid));
    char* s = "/home/rosyid";
    printf("%d\n", getInodeFromPath(sb, s) == rosyid);
}

int main() {
    testInodetoPath();
	return 0;
}
