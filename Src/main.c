/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define USE_HAL_LEGACY
#if defined(USE_HAL_LEGACY)
#include "Legacy/stm32_hal_legacy.h"
#endif
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t KEY_Flag=0;
uint8_t recv_buf[12] = {0};
const uint32_t spwm[1000]= {
    1201,1213,1226,1238,1251,1263,1276,1288,1301,1313,
    1326,1338,1351,1363,1376,1388,1400,1413,1425,1437,
    1450,1462,1474,1486,1498,1511,1523,1535,1547,1559,
    1571,1583,1594,1606,1618,1630,1641,1653,1665,1676,
    1688,1699,1710,1722,1733,1744,1755,1766,1777,1788,
    1799,1810,1821,1831,1842,1853,1863,1873,1884,1894,
    1904,1914,1924,1934,1944,1954,1964,1973,1983,1992,
    2001,2011,2020,2029,2038,2047,2056,2064,2073,2081,
    2090,2098,2106,2115,2123,2130,2138,2146,2154,2161,
    2169,2176,2183,2190,2197,2204,2211,2217,2224,2230,
    2237,2243,2249,2255,2261,2266,2272,2277,2283,2288,
    2293,2298,2303,2308,2313,2317,2321,2326,2330,2334,
    2338,2342,2345,2349,2352,2355,2359,2362,2365,2367,
    2370,2372,2375,2377,2379,2381,2383,2385,2386,2388,
    2389,2390,2392,2393,2393,2394,2395,2395,2395,2395,
    2395,2395,2395,2395,2394,2394,2393,2392,2391,2390,
    2389,2387,2386,2384,2382,2380,2378,2376,2374,2371,
    2369,2366,2363,2360,2357,2354,2351,2347,2343,2340,
    2336,2332,2328,2324,2319,2315,2310,2306,2301,2296,
    2291,2285,2280,2275,2269,2263,2258,2252,2246,2240,
    2233,2227,2221,2214,2207,2201,2194,2187,2179,2172,
    2165,2157,2150,2142,2134,2127,2119,2110,2102,2094,
    2086,2077,2069,2060,2051,2042,2033,2024,2015,2006,
    1997,1987,1978,1968,1959,1949,1939,1929,1919,1909,
    1899,1889,1879,1868,1858,1847,1837,1826,1815,1805,
    1794,1783,1772,1761,1750,1739,1727,1716,1705,1693,
    1682,1670,1659,1647,1636,1624,1612,1600,1589,1577,
    1565,1553,1541,1529,1517,1505,1492,1480,1468,1456,
    1444,1431,1419,1407,1394,1382,1369,1357,1345,1332,
    1320,1307,1295,1282,1270,1257,1245,1232,1219,1207,
    1194,1182,1169,1157,1144,1132,1119,1107,1094,1082,
    1069,1057,1044,1032,1019,1007,995,982,970,958,
    945,933,921,909,897,884,872,860,848,836,
    824,812,801,789,777,765,754,742,730,719,
    707,696,685,673,662,651,640,629,618,607,
    596,585,574,564,553,542,532,522,511,501,
    491,481,471,461,451,441,431,422,412,403,
    394,384,375,366,357,348,339,331,322,314,
    305,297,289,280,272,265,257,249,241,234,
    226,219,212,205,198,191,184,178,171,165,

    158,152,146,140,134,129,123,118,112,107,
    102,97,92,87,82,78,74,69,65,61,
    57,53,50,46,43,40,36,33,30,28,
    25,23,20,18,16,14,12,10,9,7,
    6,5,3,2,2,1,0,0,0,0,
    0,0,0,0,1,1,2,3,4,5,
    6,8,9,11,13,15,17,19,21,24,
    26,29,32,35,38,41,44,48,52,55,
    59,63,67,71,76,80,85,89,94,99,
    104,110,115,120,126,132,137,143,149,155,
    162,168,174,181,188,194,201,208,216,223,
    230,238,245,253,261,268,276,285,293,301,
    309,318,326,335,344,353,362,371,380,389,
    398,408,417,427,436,446,456,466,476,486,
    496,506,516,527,537,548,558,569,580,590,
    601,612,623,634,645,656,668,679,690,702,
    713,725,736,748,759,771,783,795,806,818,
    830,842,854,866,878,890,903,915,927,939,
    951,964,976,988,1001,1013,1026,1038,1050,1063,
    1075,1088,1100,1113,1125,1138,1150,1163,1176,1188,

    1201,1213,1226,1238,1251,1263,1276,1288,1301,1313,
    1326,1338,1351,1363,1376,1388,1400,1413,1425,1437,
    1450,1462,1474,1486,1498,1511,1523,1535,1547,1559,
    1571,1583,1594,1606,1618,1630,1641,1653,1665,1676,
    1688,1699,1710,1722,1733,1744,1755,1766,1777,1788,
    1799,1810,1821,1831,1842,1853,1863,1873,1884,1894,
    1904,1914,1924,1934,1944,1954,1964,1973,1983,1992,
    2001,2011,2020,2029,2038,2047,2056,2064,2073,2081,
    2090,2098,2106,2115,2123,2130,2138,2146,2154,2161,
    2169,2176,2183,2190,2197,2204,2211,2217,2224,2230,
    2237,2243,2249,2255,2261,2266,2272,2277,2283,2288,
    2293,2298,2303,2308,2313,2317,2321,2326,2330,2334,
    2338,2342,2345,2349,2352,2355,2359,2362,2365,2367,
    2370,2372,2375,2377,2379,2381,2383,2385,2386,2388,
    2389,2390,2392,2393,2393,2394,2395,2395,2395,2395,
    2395,2395,2395,2395,2394,2394,2393,2392,2391,2390,
    2389,2387,2386,2384,2382,2380,2378,2376,2374,2371,
    2369,2366,2363,2360,2357,2354,2351,2347,2343,2340,
    2336,2332,2328,2324,2319,2315,2310,2306,2301,2296,
    2291,2285,2280,2275,2269,2263,2258,2252,2246,2240,
    2233,2227,2221,2214,2207,2201,2194,2187,2179,2172,
    2165,2157,2150,2142,2134,2127,2119,2110,2102,2094,
    2086,2077,2069,2060,2051,2042,2033,2024,2015,2006,
    1997,1987,1978,1968,1959,1949,1939,1929,1919,1909,
    1899,1889,1879,1868,1858,1847,1837,1826,1815,1805,
    1794,1783,1772,1761,1750,1739,1727,1716,1705,1693,
    1682,1670,1659,1647,1636,1624,1612,1600,1589,1577,
    1565,1553,1541,1529,1517,1505,1492,1480,1468,1456,
    1444,1431,1419,1407,1394,1382,1369,1357,1345,1332,
    1320,1307,1295,1282,1270,1257,1245,1232,1219,1207,
    1194,1182,1169,1157,1144,1132,1119,1107,1094,1082,
    1069,1057,1044,1032,1019,1007,995,982,970,958,
    945,933,921,909,897,884,872,860,848,836,
    824,812,801,789,777,765,754,742,730,719,
    707,696,685,673,662,651,640,629,618,607,
    596,585,574,564,553,542,532,522,511,501,
    491,481,471,461,451,441,431,422,412,403,
    394,384,375,366,357,348,339,331,322,314,
    305,297,289,280,272,265,257,249,241,234,
    226,219,212,205,198,191,184,178,171,165
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */
    uint16_t pluse = 500;
    uint16_t i = 0;
    /* USER CODE END 1 */


    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_USART1_UART_Init();
    MX_TIM1_Init();
    MX_TIM3_Init();
    /* USER CODE BEGIN 2 */
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);//产生PWM，舵机转动
    HAL_TIM_Base_Start_IT(&htim3);//使能定时器3，频率2Hz
    HAL_UART_Receive_DMA(&huart1, recv_buf, 12);  //使能DMA接收
    printf("Hello, i am %s\r\n", "mculover666");
    printf("Test int: i = %d\r\n", 100);
    printf("Test float: i = %f\r\n", 1.234);
    printf("Test hex: i = 0x%2x\r\n",100);
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        /*判断按键是否按下进行相应操作*/
        switch( KEY_Flag ) {
        case 1:
            /* 按键1按下 */
            //
            pluse += 200;
            HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
            KEY_Flag = 0;
            break;
        case 2:
            /* 按键2按下 */
            //
            pluse -= 200;
            HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
            KEY_Flag = 0;
            break;
        case 3:
            /* 按键13按下 */
            //
            pluse = 5000;
            HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
            KEY_Flag = 0;
            break;
        default:
            break;
        }
        /*执行操作*/
        printf("DATA:%d,%d,%d\n",spwm[i],spwm[i+399],spwm[i+199]);
        if(++i >= 600)  //一周期采样600个点
            i = 0;
        //设置PWM占空比
        if(pluse <= 5000&&pluse >= 500) {
            HAL_Delay(200);//延时等待舵机稳定
            __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, (uint16_t)pluse);
        }
    }
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB busses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    //将接收到的数据再发送
    HAL_UART_Transmit(&huart1,recv_buf,sizeof(recv_buf), 0xFFFF);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

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
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
