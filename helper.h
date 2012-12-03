/*
 * helper.h
 *
 *  Created on: Dec 3, 2012
 *      Author: gmochid
 */

#ifndef HELPER_H_
#define HELPER_H_

int tokenizer(char* str, char*** arr_token, char tok, int max_arr_token);
void bufferFile(char* path, char ** buff, int size_buff);
void bufferFileTo(char* path, char*toPath, char** buff, int size_buff);

#endif /* HELPER_H_ */
