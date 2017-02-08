/*
 * sd_card.h
 *
 *  Created on: 26 de jan de 2017
 *      Author: Abdoral Timóteo, Adriano Pereira, Bruno Amaral
 */

#ifndef SD_CARD_H_
#define SD_CARD_H_

FATFS montaSd();
void formatSd(FATFS SDFatFs);
void createFile();
void writeLine(FIL *MyFile);


#endif /* SD_CARD_H_ */
