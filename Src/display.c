/*
 * display.c
 *
 *  Created on: 26 de jan de 2017
 *      Author: Abdoral Timóteo
 */

#include "i2c.h"

uint8_t clear_screen[] = {0xfe, 0x51};
uint8_t left_cursor[] = {0xfe, 0x49};
uint8_t right_cursor[] = {0xfe, 0x4A};
uint8_t set_cursor_line1[] = {0xfe, 0x45, 0x00};
uint8_t set_cursor_line2[] = {0xfe, 0x45, 0x40};
uint8_t set_cursor_line3[] = {0xfe, 0x45, 0x14};
uint8_t set_cursor_line4[] = {0xfe, 0x45, 0x54};

void initDisplay() {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
}

void clearDisplay() {
	HAL_I2C_Master_Transmit(&hi2c1, 0x50, clear_screen, sizeof(clear_screen), 1000);
    HAL_Delay(50);
}

void printDisplay(char text[]) {
	HAL_I2C_Master_Transmit(&hi2c1, 0x50, text, strlen(text), 1000);
}

void printMenu() {
	char titulo[] = "   BANCO DE DADOS";
    char login[] = "1 Login";
	char cadastrar[] = "4 Cadastrar";
	char deletar[] = "7 Deletar";

	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
	printDisplay(titulo);
	//HAL_I2C_Master_Transmit(&hi2c1, 0x50, titulo, sizeof(titulo), 1000);
	HAL_I2C_Master_Transmit(&hi2c1, 0x50, set_cursor_line2, sizeof(set_cursor_line2), 1000);
	HAL_I2C_Master_Transmit(&hi2c1, 0x50, login, strlen(login), 1000);
	HAL_I2C_Master_Transmit(&hi2c1, 0x50, set_cursor_line3, sizeof(set_cursor_line3), 1000);
	HAL_I2C_Master_Transmit(&hi2c1, 0x50, cadastrar, strlen(cadastrar), 1000);
	//HAL_I2C_Master_Transmit(&hi2c1, 0x50, set_cursor_line4, sizeof(set_cursor_line4), 1000);
	//HAL_I2C_Master_Transmit(&hi2c1, 0x50, deletar, strlen(deletar), 1000);
	HAL_I2C_Master_Transmit(&hi2c1, 0x50, right_cursor, sizeof(right_cursor), 1000);
}

void printMenuInstalacao() {
    char *opt[4] = {"   INICIALIZACAO","A - Utilizar Atual","B - Criar Novo"};

	printDisplay(opt[0]);
	HAL_I2C_Master_Transmit(&hi2c1, 0x50, set_cursor_line2, sizeof(set_cursor_line2), 1000);
	printDisplay(opt[1]);
	HAL_I2C_Master_Transmit(&hi2c1, 0x50, set_cursor_line3, sizeof(set_cursor_line3), 1000);
	printDisplay(opt[2]);
	HAL_I2C_Master_Transmit(&hi2c1, 0x50, right_cursor, sizeof(right_cursor), 1000);
}

void readFromKeyBoard(char key[]) {
	printDisplay(key);
	HAL_I2C_Master_Transmit(&hi2c1, 0x50, right_cursor, sizeof(right_cursor), 1000);
}


