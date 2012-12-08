/*
 * functional.h
 *
 *  Created on: Dec 4, 2012
 *      Author: gmochid
 */

#ifndef FUNCTIONAL_H_
#define FUNCTIONAL_H_

#include "model/superblock.h"
#include "shm.h"
#include "helper.h"

static char currentDirectory[SIZEDIRECTORY];
static SuperBlock* currentSuperblock;
static char fileFormatPath[STDSIZE];
char* command;

/**
 * not ready
 */
void fsm_FORMAT(char* filepath, long size);
void fsm_MOUNT(char* filepath);
void fsm_UNMOUNT();
/**
 * Mencetak ke layar currentDirectory
 */
void fsm_PWD();
/**
 * Mencetak semua child dari Inode currentDirectory ditambah path
 * USE: getInodeFromPath
 */
void fsm_LS(char * path);
/**
 * Menambahkan atau mengurangi isi dari currentDirectory
 * Menambahkan dengan parameter path string. Contoh: cd abc, cd ab/xy.
 * Mengurangi dengan parameter path "..". Contoh: cd .., cd ../../ .
 * USE: getInodeFromPath, inode->parent dan inode->child
 */
void fsm_CD(char* path);
/**
 * Membuat direktori baru, sebagai child dari currentDirectory
 * USE: getInodeFromPath, createInode
 */
void fsm_MKDIR(char* newDirectory);
/**
 * Delete sebuah Inode (bida folder atau file)
 */
void fsm_RM(char* path);
/**
 * Mencetak status dari sebuah superblock (print semua kecuali name dan root)
 */
void fsm_STATUS();

void fsm_CAT(char* filepath);

void writeToFile(FILE* file, SuperBlock* sb);

void fsm_handleInput();

#endif /* FUNCTIONAL_H_ */
