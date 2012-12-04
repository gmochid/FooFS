/*
 * superblock.h
 *
 *  Created on: Dec 3, 2012
 *      Author: gmochid
 */

#ifndef SUPERBLOCK_H_
#define SUPERBLOCK_H_

#include "inode.h"

typedef struct {
	long capacity; // kapasitas total sebuah blok
	long usedCapacity; // kapasitas yang telah digunakan
	long availableCapacity; // kapasitas yang masih kosong
	char* name; // nama dari blok ini
	long sizeBlock; // besar ukuran dari sebuah blok pada filesystem ini
	long totalBlock; // total block pada filesystem ini
	long usedBlock; // total block yang telah dialokasi
	long availableBlock; // total block yang masih dapat dialokasi
	Inode* root; // pointer pada struktur data yg dimiliki superblock ini
} SuperBlock;

/**
 * Membuat SuperBlock baru (DONE)
 * Relasi (capacity = sizeBlock * totalBlock) harus dipenuhi
 */
SuperBlock* createSuperBlock(char* name, long capacity, long sizeBlock, long totalBlock);

/**
 * Memakai resources dari superblock sebesar cap (DONE)
 * cap harus kurang dari atau sama dengan kapasitas yang tersedia
 */
void useResources(SuperBlock* sb, long cap);
/**
 * Melepas resources dari superblock sebesar cap (DONE)
 */
void releaseResources(SuperBlock* sb, long cap);

/**
 * Mendelete sebuah inode beserta semua childnya (DONE)
 */
void deleteInode(SuperBlock* sb, Inode* now, Inode* inode);
/**
 * Meremove memori dari now dan semua block yang dimilikinya (DONE)
 */
void removeInode(SuperBlock* sb, Inode* now);

/**
 * Membuat inode baru (DONE)
 * @return inode inode yang baru dibuat
 */
Inode* createInode(SuperBlock * sb, Inode* parent, char * name, long fileSize, int type);
/**
 * Save sebuah superblock pada sebuah file
 */
void convertToFile(SuperBlock* sb, char* filename); // mikir dulu ngesave ke filenya

/**
 * Mengambil sebuah Inode bisa diberikan path tertentu (DONE)
 * Bila path tidak ditemukan, maka mengembalikan null
 */
Inode* getInodeFromPath(SuperBlock* sb, char* path);
/**
 * Convert sebuah inode pada superblock menjadi string path (DONE)
 */
char* getPathFromInode(SuperBlock* sb, Inode* inode);

/**
 * Memasukkan data pada inode (DONE)
 * Data yang utuh akan dibagi-bagi menjadi beberapa blok
 */
void setBlocksData(SuperBlock* sb, Inode* this, char* data);
/**
 * Mengambil data pada inode (DONE)
 * Data yang telah dibagi-bagi akan disatukan lagi menjadi utuh
 */
char* getBlocksData(SuperBlock* sb, Inode* this);

#endif /* SUPERBLOCK_H_ */
