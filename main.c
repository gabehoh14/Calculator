#define KEY_NOT_PRESSED   NULL
char keypad[4][4][100] = {
    {"7", "8", "9", "/"},  // Row 1
    {"4", "5", "6", "*"},  // Row 2
    {"1", "2", "3", "-"},  // Row 3
    {"0", ".", "=", "+"}   // Row 4
};
char user_inputs[17][100] = {};
int current_index = 0;
int new_key;
double calculation;
char* NULL = 0;


int main();
char* parse_calculations_raw(char** inputs, int needs_compression);
char* parse_calculations_bracketless(char** inputs);
void compress(char** inputs);
int get_string_length(char** string);
char* sine(char* num);
char* cosine(char* num);
char* tangent(char* num);
void clear_user_inputs(char user_inputs[][100], int* index);
char* keypad_scan(void);




int main() {
    while (1)
    {
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */

        // Delay between user inputs
        HAL_Delay(10);
        char* key = keypad_scan();

        // Only proceed to process if a key was pressed within previous time frame
        // Only proceed if the key is a new key pressed (not held down)
        // Only proceed to process if the string is less than 15 char long
        // If the key is '=' skip and move to other condition
        if (key != KEY_NOT_PRESSED && new_key && current_index < 16 && key[0] != '=') {
            // Store user inputs as an array
            int index = 0;
            while (key[index] != '\0') {
                user_inputs[current_index][index] = key[index];
                index++;
            }


            // PRINT KEY INDEX AND STRING
            printf("Key: %s\n", key);
            printf("Index: %u\n", current_index);                       
            printf("Current string: ");
            for (int i = 0; i <= current_index; i++) {
                printf("%s", user_inputs[i]);
            }
            printf("\n");
            // PRINT END


            // Call a function to update the LCD display (pass in user inputs), void function
            // FUNCTION TO BE WRITTEN
            
            // Update by key
            current_index++;
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
            char* calculation = parse_calculation_raw(user_inputs, 1);
            // Display answer on LED display
            // FUNCTION TO BE WRITTEN

            // Reset user_input array (call function to do so)
            clear_user_inputs(user_inputs, &current_index);

            new_key = 0;
            // Turn on debug light
            HAL_GPIO_WritePin(Light_bulb_GPIO_Port, Light_bulb_Pin, GPIO_PIN_SET);
        }
    }
}

char* parse_calculations_raw(char** inputs, int needs_compression) {
    // Compress all numbers into one string
    if (needs_compression) {
        compress(inputs);
    }

    int length = get_string_length(inputs);
    int found_open = 0;
    int open_index = 0;
    int close_index = 0;
    int tracker = 0;
    for (int index = 0; index < length; index++) {
        if (inputs[index][0] == '(') {
            tracker++;
            found_open = 1;
            open_index = index;
        }
        else if (inputs[index][0] == ')') {
            tracker--;
            close_index = index;
        }
        if (found_open && tracker == 0) {
            int inside_length = close_index - open_index - 1;
            char* inside[inside_length + 1]; // +1 ensures null terminator (\0)
            for (int i = 0; i < inside_length; i++) {
                inside[i] = inputs[open_index + i + 1];
            }
            char* inside_result = parse_calculations_raw(inside, 0);
            inputs[open_index] = inside_result;
            for (int i = 0; i < length - close_index; i++) {
                inputs[open_index + i + 1] = inputs[close_index + i + 1];
            }
            length = get_string_length(inputs);
            found_open = 0;
        }
    }
    return parse_calculations_bracketless(inputs);
}

char* parse_calculations_bracketless(char** inputs) {
    // Evaluate all trig expressions
    int length = get_string_length(inputs);
    char* eval_value = NULL;
    for (int index = 0; index < length; index++) {
        char value = inputs[index][0];
        if (value == 's') {
            eval_value = sine(inputs[index+3]);
        }
        else if (value == 'c') {
            eval_value = cosine(inputs[index+3]);
        }
        else if (value == 't') {
            eval_value = tangent(inputs[index+3]);
        }
        if (eval_value) {
            inputs[index] = eval_value;
            for (int i = 0; i < length - index - 3; i++) {
                inputs[index + i + 1] = inputs[index + i + 4];
            }
            eval_value = NULL;
        }
    }
    // At this point there should be no more trig expressions


}

void compress(char** inputs) {
    int length = get_string_length(inputs);
    int start_index = -1;
    int count = 0;
    for (int i = 0; i < length; i++) {
        char value = inputs[i][0];
        if ((value >= '0' && value <= '9') || value == '.') {
            if (start_index < 0) {
                start_index = i;
            }
            inputs[start_index][count] = value;
            count++;
        }
        else {
            if (start_index >= 0 && count > 0) {
                for (int i = start_index; i <= length; i++) {
                    inputs[i] = inputs[i + count];
                }
            }
        }
        length = get_string_length(inputs);
    }
}





int get_string_length(char** string) {
	int index = 0;
	while (string[index] != NULL) {
		index++;
	}
	return index;
}

char* sine(char* num) {

}

char* cosine(char* num) {

}

char* tangent(char* num) {

}

char* keypad_scan(void) {
	for (int row = 1; row < 5; row++) {
		HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(R3_GPIO_Port, R3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(R4_GPIO_Port, R4_Pin, GPIO_PIN_RESET);
		if (row == 1) {
			HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, GPIO_PIN_SET);
		}
		else if (row == 2) {
			HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, GPIO_PIN_SET);
		}
		else if (row == 3) {
			HAL_GPIO_WritePin(R3_GPIO_Port, R3_Pin, GPIO_PIN_SET);
		}
		else if (row == 4) {
			HAL_GPIO_WritePin(R4_GPIO_Port, R4_Pin, GPIO_PIN_SET);
		}

		for (int col = 1; col < 5; col++) {
			GPIO_TypeDef* col_port;
			uint16_t col_pin;
			if (col == 1) {
				col_port = C1_GPIO_Port;
				col_pin = C1_Pin;
			}
			else if (col == 2) {
				col_port = C2_GPIO_Port;
				col_pin = C2_Pin;
			}
			else if (col == 3) {
				col_port = C3_GPIO_Port;
				col_pin = C3_Pin;
			}
			else if (col == 4) {
				col_port = C4_GPIO_Port;
				col_pin = C4_Pin;
			}
			GPIO_PinState pin_state = HAL_GPIO_ReadPin(col_port, col_pin);
			if (pin_state == GPIO_PIN_SET) {
				return keypad[row - 1][col - 1];
			}
		}
	}
	return KEY_NOT_PRESSED;
}

void clear_user_inputs(char user_inputs[][100], int* ptr_index) {
	int index = 0;
	while (user_inputs[index][0] != '\0') {
		for (int inner = 0; inner < 100; inner++) {
			user_inputs[index][inner] = '\0';
		}
		index++;
	}
	*ptr_index = 0;
	printf("Cleared\n");
}