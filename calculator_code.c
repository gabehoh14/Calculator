#include <cstdio>
#include <cstdint>
#include <cmath>

// define
double PI = 3.1415926535898793238462643383279502884197169399375;
int DEBUGGING = 1;

int main();
double calculate(char test[100], double expected);
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



char keypad[4][4][100] = {
    {"7", "8", "9", "/"},  // Row 1
    {"4", "5", "6", "*"},  // Row 2
    {"1", "2", "3", "-"},  // Row 3
    {"0", ".", "=", "+"}   // Row 4
};

int main() {
    // More complicated expressions with mixed unary operators:
    // char test1[100]  = "2+3*4";                               calculate(test1, 14.0);
    // char test2[100]  = "log(15-2^2+sin(10-2*45))/12+3^0.5";   calculate(test2, 1.815439072);
    // char test3[100]  = "sin(0)+cos(0)";                       calculate(test3, 1.0);
    // char test4[100]  = "5*(2+3)^2";                           calculate(test4, 125.0);
    // char test5[100]  = "16^.5+log(1)";                        calculate(test5, 4.0);  // if sqrt supported
    // char test6[100]  = "3+4*2/(1-5)^2";                       calculate(test6, 3.5);
    // char test7[100]  = "tan(0)";                              calculate(test7, 0.0);
    // char test8[100]  = "log(1)";                              calculate(test8, 0.0);
    // char test9[100]  = "-3+4";                                calculate(test9, 1.0);
    // char test10[100] = "2^(3^2)";                             calculate(test10, 512.0); // right-associative
    // char test11[100] = "-(3+2)*4";                            calculate(test11, -20.0);
    // char test12[100] = "+--(4-2)*(6+1)";                      calculate(test12, 14.0);
    // char test13[100] = "sin(180/2)";                          calculate(test13, 1.0);
    // char test14[100] = "cos(180)";                            calculate(test14, -1.0);
    // char test15[100] = "tan(180)";                            calculate(test15, 0.0);
    // char test16[100] = "log(10)";                             calculate(test16, 1.0);  // if e constant supported
    // char test17[100] = "1/(2+3)*4";                           calculate(test17, 0.8);
    // char test18[100] = "3^(-2)";                              calculate(test18, 0.111111111); // 1/9
    // char test19[100] = "-sin(-180/2)";                        calculate(test19, 1.0);
    // char test20[100] = "(2+3)*(4+5)";                         calculate(test20, 45.0);
    // char test21[100] = "sin(30) + cos(60)";                   calculate(test21, 1.0);              // 0.5 + 0.5
    // char test22[100] = "log(1000)";                           calculate(test22, 3.0);              // base 10
    // char test23[100] = "2^3^1";                               calculate(test23, 8.0);              // 2^(3)
    // char test24[100] = "2^(3^0)";                             calculate(test24, 2.0);              // 2^1
    // char test25[100] = "sin(90)^2+cos(90)^2";               calculate(test25, 1.0);              // identity
    // char test26[100] = "3+4*-2";                              calculate(test26, -5.0);             // test unary minus
    // char test27[100] = "-3^2";                                calculate(test27, -9.0);             // unary minus before power
    // char test28[100] = "(-3)^2";                              calculate(test28, 9.0);              // parentheses override precedence
    // char test29[100] = "-(-(-1))";                            calculate(test29, -1.0);             // triple negation
    char test30[100] = "2^2^2^0";                             calculate(test30, 16.0);             // 2^(2^(2^0)) = 2^2 = 4, 2^4 = 16
    // char test31[100] = "log(1+9)";                            calculate(test31, 1.0);              // log(10)
    // char test32[100] = "cos(0)*sin(90)";                      calculate(test32, 1.0);              // 1*1
    // char test33[100] = "tan(45)+tan(135)";                    calculate(test33, 0.0);              // 1 + (-1)
    // char test34[100] = "log(10^2)";                           calculate(test34, 2.0);              // log(100)
    // char test35[100] = "2^(-2)";                              calculate(test35, 0.25);             // 1 / (2^2)
    // char test36[100] = "cos(60)^2-sin(60)^2";                 calculate(test36, -0.5);             // trig identity
    // char test37[100] = "3^0+4^0+5^0";                         calculate(test37, 3.0);              // 1+1+1
    // char test38[100] = "log(0.01)";                           calculate(test38, -2.0);             // log(1/100)
    // char test39[100] = "2^(1+1)*2";                           calculate(test39, 8.0);              // (2^2)*2 = 4*2
    // char test40[100] = "sin(720)";                            calculate(test40, 0.0);              // 720° = 2 full rotations
    // char test41[100] = "log(100+sin(60)*2^3)/(3+cos(90))";    calculate(test41, 0.67636408973); // ≈ log(100 + 0.866*8)/3 ≈ log(106.93)/3 ≈ 2.9138/3
    // char test42[100] = "3^(sin(30+60)/2)+4^(cos(60)^2)";      calculate(test42, 3.14626436994); // 3^(1/2) + 4^0.25 ≈ 1.732 + 1.414
    // char test43[100] = "log(25+cos(180)/tan(45))+sin(90)^2";  calculate(test43, 2.38021124171); // log(25 - 1/1) + 1 = log(24) + 1 ≈ 1.3802 + 1
    
    // char test44[100] = "2^(sin(30+15) * cos(45)) + log(1000)/3";    
    // calculate(test44, 3.106); // ≈ 2^(0.707*0.707) + 3 ≈ 2^0.5 + 1 ≈ 1.414 + 1
    
    // char test45[100] = "((2+3)^2 - sin(90)*10) / log(100)";           
    // calculate(test45, 2.5); // (25 - 10)/2 = 15/2
    
    // char test46[100] = "tan(45)^2 + cos(60)^2 + sin(30)^2";           
    // calculate(test46, 1.5); // 1^2 + 0.5^2 + 0.5^2 = 1 + 0.25 + 0.25
    
    // char test47[100] = "log(50 + 5*sin(45)^2) - 2^(-1)";              
    // calculate(test47, 1.678); // log(50 + 5*0.5) - 0.5 = log(52.5) - 0.5 ≈ 1.72 - 0.5
    
    // char test48[100] = "-(2^2 + log(100)) + sin(180)^2";              
    // calculate(test48, -6.0); // -4 - 2 + 0 = -6
    
    // char test49[100] = "(3 + sin(60))^(cos(30))";                     
    // calculate(test49, 3.6244); // ≈ (3 + 0.866)^0.866
    
    // char test50[100] = "log(2^3 + sin(90)*cos(0)) / (tan(45)^2 + 1)"; 
    // calculate(test50, 0.4771); // log(8 + 1)/2 = log(9)/2 ≈ 0.9542/2

    return 0;
}

// Takes in string input parameter, calculates and returns result
double calculate(char test[100], double expected) {
    // ----------- DEBUG --------------
    if (DEBUGGING) {
        printf("Calculating %s\n", test);
    }
    // ----------- DEBUG --------------

    // 2D array filled with string characters
    char user_inputs[75][100] = {};
    int test_length = get_string_length(test);
    user_inputs[0][0] = '0';
    user_inputs[1][0] = '+';
    for (int i = 0; i < test_length; i++) {
        user_inputs[i+2][0] = test[i];
    }

    // Calculatation performed and returns final result
    parse_calculations_raw(user_inputs);
    double number = arr_to_num(user_inputs[0]);
    if (check(number, expected)) {
        printf("Properly calculated %s = %f\n", test, number);
    }
    else {
        printf("Unexpectedly calculated %s = %f, expected %f\n", test, number, expected);
    }
    return number;
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
            char inside[inside_length + 1][100] = {}; // +1 ensures null terminator
            for (int i = 0; i < inside_length; i++) {
                copy_array(inside[i], inputs[open_index + i + 1]);
            }
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
        double decimal = 0;
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