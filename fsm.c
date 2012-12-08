/*
 * functional.c
 *
 *  Created on: Dec 8, 2012
 *      Author: rosyid
 */

#include "fsm.h"

void fsm_MKDIR(char* newDirectory) {
    char ** str_tok;
    char temp[1 << 12];
    strcpy(temp, currentDirectory);
    strcat(temp, newDirectory);
    char res[1 << 10];
    parse_path(&str_tok, temp, res);
    Inode* now = getInodeFromPath(currentSuperblock, res);
    if(now == NULL) {
        Inode* Parent=getInodeFromPath(currentSuperblock,currentDirectory); //mengambil inode dari direktori yang sedang digunakan, untuk dijadikan parent dari direktori baru
        createInode(currentSuperblock, Parent, newDirectory, 0, 0); //karena yang akan dibuat adalah direktori, maka parameter 'type' diisi 0
        //dan karena itu pula parameter 'filesize' diisi 0
        printf("Directory %s created\n", res);
        return;
    }
    printf("ERROR!\nDirectory or file %s exist\n", res);
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
    char ** str_tok;
    char temp[1 << 12];
    strcpy(temp, currentDirectory);
    strcat(temp, path);
    char res[1 << 10];
    parse_path(&str_tok, temp, res);

    Inode* cwd = getInodeFromPath(currentSuperblock, res);
    if(cwd == NULL) {
        printf("ERROR!\nFile or directory not exist\n");
        return;
    }
    deleteInode(currentSuperblock, currentSuperblock->root, cwd);
    printf("File %s deleted\n", res);
}

void fsm_CD(char* path) {
    char ** str_tok;
    char temp[1 << 12];
    strcpy(temp, currentDirectory);
    strcat(temp, path);
    char res[1 << 10];
    parse_path(&str_tok, temp, res);
    Inode* inode = getInodeFromPath(currentSuperblock, res);
    if(inode == NULL) {
        printf("ERROR!\nPath %s doesn't exist\n", res);
        return;
    }
    if(inode->type) {
        printf("ERROR!\n%s is not a directory\n", res);
        return;
    }
    strcpy(currentDirectory, res);
    printf("Directory changed to %s\n", res);
}

void fsm_LS(char* path) {
    char cwd[1 << 10];
    strcpy(cwd, currentDirectory);
    if(path != NULL) {
        strcat(cwd, path);
    }

    char ** str_tok;
    char res[1 << 10];
    parse_path(&str_tok, cwd, res);


    Inode* inode = getInodeFromPath(currentSuperblock, res);
    if(inode == NULL) {
        printf("ERROR!\nPath %s doesn't exist\n", res);
        return;
    }
    if(inode->type) {
        printf("ERROR!\n%s is not a directory\n", res);
        return;
    }
    if(inode->child == NULL) {
        printf("Directory %s is empty\n", res);
        return;
    }

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
    Inode* inode = createInode(sb, NULL, "root", 0 ,0);

    FILE* file = fopen(filepath, "w+");
    writeToFile(file, sb);
    fclose(file);
}

void fsm_MOUNT(char* filepath, char* wd){
    removeFront(filepath, 1);
    FILE* file = fopen(filepath, "r+");
    strcpy(fileFormatPath, filepath);
    char name[STDSIZE];
    long sizeB, capacity, totalB;
    int tp;
    Inode* inode;
    Block* block = NULL;


    fscanf(file, "%s", name);
    fscanf(file, "%ld %ld %ld", &sizeB, &capacity, &totalB);
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
            fgets(data, currentSuperblock->sizeBlock, file);
            if(block == NULL) {
                block = inode->block;
                strcpy(block->data, data);
            } else {
                strcpy(block->data, data);
            }
            block = block->nextBlock;
        }
        fscanf(file, "%d", &tp);
    }
    strcpy(currentDirectory, wd);
    printf("Filesystem mounted successfully to %s\n", currentDirectory);
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
    char cwd[STDSIZE], buffer[STDSIZE];
    char ** token;
    strcpy(cwd, currentDirectory);
    strcat(cwd, filepath);
    parse_path(&token, cwd, buffer);
    Inode* inode = getInodeFromPath(currentSuperblock, buffer);
    if(inode == NULL) {
        printf("Path doesn't exist\n");
        return;
    }
    if(inode->type == 0) {
        printf("%s not a file\n", buffer);
        return;
    }
    printf("File %s content:\n%s\n", getBlocksData(currentSuperblock, inode));
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

