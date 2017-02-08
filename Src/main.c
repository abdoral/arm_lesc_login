/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "fatfs.h"
#include "i2c.h"
#include "sdio.h"
#include "tim.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "display.h"
#include "string.h"
#include "my_string.h"
#include "auth.h"

typedef enum {
	ST_STARTUP = 0,			/* (0) Inicialização do sistema */
	ST_EXCECUTION,			/* (1) Sistema em execução */
	ST_LOGIN,				/* (2) Realizando Login */
	ST_REGISTER,			/* (3) Realizando Cadastro */
	ST_DELETE,				/* (4) Deletando Cadastro */
} STATE;

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
	char *key;
	char login[9]; char pass[9]; char aux[9]; char loginpass[9];
	FRESULT res; /*Fatfs function common result code*/
	uint32_t byteswritten, bytesread; /* File write/read counts */
	FATFS SDFatFs;
	FIL MyFile;
	char rtext[256]; /* File read buffer */
	uint_fast32_t length = 0;
	STATE state = ST_STARTUP;
	login[0] = '\0';
	pass[0] = '\0';

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SDIO_SD_Init();
  MX_FATFS_Init();
  MX_I2C1_Init();
  MX_TIM13_Init();

  /* USER CODE BEGIN 2 */
  initDisplay();
  if(retSD == 0) {
	  	  clearDisplay();
	      printDisplay("SD Encontrado");
	      HAL_Delay(1000);
	      //Monta SD
          if(f_mount(&SDFatFs, (TCHAR const*)SD_Path, 0) != FR_OK) {
        	  clearDisplay();
        	  printDisplay("Erro Inicializacao FAT");
        	  while(1);
          }
          clearDisplay();
          printDisplay("SD Montado");
          HAL_Delay(1000);
  } else {
	  clearDisplay();
	  printDisplay("NADA ENCONTRADO!");
	  while(1);
  }

  clearDisplay();
  printMenuInstalacao();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  key = readKeyBoard();

      //Iniciar a aplicação a partir de uma base existente
	  if(key == "A" && state == ST_STARTUP) {
		  clearDisplay();
		  printDisplay("Carregando Base...");
		  HAL_Delay(1000);
		  if(f_open(&MyFile, "database.txt", FA_READ ) != FR_OK) {
			  clearDisplay();
			  printDisplay("Base Inexistente!   ");
			  printDisplay("Crie uma base");
			  HAL_Delay(2000);
			  clearDisplay();
			  printMenuInstalacao();
			  key = " ";
			  continue;
		  } else {
			  clearDisplay();
			  printDisplay("Base Localizada!");
			  //Exibe menu da aplicação
			  HAL_Delay(2000);
			  clearDisplay();
			  printMenu();
			  key = " ";
			  state = ST_EXCECUTION;
			  f_close(&MyFile);
			  continue;
		  }
	  }

	  //Formata o Sd e cria uma nova base de dados
	  if(key == "B" && state == ST_STARTUP) {
		  clearDisplay();
		  printDisplay("Criando Novo...");
		  HAL_Delay(1000);
		  //Formata SD
		  if(f_mkfs((TCHAR const*)SD_Path, 0, 0) != FR_OK) {
			  clearDisplay();
			  printDisplay("Erro Formatacao");
			  HAL_Delay(2000);
			  clearDisplay();
			  printMenuInstalacao();
			  key = " ";
			  continue;
		  }
		  //Abre arquivo para escrita
		  if(f_open(&MyFile, "database.txt", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK) {
			  clearDisplay();
			  printDisplay("Erro abrir arq");
			  HAL_Delay(2000);
			  clearDisplay();
			  printMenuInstalacao();
			  key = " ";
			  continue;
		  }

		  //Mensagem de confimação de formatação
	      clearDisplay();
	      printDisplay("Instalacao Concluida!");
	      //Exibe menu da aplicação
	      HAL_Delay(2000);
	      clearDisplay();
	      printMenu();
	      key = " ";
	      state = ST_EXCECUTION;
	      f_close(&MyFile);
	      continue;
	  }

	  //Autentica o usuário
	  if(key == "1" && state == ST_EXCECUTION) {
		  state = ST_LOGIN;
		  clearDisplay();
		  printDisplay("Login:");
		  key = " ";
		  login[0] = '\0';
		  pass[0] = '\0';
		  loginpass[0] = '\0';
		  while(state == ST_LOGIN) {
			  HAL_Delay(500);
			  do{
				  key = readKeyBoard();
				  HAL_Delay(200);
				  if(key != "E") {
					  strcat(login,key);
					  clearDisplay();
					  printDisplay("Login:");
					  printDisplay(login);
				  }
			  } while(strlen(login) < 4);
			  HAL_Delay(500);
			  clearDisplay();
			  printDisplay("Senha:");
			  do{
				  key = readKeyBoard();
				  HAL_Delay(200);
				  if(key != "E") {
					  strcat(pass,key);
					  printDisplay("*");
				  }
			  } while(strlen(pass) < 4);
			  strcat(loginpass,login);
			  strcat(loginpass,pass);
			  HAL_Delay(1000);
			  char line[9]; char* res;
			  if(f_open(&MyFile, "database.txt", FA_READ) == FR_OK) {
				  while (f_gets(line, sizeof line, &MyFile)) {
					  if(strcmp(line,loginpass) == 0) {
						  res = "Autenticado!";
						  break;
					  } else {
						  res = "Inexistente!";
						  continue;
					  }
				  }
				  clearDisplay();
				  printDisplay(res);
				  if(res == "Inexistente!") {
					  HAL_TIM_Base_Start_IT(&htim13);
					  HAL_Delay(1000);
					  HAL_TIM_Base_Stop_IT(&htim13);
				  }else {
					  HAL_Delay(1000);
				  }
				  state = ST_EXCECUTION;
			  } else {
				  clearDisplay();
				  printDisplay("Arq N Econtrado");
				  HAL_Delay(1000);
			  }
			  f_close(&MyFile);
		  }
		  clearDisplay();
		  printMenu();
		  key = " ";
		  continue;
	  }

	  //Cadastra o usuário
	  if(key == "4" && state == ST_EXCECUTION) {
		  state = ST_REGISTER;
		  clearDisplay();
		  printDisplay("Login:");
		  key = " ";
		  login[0] = '\0';
		  pass[0] = '\0';
		  while(state == ST_REGISTER) {
			  HAL_Delay(500);
			  do{
				  key = readKeyBoard();
				  HAL_Delay(200);
				  if(key != "E") {
					  strcat(login,key);
					  clearDisplay();
					  printDisplay("Login:");
					  printDisplay(login);
				  }

			  } while(strlen(login) < 4);
			  HAL_Delay(500);
			  clearDisplay();
			  printDisplay("Senha:");
			  do{
				  key = readKeyBoard();
				  HAL_Delay(200);
				  if(key != "E") {
					  strcat(pass,key);
					  printDisplay("*");
				  }

			  } while(strlen(pass) < 4);
			  HAL_Delay(1000);
			  if(regist(&MyFile,login,pass) == AU_REGISTERED) {
				  clearDisplay();
				  printDisplay("Cadastrado");
				  HAL_Delay(1000);
				  state = ST_EXCECUTION;
			  } else {
				  clearDisplay();
				  printDisplay("Nao Cadastrado!");
				  HAL_Delay(1000);
				  state = ST_EXCECUTION;
			  }
		  }
		  clearDisplay();
		  printMenu();
		  key = " ";
		  continue;
	  }

	  //TODO: implementar função de deletar registro
	  /*if(key == "7" && state == ST_EXCECUTION) {
		  state = ST_DELETE;
		  clearDisplay();
		  printDisplay("Login:");
		  key = " ";
		  login[0] = '\0';
		  pass[0] = '\0';
		  loginpass[0] = '\0';
		  while(state == ST_DELETE) {
			  HAL_Delay(500);
			  do{
				  key = readKeyBoard();
				  HAL_Delay(200);
				  if(key != "E") {
					  strcat(login,key);
					  clearDisplay();
					  printDisplay("Login:");
					  printDisplay(login);
				  }
			  } while(strlen(login) < 4);
			  HAL_Delay(500);
			  clearDisplay();
			  printDisplay("Senha:");
			  do{
				  key = readKeyBoard();
				  HAL_Delay(200);
				  if(key != "E") {
					  strcat(pass,key);
					  printDisplay("*");
				  }
			  } while(strlen(pass) < 4);
			  strcat(loginpass,login);
			  strcat(loginpass,pass);
			  HAL_Delay(1000);
			  char line[9]; char* res;
			  if(f_open(&MyFile, "database.txt", FA_READ) == FR_OK) {
				  while (f_gets(line, sizeof line, &MyFile)) {
					  if(strcmp(line,loginpass) == 0) {
						  res = "Autenticado!";
						  break;
					  } else {
						  res = "Inexistente!";
						  continue;
					  }
				  }
				  clearDisplay();
				  printDisplay(res);
				  HAL_Delay(1000);
				  state = ST_EXCECUTION;
			  } else {
				  clearDisplay();
				  printDisplay("Arq N Econtrado");
				  HAL_Delay(1000);
			  }
			  f_close(&MyFile);
		  }
		  clearDisplay();
		  printMenu();
		  key = " ";
		  continue;
	  }*/


	  if(key != " " && key != "E" && key != "A" && key != "B" && state == ST_STARTUP) {
		  clearDisplay();
		  printDisplay("Funcao Indisponivel");
		  HAL_Delay(2000);
		  clearDisplay();
		  printMenuInstalacao();
		  key = " ";
		  continue;
	  }

	  if(key != " " && key != "E" && key != "1" && key != "4" && state == ST_EXCECUTION) {
		  clearDisplay();
		  printDisplay("Funcao Indisponivel");
		  HAL_Delay(2000);
		  clearDisplay();
		  printMenu();
		  key = " ";
		  continue;
	  }
	  key = " ";

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDIO|RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48CLKSOURCE_PLLQ;
  PeriphClkInitStruct.SdioClockSelection = RCC_SDIOCLKSOURCE_CLK48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim == &htim13) {
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_12);
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
