/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <math.h>
#include <stdint.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define KEY_NOT_PRESSED   NULL
#define KEY_PRESSED       0x00
#define PI                3.1415926535898793238462643383279502884197169399375
#define DEBUGGING         0
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//char keypad[4][4] = {
//    {'7', '8', '9', '/'},  // Row 1
//    {'4', '5', '6', '*'},  // Row 2
//    {'1', '2', '3', '-'},  // Row 3
//    {'0', '.', '=', '+'}   // Row 4
//};

char keypad1[4][4][100] = {
    {"7", "8", "9", "/"},  // Row 1
    {"4", "5", "6", "*"},  // Row 2
    {"1", "2", "3", "-"},  // Row 3
    {"0", ".", "=", "+"}   // Row 4
};

char keypad2[4][4][100] = {
    {"(", ")", "^", ""},  // Row 1
    {"sin(", "cos(", "tan(", "log("},  // Row 2
    {"1", "2", "3", "-"},  // Row 3
    {"0", ".", "=", "+"}   // Row 4
};

char user_inputs[75][100] = {};
int current_index = 0;
int new_key;
double calculation;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
void clear_user_inputs(char user_inputs[][100], int* ptr_index);
void compress(char inputs[75][100]);
int get_string_length(char string[100]);
int get_array_length(char arr[75][100]);
void parse_calculations_raw(char inputs[75][100]);
void parse_calculations_bracketless(char inputs[75][100]);
void copy_array(char arr1[], char arr2[100]);
void simplify(char arr[75][100]);
int is_operator(char character);
int is_num(char arr[100]);
double arr_to_num(char arr[100]);
void num_to_arr(double num, char arr[100]);
// double pow(double num, int power);
uint64_t factorial(int num);
void sine(char arr[100]);
void cosine(char arr[100]);
void tangent(char arr[100]);
void logarithm(char arr[100]);
double round(double num);
int check(double num1, double num2);
char* keypad1_scan(void);
char* keypad2_scan(void);
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
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  user_inputs[0][0] = '0';
  user_inputs[1][0] = '+';
  current_index = 2;
  new_key = 1;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  // Delay between user inputs
	  HAL_Delay(10);
	  char* key = keypad1_scan();
	  if (key == KEY_NOT_PRESSED) {
		  key = keypad2_scan();
	  }

	  // Only proceed to process if a key was pressed within previous time frame
	  // Only proceed if the key is a new key pressed (not held down)
	  // Only proceed to process if the string is less than 74 char long
	  // If the key is '=' skip and move to other condition
	  if (key != KEY_NOT_PRESSED && new_key && current_index < 75 && key[0] != '=') {
		  // DEBUG START
		  printf("Key: %s\n", key);
//		  printf("Index: %u\n", current_index);
		  // DEBUG END

		  // Store user inputs as an array
		  int index = 0;
		  while (key[index] != '\0') {
			  user_inputs[current_index][0] = key[index];
			  index++;
			  current_index++;
		  }

		  printf("Current string: ");
		  for (int i = 2; i <= current_index; i++) {
			  printf("%s", user_inputs[i]);
		  }
		  printf("\n");

		  // Call a function to update the LCD display (pass in user inputs), void function
		  //

		  new_key = 0;
	  }
	  else if (key == KEY_NOT_PRESSED){
		  // If key is not pressed, set that a new key is ready to be processed
		  // Turn off debug light
		  new_key = 1;
		  HAL_GPIO_WritePin(Light_bulb_GPIO_Port, Light_bulb_Pin, GPIO_PIN_RESET);
	  }
	  // If pressed key is = then process
	  if (key[0] == '=' && new_key) {
		  printf("%s", key);
		  printf("\n");
		  // Call function to read user input array
		  parse_calculations_raw(user_inputs);
		  printf("%s\n", user_inputs[0]);

		  // Reset user_input array (call function to do so)
		  clear_user_inputs(user_inputs, &current_index);

		  new_key = 0;
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, R4_1_Pin|R3_1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, R3_2_Pin|R4_2_Pin|R1_1_Pin|LD2_Pin
                          |R2_2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, R2_1_Pin|R1_2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : R4_1_Pin R3_1_Pin */
  GPIO_InitStruct.Pin = R4_1_Pin|R3_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : C1_1_Pin C2_1_Pin C3_1_Pin C3_2_Pin
                           C4_2_Pin */
  GPIO_InitStruct.Pin = C1_1_Pin|C2_1_Pin|C3_1_Pin|C3_2_Pin
                          |C4_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : R3_2_Pin R4_2_Pin R1_1_Pin LD2_Pin
                           R2_2_Pin */
  GPIO_InitStruct.Pin = R3_2_Pin|R4_2_Pin|R1_1_Pin|LD2_Pin
                          |R2_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : R2_1_Pin R1_2_Pin */
  GPIO_InitStruct.Pin = R2_1_Pin|R1_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : C2_2_Pin C1_2_Pin C4_1_Pin */
  GPIO_InitStruct.Pin = C2_2_Pin|C1_2_Pin|C4_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */

/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
int _write(int file, char *ptr, int len)
{
  (void)file;
  int DataIdx;

  for (DataIdx = 0; DataIdx < len; DataIdx++)
  {
    ITM_SendChar(*ptr++);
  }
  return len;
}

// Gets the length of string
// Requires that the string ends in null terminator
int get_string_length(char string[100]) {
	int index = 0;
	while (string[index] != '\0') {
		index++;
	}
	return index;
}


// Gets the length of an array of strings
// Requires that the "end" of the array has the null terminator at index 0
int get_array_length(char arr[75][100]) {
	int index = 0;
	while (arr[index][0] != '\0') {
		index++;
	}
	return index;
}


// Sets all characters from index length onwards as the null terminator
void clear_array(char inputs[75][100], int length) {
    for (int i = length; i < 30; i++) {
        for (int j = 0; j < 100; j++) {
            inputs[i][j] = '\0';
        }
    }
}


// Calculates and parses a function with brackets
// A recursive call is made if brackets are found, passing an array of the calculation inside the brackets
// Brackets are replaced with the calculated inside value, shifting everything down once brackets are removed
// The final calculated value is stored in inputs[0]
void parse_calculations_raw(char inputs[75][100]) {
    // Compress the inputs array
    compress(inputs);

    // Initialize variables
    int length = get_array_length(inputs);
    int found_open = 0;
    int open_index = -1;
    int close_index = -1;
    int tracker = 0;

    // ----------- DEBUG --------------
    if (DEBUGGING) {
        for (int i = 0; i < length; i++) {
            printf("Post Compression: %s\n", inputs[i]);
        }
        printf("\n");
    }
    // ----------- DEBUG --------------

    for (int index = 0; index < length; index++) {
        if (inputs[index][0] == '(') {
            tracker++;
            if (!found_open) {
                found_open = 1;
                open_index = index;
            }
        }
        else if (inputs[index][0] == ')') {
            tracker--;
            close_index = index;
        }
        // If the outside-most set of brackets were found, execute the following
        // open_index and close_index hold the index of the corresponding brackets
        if (found_open && tracker == 0) {
            // Creates a new array (inside) to pass through and calculate new value
            int inside_length = close_index - open_index - 1;
            char inside[75][100]; // +1 ensures null terminator
            for (int i = 0; i < inside_length; i++) {
                copy_array(inside[i], inputs[open_index + i + 1]);
            }
            inside[inside_length][0] = '\0';
            parse_calculations_raw(inside);

            // Replace the "(" with the calculated inside result
            copy_array(inputs[open_index], inside[0]);

            // Shift all elements, up to and including the null terminator, down
            for (int i = 0; i < length - close_index; i++) {
                copy_array(inputs[open_index + i + 1], inputs[close_index + i + 1]);
            }
            index = open_index + 1;
            length = get_array_length(inputs);
            found_open = 0;
        }
    }
    parse_calculations_bracketless(inputs);
    num_to_arr(round(arr_to_num(user_inputs[0])), user_inputs[0]);
}


// Compresses the inputs array so that each number is individually stored at one index of the inputs array
/* Ex:
   inputs = { {'4'},
              {'5'},
              {'-'},
              {'2'},
              {'.'},
              {'4'} }

   After compression:
   inputs = { {'4', '5'},
              {'-'},
              {'2', '.', '4'} }
*/
void compress(char inputs[75][100]) {
    // Initialize variables
    int length = get_array_length(inputs);
    int number_start_index = -1;
    int number_length = 0;
    // Loop through each index until null terminator
    for (int i = 0; i < length + 1; i++) {
        char value = inputs[i][0];

        // Enter if loop if the character at index i is a number/decimal
        // Sets index as the start index if not yet found (< 0)
        // Assigns the character in the appropriate position if start index is found
        if ((value >= '0' && value <= '9') || value == '.') {
            number_length++;
            if (number_start_index < 0) {
                number_start_index = i;
                number_length = 0;
            }
            else {
                inputs[number_start_index][number_length] = value;
            }
        }
        else {
            // All numbers are shifted down when the end of a number is found
            if (number_start_index >= 0 && number_length >= 0) {
                for (int i = number_start_index + 1; i <= length; i++) {
                    inputs[i][0] = inputs[i + number_length][0];
                }
                i = number_start_index + 1;
                number_start_index = -1;
            }
        }
        length = get_array_length(inputs);
    }
}


// Copies elements in arr2 to arr1
// Assume arr1 has appropriate size (add assertion for this)
void copy_array(char arr1[], char arr2[100]) {
    int arr2_length = get_string_length(arr2);
    for (int i = 0; i < arr2_length + 1; i++) { // +1 added to copy null terminator
        arr1[i] = arr2[i];
    }
}

void parse_calculations_bracketless(char inputs[75][100]) {
    // Evaluate all trig and log expressions
    int length = get_array_length(inputs);
    // ----------- DEBUG START --------------
    if (DEBUGGING) {
        for (int i = 0; i < length; i++) {
            printf("Bracketless begin: %s\n", inputs[i]);
        }
        printf("\n");
    }
    // ----------- DEBUG END --------------

    int found_function = 0; // true/false
    for (int index = 0; index < length; index++) {
        char value = inputs[index][0];
        if (value == 's') {
            sine(inputs[index+3]);
            found_function = 1;
        }
        else if (value == 'c') {
            cosine(inputs[index+3]);
            found_function = 1;
        }
        else if (value == 't') {
            tangent(inputs[index+3]);
            found_function = 1;
        }
        else if (value == 'l') {
            // log function
            logarithm(inputs[index + 3]);
            found_function = 1;
        }
        if (found_function) {
            copy_array(inputs[index], inputs[index+3]);
            for (int i = 0; i < length - index - 3; i++) {
                copy_array(inputs[index + i + 1], inputs[index + i + 4]);
            }
            found_function = 0;
            length = get_array_length(inputs);
        }
    }


    // ----------- DEBUG START --------------
    if (DEBUGGING) {
        for (int i = 0; i < length; i++) {
            printf("End Trig/Log: %s\n", inputs[i]);
        }
        printf("\n");
    }
    // ----------- DEBUG END --------------
    // At this point there should be no more trig and log expressions

    length = get_array_length(inputs);

    // Evaluate factorials
    // At this point there should be no more factorials expressions
    for (int index = 0; index < length; index++) {
        int factorial_index = -1;

        char value = inputs[index][0];
        if (value == '!') {
            factorial_index = index;
        }
        if (factorial_index > 0 && is_num(inputs[factorial_index - 1])) {
            double num = arr_to_num(inputs[factorial_index - 1]);
            if (num - (int)num > 0) {
                // RETURN AN ERROR (decimal factorial)
            }
            else {
                num = factorial((int)num);
                num_to_arr(num, inputs[factorial_index - 1]);
                for (int i = 0; i < length - factorial_index; i++) {
                    copy_array(inputs[factorial_index + i], inputs[factorial_index + i + 1]);
                }
                length = get_array_length(inputs);
            }
        }
    }

    // ----------- DEBUG START --------------
    if (DEBUGGING) {
        for (int i = 0; i < length; i++) {
            printf("End Factorial: %s\n", inputs[i]);
        }
        printf("\n");
    }
    // ----------- DEBUG END --------------

    length = get_array_length(inputs);

    // Evaluate exponents
    // At this point there should be no more exponents expressions

    for (int index = length - 1; index >= 0; index--) {
        int base_index = -1;
        int pow_index = -1;
        char val = inputs[index][0];
        if (val == '^') {
            if (index == 0) {
                // return no base error
            }
            else if (!is_num(inputs[index - 1])) {
                // base not a num
            }
            else if (!is_num(inputs[index + 1])) {
                // pow not a num
            }
            else {
                base_index = index - 1;
                pow_index = index + 1;
                double base = arr_to_num(inputs[base_index]);
                double power = arr_to_num(inputs[pow_index]);
                double result = pow(base, power);
                num_to_arr(result, inputs[base_index]);
                for (int i = 0; i < length - index + 1; i++) {
                    copy_array(inputs[index + i], inputs[index + i + 2]);
                }
            }
        }
    }


    // ----------- DEBUG START --------------
    if (DEBUGGING) {
        for (int i = 0; i < length; i++) {
            printf("End exp: %s\n", inputs[i]);
        }
        printf("\n");
    }
    // ----------- DEBUG END --------------

    // Evaluate multiplication and division
    length = get_array_length(inputs);
    simplify(inputs);
    if (inputs[0][0] == '-' && inputs[0][1] == '\0') {
        inputs[0][2] = '\0';
        inputs[0][1] = '1';
    }
    length = get_array_length(inputs);

    // ----------- DEBUG START --------------
    if (DEBUGGING) {
        for (int i = 0; i < length; i++) {
            printf("Pre m/d: %s\n", inputs[i]);
        }
        printf("\n");
    }
    // ----------- DEBUG END --------------

    for (int index = 0; index < length; index++) {
        int calculated = 0;
        double first_num = 0;
        int found_first_num = 0;
        length = get_array_length(inputs);
        if (is_num(inputs[index])) {
            first_num = arr_to_num(inputs[index]);
            found_first_num = 1;
        }
        else {
            // RETURN AN ERROR (multiplying/dividing with no first num, NEED TO ACCOUNT FOR + AND -)
        }

        if (index + 1 < length && found_first_num) {
            char value = inputs[index + 1][0];
            if (value == '*') {
                if (index + 2 < length) {
                    if (is_num(inputs[index + 2])) {
                        double second_num = arr_to_num(inputs[index + 2]);
                        first_num *= second_num;
                        num_to_arr(first_num, inputs[index]);
                        for (int i = 0; i < length - index - 2; i++) {
                            copy_array(inputs[index + i + 1], inputs[index + i + 3]);
                        }
                        calculated = 1;
                    }
                    else {
                        // RETURN AN ERROR (multiplying by something that isnt a number)
                    }
                }
                else {
                    // RETURN AN ERROR (multiplying by nothing)
                }
            }
            else if (value == '/') {
                if (index + 2 < length) {
                    if (is_num(inputs[index + 2])) {
                        double second_num = arr_to_num(inputs[index + 2]);
                        if (second_num != 0) {
                            first_num /= second_num;
                            num_to_arr(first_num, inputs[index]);
                            for (int i = 0; i < length - index - 2; i++) {
                                copy_array(inputs[index + i + 1], inputs[index + i + 3]);
                            }
                            calculated = 1;
                        }
                        else {
                            // RETURN AN ERROR (division by 0);
                        }
                    }
                    else {
                        // RETURN AN ERROR (dividing by something that isnt a number)
                    }
                }
                else {
                    // RETURN AN ERROR (dividing by nothing)
                }
            }
            else if (is_num(inputs[index + 1])) {
                // calculate here
                double second_num = arr_to_num(inputs[index + 1]);
                first_num *= second_num;
                num_to_arr(first_num, inputs[index]);
                for (int i = 0; i < length - index - 1; i++) {
                    copy_array(inputs[index + i + 1], inputs[index + i + 2]);
                }
                calculated = 1;
            }
        }

        if (calculated) {
            index--;
        }
    }

    length = get_array_length(inputs);

    // At this point there should be no more multiplication/division

    // Evaluate addition and subtraction
    for (int index = 0; index < length; index++) {
        int found_first_num = 0; // boolean
        double first_num = 0;
        double second_num = 0;
        int calculated = 0;
        if (is_num(inputs[index])) {
            first_num = arr_to_num(inputs[index]);
            found_first_num = 1;
        }

        if (found_first_num && index + 1 < length) {
            char value = inputs[index + 1][0];
            if (value == '+') {
                if (index + 2 < length) {
                    second_num = arr_to_num(inputs[index + 2]);
                    first_num += second_num;
                    num_to_arr(first_num, inputs[index]);
                    for (int i = 0; i < length - index - 2; i++) {
                        copy_array(inputs[index + i + 1], inputs[index + i + 3]);
                    }
                    calculated = 1;
                }
                else {
                    // RETURN AN ERROR (adding by nothing)
                }
            }
            if (value == '-') {
                if (index + 2 < length) {
                    second_num = arr_to_num(inputs[index + 2]);
                    first_num -= second_num;
                    num_to_arr(first_num, inputs[index]);
                    for (int i = 0; i < length - index - 2; i++) {
                        copy_array(inputs[index + i + 1], inputs[index + i + 3]);
                    }
                    calculated = 1;
                }
                else {
                    // RETURN AN ERROR (adding by nothing)
                }
            }
        }
        if (calculated) {
            index--;
        }
    }

    length = get_array_length(inputs);
    // EVERYTHING IS EVALUATED AT THIS POINT
}

void simplify(char arr[75][100]) {
    int length = get_array_length(arr);
    int found_operator = 0; // boolean
    int multiplier = 1;
    int start_index = -1;
    int counter = 0;
    for (int i = 0; i < length; i++) {
        if (is_operator(arr[i][0]) && arr[i][1] == '\0' && found_operator == 0) {
            start_index = i;
            found_operator = 1;
        }
        else {
            if (!is_num(arr[i]) && (arr[i][0] == '+' || arr[i][0] == '-')) {
                counter++;
                if (arr[i][0] == '-') {
                    multiplier *= -1;
                }
            }
            else if (found_operator) {
                double num = arr_to_num(arr[i]);
                num *= multiplier;
                num_to_arr(num, arr[i]);
                for (int j = 0; j < length - start_index; j++) {
                    copy_array(arr[start_index + j + 1], arr[start_index + j + counter + 1]);
                }
                length = get_array_length(arr);
                i = start_index;
                found_operator = 0;
                counter = 0;
                multiplier = 1;
            }
        }
    }
}

double arr_to_num(char arr[100]) {
    int index = 0;
    double num = 0;
    int is_neg = 0;
    if (arr[index] == '-') {
        is_neg = 1;
        index++;
    }
    while (arr[index] != '\0' && arr[index] != '.') {
        num *= 10;
        char character = '0';
        // ADD CHECK IF THERE IS UNKNOWN CHARACTER (to_add = -1 or something)
        for (int i = 0; character + i <= '9'; i++) {
            if (arr[index] == (character + i)) {
                num += i;
            }
        }
        index++;
    }
    if (arr[index] == '.') {
        index++;
        double divisor = 1;
        while (arr[index] != '\0') {
            divisor *= 10;
            char character = '0';
            // ADD CHECK IF THERE IS UNKNOWN CHARACTER (to_add = -1 or something)
            for (int i = 0; character + i <= '9'; i++) {
                if (arr[index] == (character + i)) {
                    num += i/divisor;
                }
            }
            index++;
        }
    }
    if (is_neg) {
        num *= -1;
    }
    return num;
}

void num_to_arr(double num, char arr[100]) {
    int index = 0;
    if (num == 0) {
        arr[0] = '0';
        arr[1] = '\0';
        return;
    }
    if (num < 0) {
        arr[0] = '-';
        num *= -1;
        index++;
    }
    int whole_num = (int)num;
    double fractional_num = num - whole_num;

    // placing in whole_num digits
    int num_digits = 0;
    int temp = whole_num;
    while (temp != 0) {
        num_digits += 1;
        temp /= 10;
    }
    for (int i = num_digits - 1; i >= 0; i--) {
        arr[i + index] = (whole_num % 10) + '0';
        whole_num /= 10;
    }
    index += num_digits;
    if (fractional_num != 0) {
        // place decimal
        arr[index] = '.';
        index++;

        // placing in decimal digits (need to account for floating point errors)
        while (fractional_num > 0.0001 && fractional_num < 0.9999) {
            fractional_num *= 10;
            if (fractional_num - ((int)fractional_num + 1) > -0.0001) {
                fractional_num += 1;
            }
            arr[index] = (int)fractional_num + '0';
            fractional_num -= (int)fractional_num;
            index++;
        }
    }

    arr[index] = '\0';
}

int is_operator(char character) {
    if (character == '+' || character == '-' || character == '*' || character == '/') {
        return 1;
    }
    return 0;
}

int is_num(char arr[100]) {
    char value = arr[0];
    if ((value >= '0' && value <= '9') || value == '.' || (value == '-' && ((arr[1] >= '0' && arr[1] <= '9') || arr[1] == '.'))) {
        return 1;
    }
    return 0;
}

// double pow(double num, int power) {
//     double result = 1;
//     for (int i = 0; i < power; i++) {
//         result *= num;
//     }
//     return result;
// }

uint64_t factorial(int num) {
    uint64_t result = 1;
    for (int i = num; i > 0; i--) {
        result *= i;
    }
    return result;
}

void sine(char arr[100]) {
    double num = arr_to_num(arr);
    while (!(num <= 180 && num > -180)) {
        num -= 360;
    }
    num *= (PI/180);
    double result = 0;
    for (int i = 0; i < 10; i++) {
        result += (pow(-1, i) * pow(num, 2*i + 1) / factorial(2*i + 1));
    }
    result = round(result);
    num_to_arr(result, arr);
}

void cosine(char arr[100]) {
    double num = arr_to_num(arr);
    while (!(num <= 180 && num > -180)) {
        num -= 360;
    }
    num *= (PI/180);
    double result = 0;
    for (int i = 0; i < 10; i++) {
        result += (pow(-1, i) * pow(num, 2*i) / factorial(2*i));
    }
    result = round(result);
    num_to_arr(result, arr);
}

// ADD CHECK FOR DIVISION BY 0
void tangent(char arr[100]) {
    double num = arr_to_num(arr);
    sine(arr);
    double sined = arr_to_num(arr);
    num_to_arr(num, arr);
    cosine(arr);
    double cosined = arr_to_num(arr);
    double result = round(sined / cosined);
    if (cosined > 0.00001 || cosined < -0.00001) {
        // printf("%f\n", (sined/cosined * 10000000000));
        num_to_arr(result, arr);
    }
    else {
        return;
        // return error
    }
}

void logarithm(char arr[100]) {
    double num = arr_to_num(arr);
    if (num <= 0) {
        // return error
    }
    double result = log10(num);
    result = round(result);
    num_to_arr(result, arr);
}

double round(double num) {
    num *= 100000000;
    num = (int)num;
    num /= 100000000;
    return num;
}

int check(double num1, double num2) {
    num1 *= 100000;
    num1 = (int)num1;
    num1 /= 100000;
    num2 *= 100000;
    num2 = (int)num2;
    num2 /= 100000;
    if (num1 == num2) {
        return 1;
    }
    return 0;
}


void clear_user_inputs(char user_inputs[][100], int* ptr_index) {
	for (int index = 0; index < 76; index++) {
		for (int inner = 0; inner < 100; inner++) {
			user_inputs[index][inner] = '\0';
		}
	}
	user_inputs[0][0] = '0';
	user_inputs[1][0] = '+';
	*ptr_index = 2;
	printf("Cleared\n");
}


char* keypad1_scan(void) {
	for (int row = 1; row < 5; row++) {
		HAL_GPIO_WritePin(R1_1_GPIO_Port, R1_1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(R2_1_GPIO_Port, R2_1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(R3_1_GPIO_Port, R3_1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(R4_1_GPIO_Port, R4_1_Pin, GPIO_PIN_RESET);
		if (row == 1) {
			HAL_GPIO_WritePin(R1_1_GPIO_Port, R1_1_Pin, GPIO_PIN_SET);
		}
		else if (row == 2) {
			HAL_GPIO_WritePin(R2_1_GPIO_Port, R2_1_Pin, GPIO_PIN_SET);
		}
		else if (row == 3) {
			HAL_GPIO_WritePin(R3_1_GPIO_Port, R3_1_Pin, GPIO_PIN_SET);
		}
		else if (row == 4) {
			HAL_GPIO_WritePin(R4_1_GPIO_Port, R4_1_Pin, GPIO_PIN_SET);
		}

		for (int col = 1; col < 5; col++) {
			GPIO_TypeDef* col_port;
			uint16_t col_pin;
			if (col == 1) {
				col_port = C1_1_GPIO_Port;
				col_pin = C1_1_Pin;
			}
			else if (col == 2) {
				col_port = C2_1_GPIO_Port;
				col_pin = C2_1_Pin;
			}
			else if (col == 3) {
				col_port = C3_1_GPIO_Port;
				col_pin = C3_1_Pin;
			}
			else if (col == 4) {
				col_port = C4_1_GPIO_Port;
				col_pin = C4_1_Pin;
			}
			GPIO_PinState pin_state = HAL_GPIO_ReadPin(col_port, col_pin);
			if (pin_state == GPIO_PIN_SET) {
				return keypad1[row - 1][col - 1];
			}
		}
	}
	return KEY_NOT_PRESSED;
}


char* keypad2_scan(void) {
	for (int row = 1; row < 5; row++) {
		HAL_GPIO_WritePin(R1_2_GPIO_Port, R1_2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(R2_2_GPIO_Port, R2_2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(R3_2_GPIO_Port, R3_2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(R4_2_GPIO_Port, R4_2_Pin, GPIO_PIN_RESET);
		if (row == 1) {
			HAL_GPIO_WritePin(R1_2_GPIO_Port, R1_2_Pin, GPIO_PIN_SET);
		}
		else if (row == 2) {
			HAL_GPIO_WritePin(R2_2_GPIO_Port, R2_2_Pin, GPIO_PIN_SET);
		}
		else if (row == 3) {
			HAL_GPIO_WritePin(R3_2_GPIO_Port, R3_2_Pin, GPIO_PIN_SET);
		}
		else if (row == 4) {
			HAL_GPIO_WritePin(R4_2_GPIO_Port, R4_2_Pin, GPIO_PIN_SET);
		}

		for (int col = 1; col < 5; col++) {
			GPIO_TypeDef* col_port;
			uint16_t col_pin;
			if (col == 1) {
				col_port = C1_2_GPIO_Port;
				col_pin = C1_2_Pin;
			}
			else if (col == 2) {
				col_port = C2_2_GPIO_Port;
				col_pin = C2_2_Pin;
			}
			else if (col == 3) {
				col_port = C3_2_GPIO_Port;
				col_pin = C3_2_Pin;
			}
			else if (col == 4) {
				col_port = C4_2_GPIO_Port;
				col_pin = C4_2_Pin;
			}
			GPIO_PinState pin_state = HAL_GPIO_ReadPin(col_port, col_pin);
			if (pin_state == GPIO_PIN_SET) {
				return keypad2[row - 1][col - 1];
			}
		}
	}
	return KEY_NOT_PRESSED;
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