void fsm_CP(char* pathfrom, char* pathto) {
    if(pathfrom[0] == '@') { // copy dari OS ke filesystem
        removeFront(pathfrom, 1);

        printf("%s\n", pathfrom);
        FILE* file = fopen(pathfrom, "w+");
        if(file == NULL) {
            printf("Reading file %s\n", pathfrom);
            return;
        }
        //printf("%d\n", fd);
        //printf("%d\n", size);

        char data[STDSIZE];
        char temp[STDSIZE];
        //fgets(temp, STDSIZE, file);
        //strcat(data, temp);
        strcpy(data, "");
        while(fscanf(file, "%s", temp) != EOF) {
            strcat(data, temp);
            printf("-%s\n", temp);
        }
        printf("--%s\n", data);

        int isize = strlen(data);
        //size harus bisa dibagi 4
        if(isize % 4 == 1) {
            isize+=3;
        } else if(isize % 4 == 2) {
            isize+=2;
        } else if(isize % 4 == 3) {
            isize+=1;
        }

        Inode* inode = createInode(currentSuperblock,
                getInodeFromPath(currentSuperblock, pathto),
                pathfrom, isize, 1);
        setBlocksData(currentSuperblock, inode, data);

    } else if(pathto[0] == '@') { // copy filesytem ke OS
        char* data = getBlocksData(currentSuperblock, getInodeFromPath(currentSuperblock, pathfrom));

        removeFront(pathto, 1);
        strcat(pathto, "/test");

        int fd = open(pathto, O_RDWR | O_APPEND | O_CREAT, 0666);
        write(fd, data, strlen(data));

    } else if((pathfrom[0] != '@') && (pathto[0] != '@')) {
        Inode* from = getInodeFromPath(currentSuperblock, pathfrom);
        Inode* to = getInodeFromPath(currentSuperblock, pathto);
        if((from != NULL) && (to != NULL)) {
            copyInode(currentSuperblock, from, to);
        } else {
            printf("ERROR!\n<pathfrom> or <pathto> not exist\n");
        }

    } else {
        printf("Cant copy file from fs OS to fs OS\n");
    }
}

void fsm_MV(char* pathfrom, char* pathto) {
    if(pathfrom[0] == '@') {
    } else if(pathto[0] == '@') {
    } else if((pathfrom[0] != '@') && (pathto[0] != '@')) {
        Inode* from = getInodeFromPath(currentSuperblock, pathfrom);
        Inode* to = getInodeFromPath(currentSuperblock, pathto);
        if((from != NULL) && (to != NULL)) {
            moveInode(currentSuperblock, from, to);
        } else {
            printf("ERROR!\n<pathfrom> or <pathto> not exist\n");
        }
    } else {
        printf("Cant move file from fs OS to fs OS\n");
    }
}

void fsm_handleInput() {
    command = open_shm();

    while(1) {
        char **token;
        char g[STDSIZE];
        strcpy(g, command);
        int len = tokenizer(g, &token, ' ', 100);

        if(!strcmp(*(token), "mount")) {
            fsm_MOUNT(*(token + 1), *(token + 2));
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
                printf("Use CD <path>\n");
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
            if(len == 3) {
                fsm_CP(*(token+1), *(token+2));
            } else  {
                printf("Use CP <pathfrom> <pathto>");
            }
        } else if(!strcmp(*(token), "mv")) {
            if(len == 3) {
                fsm_MV(*(token+1), *(token+2));
            } else  {
                printf("Use MV <pathfrom> <pathto>");
            }
        } else if(!strcmp(*(token), "cat")) {
            if(len == 2) {
                fsm_CAT(*(token + 1));
            } else {
                printf("Use CAT <filepath>\n");
            }
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
