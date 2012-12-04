/*
 * functional.h
 *
 *  Created on: Dec 4, 2012
 *      Author: gmochid
 */

#ifndef FUNCTIONAL_H_
#define FUNCTIONAL_H_

#include "model/superblock.h"

void func_FORMAT(char* filepath, long size);
void func_LS(SuperBlock* sb, Inode* sb);

#endif /* FUNCTIONAL_H_ */
