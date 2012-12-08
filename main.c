#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model/superblock.h"
#include "fsm.h"
#include "shm.h"

int end;
int mount;

void sigchld_handler(int s)
{
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void read_input() {
    char* command = open_and_init_shm();
    end = 0;
    mount = 0;
    while(!end) {
        printf("\n>> ");

        char s[STDSIZE], g[STDSIZE];
        fgets(s, sizeof(s), stdin);
        s[strlen(s) - 1] = '\0'; // remove new line

        char **token;
        strcpy(g, s);
        int len = tokenizer(g, &token, ' ', 100);

        if(!strcmp(*(token), "format")) {
            if(len != 3) {
                printf("Use : format @<pathfile> <size>\n");
            } else {
                char path[STDSIZE];
                long len;

                strcpy(path, *(token+1));
                removeFront(path, 1);

                sscanf(*(token+2), "%ld", &len);

                fsm_FORMAT(path, len);
            }
        } else if(!strcmp(*(token), "mount")) {
            if(len != 3) {
                printf("Use : mount @<pathfile> <mount-point>\n");
            } else {
                mount = 1;
                strcpy(command, s);
                if(!fork()) {
                    fsm_handleInput();
                }
                while(strcmp(command, "DONE"))
                    usleep(30 * 1000);
            }

        } else if(!strcmp(*(token), "exit")) {
            end = 1;
        } else {
            if(mount) {
                strcpy(command, s);
                while(strcmp(command, "DONE"))
                    usleep(30 * 1000);
            } else {
                printf("Input not defined\n");
            }
        }

    }

}
/*
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
    printf("%s\n", getPathFromInode(sb, home));
    char s[] = "/home/rosyid";
    printf("%d\n", getInodeFromPath(sb, s) == rosyid);
    printf("------\n");
    //deleteInode(sb, root, home);
    fsm_FORMAT("test", 65536);
}
*/

int main() {
    read_input();
    printf("Thank you :)\n");
	return 0;
}
