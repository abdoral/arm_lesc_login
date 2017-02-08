/*
 * auth.c
 *
 *  Created on: 2 de fev de 2017
 *      Author: Abdoral Neto
 */

#include "fatfs.h"
#include "ff.h"
#include "auth.h"
#include "string.h"


AUTHRESULT log(FIL* file, char* loginpass){
	char line[9];
    if(f_open(file, "database.txt", FA_READ) == FR_OK) {
	    while (f_gets(line, sizeof line, file)) {
		   if(strcmp(line,loginpass)) {
			   return AU_AUTHENTICATED;
		   }
	    }
    }
    f_close(file);
	return AU_FAIL;
}


AUTHRESULT regist(FIL* file, char* login, char* pass){
	uint32_t byteswritten;
	if(f_open(file, "database.txt", FA_WRITE) == FR_OK) {
		f_lseek(file, f_size(file));
		f_write(file,login,sizeof(login),(void*)&byteswritten);
		f_write(file,pass,sizeof(pass),(void*)&byteswritten);
	    f_putc('\n',file);
	    f_close(file);
	    return AU_REGISTERED;
	}

	return AU_FAIL;
}

AUTHRESULT delete(FIL* file, char login[4], char pass[4]){
	return AU_FAIL;
}


