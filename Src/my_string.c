/*
 * my_sring.c
 *
 *  Created on: 2 de fev de 2017
 *      Author: Abdoral Neto, Adriano Pereira, Bruno Amaral
 */

#include "string.h"

void concat(char str1[], char str2[]) {
	char buffer[256];
	buffer[0] = '\0';
    strcpy(buffer, str1);
    strcat(buffer, str2);
    strcpy(str1,buffer);
}


