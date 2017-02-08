/*
 * sd_card.c
 *
 *  Created on: 26 de jan de 2017
 *      Author: Abdoral Timóteo, Adriano Pereira, Bruno Amaral
 */


#include "fatfs.h"
#include "ff.h"
#include "display.h"

FATFS montaSd() {
	FRESULT res;
	FATFS SDFatFs;
	res  = f_mount(&SDFatFs, (TCHAR const*)SD_Path, 0);
	if(res != FR_OK) {
		clearDisplay();
	    printDisplay("Erro Inicializacao FAT");
		while(1);
	} else {
		clearDisplay();
		printDisplay("SD Montado");
		return SDFatFs;
	}
}

void formatSd(FATFS SDFatFs){
   clearDisplay();
   printDisplay("SD Encontrado");
   HAL_Delay(1000);
   /*##-2- Register the file system object to the FatFs module ###*/
   if(f_mount(&SDFatFs, (TCHAR const*)SD_Path, 0) != FR_OK){
	   /* FatFs Initialization Error*/
	   clearDisplay();
	   printDisplay("Erro Inicializacao FAT");
	   while(1);
   } else {
	  /*##-3- Create a FAT file system (format) on the logical drive#*/
	  /* WARNING: Formatting the uSD card will delete all content on the
  device */
	  if(f_mkfs((TCHAR const*)SD_Path, 0, 0) != FR_OK){
		  /* FatFs Format Error*/
		  clearDisplay();
		  printDisplay("FatFs Format Error");
		  while(1);
	  } else {
		  clearDisplay();
		  printDisplay("SD Formatado");
	  }
  }
}

void createFile(){

}

void writeLine(FIL *MyFile){

}



