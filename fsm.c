/*
 * functional.c
 *
 *  Created on: Dec 8, 2012
 *      Author: rosyid
 */

#include "fsm.h"

void fsm_MKDIR(char* newDirectory) {
    Inode* Parent=getInodeFromPath(currentSuperblock,currentDirectory); //mengambil inode dari direktori yang sedang digunakan, untuk dijadikan parent dari direktori baru
    createInode(currentSuperblock, Parent, newDirectory, 0, 0); //karena yang akan dibuat adalah direktori, maka parameter 'type' diisi 0
    //dan karena itu pula parameter 'filesize' diisi 0
}

void fsm_PWD() {
    printf("%s\n", currentDirectory);
}

void fsm_STATUS() {
    printf("Size Block : %ld\n", currentSuperblock->sizeBlock);
    printf("Total Capacity : %ld\n", currentSuperblock->capacity);
    printf("Used Capacity : %ld\n", currentSuperblock->usedCapacity);
    printf("Available Capacity : %ld\n", currentSuperblock->availableCapacity);
    printf("Total Block : %ld\n", currentSuperblock->totalBlock);
    printf("Used Block : %ld\n", currentSuperblock->usedBlock);
    printf("Available Block : %ld\n", currentSuperblock->availableBlock);
    printf("File count : %ld\n", currentSuperblock->fileCount);
    printf("Directory count : %ld\n", currentSuperblock->directoryCount);
}

void fsm_RM(char* path) {
    Inode* cwd = getInodeFromPath(currentSuperblock, currentDirectory);
    if(cwd == NULL) {
        printf("File or directory not exist\n");
        return;
    }
    deleteInode(currentSuperblock, currentSuperblock->root, cwd);
}

void fsm_CD(char* path) {
    char ** str_tok;
    char temp[1 << 12];
    strcpy(temp, currentDirectory);
    strcat(temp, path);
    char res[1 << 10];
    parse_path(&str_tok, temp, res);
    strcpy(currentDirectory, res);
}

void fsm_LS(char* path) {
    char cwd[1 << 10];
    strcpy(cwd, currentDirectory);
    if(path != NULL) {
        strcat(cwd, path);
    }

    Inode* inode = getInodeFromPath(currentSuperblock, cwd);
    Inode* child = inode->child;
    while(child != NULL) {
        if(child->type) {
            printf("%s\n", child->name);
        } else {
            printf("%s (dir)\n", child->name);
        }
        child = child->sibling;
    }
}

void fsm_FORMAT(char* filepath, long size) {
    long sizeB = (1 << 12);
    long totalB = size / sizeB;
    SuperBlock* sb = createSuperBlock("GMO", size, sizeB, totalB);

    FILE* file = fopen(filepath, "w+");
    writeToFile(file, sb);
    fclose(file);
}

void fsm_MOUNT(char* filepath){
    FILE* file = fopen(filepath, "r+");
    char name[STDSIZE];
    long sizeB, capacity, totalB;
    int tp;
    Inode* inode;
    Block* block;

    fscanf(file, "%s%ld%ld%ld", name, &sizeB, &capacity, &totalB);
    currentSuperblock = createSuperBlock(name, capacity, sizeB, totalB);

    fscanf(file, "%d", &tp);
    while(tp < 3) {
        if(tp == 1) {
            char par[STDSIZE];
            long fileSize;
            int type;

            fscanf(file, "%s%s%ld%d", par, name, &fileSize, &type);

            if(!strcmp(par, "NULL")) {
                inode = createInode(currentSuperblock, NULL, name, fileSize, type);
                currentSuperblock->root = inode;
            } else {
                inode = createInode(currentSuperblock, getInodeFromPath(currentSuperblock, par), name, fileSize, type);
            }
        } else { // tp == 2
            char data[currentSuperblock->sizeBlock];
            fscanf(file, "%s", data);
            Block* after = createBlockData(currentSuperblock->sizeBlock, data, NULL);
            if(inode->block == NULL) {
                inode->block = after;
                block = after;
            } else {
                block->nextBlock = after;
            }
        }
        fscanf(file, "%d", &tp);
    }
}

void fsm_UNMOUNT() {
    FILE * file = fopen(fileFormatPath, "w+");
    writeToFile(file, currentSuperblock);
    fclose(file);
    strcpy(command, "DONE");
    printf("%s\n", command);
    exit(0);
}

void fsm_CAT(char* filepath) {
    /* TODO */
}

void writeToFile(FILE* file, SuperBlock* sb) {
    fprintf(file, "%s\n", sb->name); // menunjukkan nama
    fprintf(file, "%ld\n", sb->sizeBlock); // size block
    fprintf(file, "%ld\n", sb->capacity);
    fprintf(file, "%ld\n", sb->totalBlock);

    Inode* queue[1 << 10];
    int h = 0;
    int t = 0;
    if(sb->root != NULL) {
        queue[t++] = sb->root;
    }
    while(h < t) {
        Inode* now = queue[h++];
        fprintf(file, "1\n");
        if(now->parent == NULL) {
            fprintf(file, "NULL\n");
        } else {
            char* x = getPathFromInode(sb, now->parent);
            if(x == NULL) {
                fprintf(file, "/\n");
            } else {
                fprintf(file, "%s\n", getPathFromInode(sb, now->parent));
            }
        }
        fprintf(file, "%s\n%ld\n%d\n", now->name, now->fileSize, now->type);
        Block* b = now->block;


        while(b != NULL) {
            fprintf(file, "2\n%s\n", b->data);
            b = b->nextBlock;
        }
        Inode* child = now->child;
        while(child != NULL) {
            queue[t++] = child;
            child = child->sibling;
        }
    }
    fprintf(file, "3\n");
}

void fsm_handleInput() {
    command = open_shm();

    while(1) {
        char **token;
        char g[STDSIZE];
        strcpy(g, command);
        int len = tokenizer(g, &token, ' ', 100);

        if(!strcmp(*(token), "mount")) {

        } else if(!strcmp(*(token), "unmount")) {
            if(len == 2) {
                fsm_UNMOUNT();
            } else  {
                printf("Use UNMOUNT <mount-point>\n");
            }
        } else if(!strcmp(*(token), "ls")) {
            if(len == 1) {
                fsm_LS(NULL);
            } else {
                fsm_LS(*(token + 1));
            }
        } else if(!strcmp(*(token), "pwd")) {
            fsm_PWD();
        } else if(!strcmp(*(token), "cd")) {
            if(len == 2) {
                fsm_CD(*(token + 1));
            } else {
                printf("Use LIST <path>\n");
            }
        } else if(!strcmp(*(token), "mkdir")) {
            if(len == 2) {
                fsm_MKDIR(*(token + 1));
            } else  {
                printf("Use MKDIR <path>\n");
            }
        } else if(!strcmp(*(token), "rm")) {
            if(len == 2) {
                fsm_RM(*(token + 1));
            } else {
                printf("Use RM <path>\n");
            }
        } else if(!strcmp(*(token), "cp")) {

        } else if(!strcmp(*(token), "mv")) {

        } else if(!strcmp(*(token), "cat")) {

        } else if(!strcmp(*(token), "status")) {
            fsm_STATUS();
        } else {
            printf("Command undefined");
        }

        strcpy(command, "DONE");
        while(!strcmp(command, "DONE"))
            usleep(30 * 1000);
    }
}
