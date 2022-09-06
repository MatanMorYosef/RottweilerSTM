#include <stdio.h>
#include "main.h"
#include "uart.h"
#include "EXTI.h"
#include "TIM4Config.h"

#define MAX_UART_PACKET_SIZE			37
/* Private defines ----------------------------------------------------------*/
#define PI 										3.1416
#define dAngle								360.0 / 3600.0

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
void handlePacket(void);
void uart_write(int ch);



/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t TxData[20] = "serial print\r\n";
uint8_t RxData[MAX_UART_PACKET_SIZE];
volatile unsigned long millis = 0,tp = 0,counter=0;
int8_t rCmd [2] = {0.0, 0.0}; // structure of right and left cmd is : {pwmSig, dirSig}
int8_t lCmd [2] = {0.0, 0.0};
double angleRef = 0.0;
int8_t parkingCmd = 0;
char pivotCmd = 'O';
char brakeCmd = 'B';
char cmdMsg = 'Y';
int lightsCmd = 0;

uint8_t x[MAX_UART_PACKET_SIZE + 1];


volatile double angle = 0.0;
volatile int dir = 0;  

struct __FILE {
    int dummy;
};
 
/* You need this if you want use printf */
/* Struct FILE is implemented in stdio.h */
FILE __stdout;
 
int fputc(int ch, FILE *f) {
	
    uart2_write(ch);
    
    return ch;
    
}

/* USER CODE END 0 */



int main(void)
{

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();


  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
	
	
	// configure the encoder phase A to be an interrupt pin on PD0 and phase B to be on PD1
	EXTI_Config(); //define pin PD0 as input pull-up and pin PD1 as input pull-up pin
  TIM4_Config();
	TIM3_Config();
	/* USER CODE BEGIN 2 */
	
	//HAL_UART_Receive_DMA(&huart2, RxData, MAX_UART_PACKET_SIZE);
	HAL_UART_Receive_IT(&huart2, RxData, MAX_UART_PACKET_SIZE);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		if (millis - tp > 2000){
			//printf("%f\r\n",angle);
			tp = millis;
		}
		
  }
  /* USER CODE END 3 */
}

void handlePacket(void){
	
		int8_t commaCounter = 0, sign = 1;
    int temp = 0;

    for (int i = 0; x[i] != ']'; i++) {
      if (x[i] == ' ' || x[i] == '[')
        continue;
      if ((x[i] == ',') && (commaCounter <= 7)) {
        commaCounter++;
        switch (commaCounter) {
          case 1:
            lCmd[0] = temp;
            break;
          case 2:
            lCmd[1] = sign * temp;
            break;
          case 3:
            rCmd[0] = temp;
            break;
          case 4:
            rCmd[1] = sign * temp;
            break;
          case 5:
            angleRef = sign * temp * PI / 180;
            break;
          case 6:
            lightsCmd = sign;
            break;
          case 7:
            parkingCmd = sign;
            break;
        }
        temp = 0;
        sign = 1;
      }
      else {
        if (x[i] == '-')                  // if signed
          sign = -1;
        else if (x[i] == 'R' || x[i] == 'L' || x[i] == 'O')   // if pivot
          pivotCmd = x[i];
        else if (x[i] == 'G' || x[i] == 'B' )    //if brake command
          brakeCmd = x[i];
        else                              // must be digit
          temp = 10 * temp + x[i] - '0';
      }
		}
		
		printf("l_dc: %d | l_dir: %d | r_dc: %d  | r_dir: %d | steer anngle: %f\r\n",lCmd[0], lCmd[1], rCmd[0], rCmd[1], angleRef*180/PI);
}
	




 void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	 
	 UNUSED(huart);
	 
	 memcpy(x,RxData,sizeof(RxData));
	 
	 //HAL_UART_Transmit(&huart2, x, MAX_UART_PACKET_SIZE, 10);
	 HAL_UART_Receive_IT(&huart2, RxData, MAX_UART_PACKET_SIZE);
	 
	 handlePacket();
 }
 
 void TIM3_IRQHandler (void){
	
	printf("%lu    %lu\r\n",millis,counter++);
	
	
	TIM3->SR &= ~(TIM_SR_UIF);
	
}






































/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

}



/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

