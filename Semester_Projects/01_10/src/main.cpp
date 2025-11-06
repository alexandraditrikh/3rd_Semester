#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>

#include "List.h" 

#define SIZE_DIGIT 5
#define MAX_SIZE_COND 20
#define MAX_SIZE_EXPR 20
#define MAX_IDENT_LENGTH 5
#define MAX_VARIABLES 10

using namespace std;

int calculate_expression(char* expr, int m, VariableNode*& top) {
    int result = 0;
    char operation = '+';
    int i = 0;

    while (expr[i] != '\0') {
        int val = 0;
        bool is_negative = false;
    
        if (expr[i] == '-' && (i == 0 || expr[i-1] == '+' || expr[i-1] == '-' || expr[i-1] == '*' || expr[i-1] == '/')) {
            is_negative = true;
            i++;
        }
    
        if (isdigit(expr[i])) {
            while (isdigit(expr[i]) && expr[i] != '\0') {
                val = val * 10 + (expr[i] - '0');
                i++;
            }
        } 
       
        else if (isalpha(expr[i])) {
            char current_ident[MAX_IDENT_LENGTH + 1] = {0};
            int ident_idx = 0;
            
            while ((isalpha(expr[i]) || isdigit(expr[i])) && expr[i] != '\0' && ident_idx < m) {
                current_ident[ident_idx++] = expr[i];
                i++;
            }
            current_ident[ident_idx] = '\0';
            
            val = get_variable_value(top, current_ident, m);
            if (ident_idx > m) { 
                cerr << "Err: Identifier name too long in expression\n";
                return 0; 
            }
        } else {
            if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') {
                operation = expr[i];
                i++;
                continue;
            } else {
                cerr << "Err: Unexpected symbol in expression: " << expr[i] << " at index " << i << endl;
                break;
            }
        }

        if (is_negative) {
            val = -val;
        }

        switch (operation) {
            case '+': result += val; break;
            case '-': result -= val; break;
            case '*': result *= val; break;
            case '/': 
                if (val != 0) {
                    result /= val; 
                } else {
                    cerr << "Err: Division by zero\n";
                    return 0;
                }
                break;
            default: break; 
        }

        while (expr[i] != '\0' && (isspace(expr[i]))) { 
            i++;
        }
        
        if (expr[i] != '\0') {
            operation = expr[i];
            i++;
        }
    }
    return result;
}

bool calculate_condition(char* cond, int m, VariableNode*& top) {
    char left[MAX_SIZE_EXPR] = {0};
    char right[MAX_SIZE_EXPR] = {0};
    char op_str[3] = {0};
    
    int i = 0, op_idx = 0;
    bool operator_found = false;

    while (cond[i] != '\0') {
        if (cond[i] == '<' || cond[i] == '>' || cond[i] == '=') {
            operator_found = true;
            break;
        }
        left[i] = cond[i];
        i++;
    }
    
    if (operator_found) {
        op_str[op_idx++] = cond[i++];
        if ((op_str[0] == '<' || op_str[0] == '>') && cond[i] == '=') {
            op_str[op_idx++] = cond[i++];
        } else if (op_str[0] == '=' && cond[i] == '=') {
            op_str[op_idx++] = cond[i++];
        }
    }

    int right_start = i;
    int right_idx = 0;
    while (cond[i] != '\0') {
        right[right_idx++] = cond[i];
        i++;
    }

    if (op_idx == 0 || left[0] == '\0' || right[0] == '\0') {
        return false; 
    }

    int left_val = calculate_expression(left, m, top);
    int right_val = calculate_expression(right, m, top);

    if (strcmp(op_str, "==") == 0) return left_val == right_val;
    if (strcmp(op_str, "<=") == 0) return left_val <= right_val;
    if (strcmp(op_str, ">=") == 0) return left_val >= right_val;
    if (strcmp(op_str, "<") == 0) return left_val < right_val;
    if (strcmp(op_str, ">") == 0) return left_val > right_val;
    
    return false;
}

bool task(ifstream& in, int m, char* ident, bool& condition_val, int& expr1_value, int& expr2_value, VariableNode*& top) {
    char c;

    while (in.peek() != EOF && isspace(in.peek())) {
        in.get();
    }
    if (in.peek() == EOF) {
        // Достигли конца файла, но это не ошибка, просто больше нет выражений
        return true; 
    }

    int s = 0; // Начальное состояние
    int count = 0;
    int count_dig = 0;

    int ident_idx = 0;
    int condition_idx = 0;
    int expr1_idx = 0;
    int expr2_idx = 0;

    char condition_str[MAX_SIZE_COND] = {0};
    char expr1_str[MAX_SIZE_EXPR] = {0};
    char expr2_str[MAX_SIZE_EXPR] = {0};

    char current_op = '\0';
    int err_case = 0;
    
    while (in.get(c)) {
        if (s > 0 && isspace(c)) continue;

        switch (s) {
        case 0: {
            if (isalpha(c)) {
                ident[ident_idx++] = c;
                s = 1;
                count = 1;
            } else { 
                err_case = 0;
                s = 100;
            }
            break;
        }
        case 1: {
            if (isalpha(c) || isdigit(c)) {
                ident[ident_idx++] = c;
                count++; 
                if (count > m) { 
                    err_case = 1;
                    s = 100;
                }
            } 
            else if (c == '=') {
                ident[ident_idx] = '\0';
                s = 2; 
            } 
            else { 
                err_case = 1;
                s = 100;
            }
            break;
        }
        case 2: { 
            condition_str[condition_idx++] = c;
            if (c == '+' || c == '-') {
                s = 20; 
            } else if (isdigit(c)) {
                count_dig = 1;
                s = 3;
            } else if (isalpha(c)) {
                count = 1;
                s = 4;
            } else { 
                err_case = 2;
                s = 100;
            }
            break;
        }
        case 20: {
            condition_str[condition_idx++] = c; 
            if (isdigit(c)) {
                count_dig = 1;
                s = 3;
            } else { 
                err_case = 20;
                s = 100;
            }
            break;
        }
        case 3: {
            if (isdigit(c)) {
                condition_str[condition_idx++] = c;
                count_dig++; 
                if (count_dig > SIZE_DIGIT) { 
                    err_case = 3;
                    s = 100;
                }
            } else {
                if (count_dig == 0 || (SIZE_DIGIT > 0 && count_dig != SIZE_DIGIT)) { 
                    err_case = 3; s = 100; break;
                }
                in.unget();
                while (in.peek() != EOF && isspace(in.peek())) {
                    in.get();
                }
                if (!in.get(c)) { err_case = 3; s = 100; break; }

                if (c == '>' || c == '<' || c == '=') {
                    if (in.peek() == '=') {
                        condition_str[condition_idx++] = c;
                        c = in.get(); 
                        condition_str[condition_idx++] = c;

                        s = 6;
                        break;
                    } else {
                        condition_str[condition_idx++] = c;

                        s = 6; 
                        break;
                    }
                }
                else { 
                    err_case = 3;
                    s = 100;
                } 
            }
            break;
        }
        case 4: {
        if (c == '>' || c == '<' || c == '=') {
            if (in.peek() == '=') {
                condition_str[condition_idx++] = c;
                c = in.get(); 
                condition_str[condition_idx++] = c;

                s = 6;
                break;
            } else {
                condition_str[condition_idx++] = c;

                s = 6; 
                break;
            }
        } 
        else if (isalpha(c) || isdigit(c)) {
            condition_str[condition_idx++] = c;
            count++; 
            if (count > m) { 
                err_case = 4;
                s = 100;
            }
            break;
        }
        else { 
            err_case = 4;
            s = 100;
            break;
        }
        }
        case 6: {
            condition_str[condition_idx++] = c;
            if (c == '+' || c == '-') {
                s = 21; 
            } else if (isdigit(c)) {
                count_dig = 1;
                s = 7; 
            } else if (isalpha(c)) {
                count = 1;
                s = 8; 
            } else { 
                err_case = 6;
                s = 100;
            }
            break;
        }
        case 21: { 
            condition_str[condition_idx++] = c;
            if (isdigit(c)) {
                count_dig = 1;
                s = 7; 
            } else { 
                err_case = 21;
                s = 100;
            }
            break;
        }
        case 7: { 
            if (isdigit(c)) {
                condition_str[condition_idx++] = c;
                count_dig++; 
                if (count_dig > SIZE_DIGIT) { 
                    err_case = 7;
                    s = 100;
                }
            } else if (c == '?') {
                if (count_dig != SIZE_DIGIT) { 
                    err_case = 7;
                    s = 100;
                    break;
                } 
                condition_str[condition_idx] = '\0';
                condition_val = calculate_condition(condition_str, m, top);
                s = 9; 
            } else { 
                err_case = 7;
                s = 100;
            }
            break;
        }
        case 8: { 
            if (isalpha(c) || isdigit(c)) {
                condition_str[condition_idx++] = c;
                count++; 
                if (count > m) { 
                    err_case = 8;
                    s = 100; 
                }
            } else if (c == '?') {
                condition_str[condition_idx] = '\0';
                condition_val = calculate_condition(condition_str, m, top);
                s = 9; 
            } else { 
                err_case = 8;
                s = 100;
            }
            break;
        }
        case 9: { 
            expr1_str[expr1_idx++] = c;
            if (c == '+' || c == '-') {
                current_op = c;
                s = 22; 
            } else if (isdigit(c)) {
                count_dig = 1; 
                s = 10; 
            } else if (isalpha(c)) {
                count = 1;
                s = 11; 
            } else { 
                err_case = 9;
                s = 100;
            }
            break;
        }
        case 22: { 
            if (isdigit(c)) {
                expr1_str[expr1_idx++] = c;
                count_dig = 1;
                s = 10; 
            }
            else { 
                err_case = 22;
                s = 100; 
            }
            break;
        }
        case 10: { 
            if (isdigit(c)) {
                expr1_str[expr1_idx++] = c;
                count_dig++; 
                if (count_dig > SIZE_DIGIT) { 
                    err_case = 10;
                    s = 100;
                }
            } 
            else { 
                if (count_dig != SIZE_DIGIT) { 
                    err_case = 10;
                    s = 100;
                    break;
                }
                
                if (c == '+' || c == '-' || c == '*' || c == '/'){
                    expr1_str[expr1_idx++] = c;
                    current_op = c;
                    s = 12; 
                } 
                else if (c == ':') {
                    expr1_str[expr1_idx] = '\0';
                    expr1_value = calculate_expression(expr1_str, m, top);
                    s = 15; 
                }
                else { 
                    err_case = 10;
                    s = 100;
                }
            }
            break;
        }
        case 11: { 
            if (isalpha(c) || isdigit(c)) {
                expr1_str[expr1_idx++] = c;
                count++; 
                if (count > m) { 
                    err_case = 11;
                    s = 100; 
                }
            } else if (c == '+' || c == '-' || c == '*' || c == '/') {
                expr1_str[expr1_idx++] = c;
                current_op = c;
                s = 12; 
            } else if (c == ':') {
                expr1_str[expr1_idx] = '\0';
                expr1_value = calculate_expression(expr1_str, m, top);
                s = 15; 
            } else { 
                err_case = 11;
                s = 100;
            }
            break;
        }
        case 12: {
            expr1_str[expr1_idx++] = c;
            if (c == '+' || c == '-') {
                current_op = c;
                s = 23; 
            }
            else if (isdigit(c)) {
                count_dig = 1; 
                s = 13; 
            } 
            else if (isalpha(c)) {
                count = 1; 
                s = 14; 
            } 
            else { 
                err_case = 12;
                s = 100;
            }
            break;
        }
        case 23: { 
            if (isdigit(c)) {
                expr1_str[expr1_idx++] = c;
                count_dig = 1;
                s = 13; 
            } 
            else { 
                err_case = 23;
                s = 100;
            }
            break;
        }
        case 13: { 
            if (isdigit(c)) {
                expr1_str[expr1_idx++] = c;
                count_dig++; 
                if (count_dig > SIZE_DIGIT) { 
                    err_case = 13;
                    s = 100;
                }
            }
            else if (c == ':') {
                if (count_dig != SIZE_DIGIT) { 
                    err_case = 13;
                    s = 100;
                    break;
                } 
                expr1_str[expr1_idx] = '\0';
                expr1_value = calculate_expression(expr1_str, m, top);
                s = 15; 
            }
            else { 
                err_case = 13;
                s = 100;
            }
            break; 
        }
        case 14: { 
            if (isalpha(c) || isdigit(c)) {
                expr1_str[expr1_idx++] = c;
                count++; 
                if (count > m) { 
                    err_case = 14;
                    s = 100;
                }
            }
            else if (c == ':') {
                expr1_str[expr1_idx] = '\0';
                expr1_value = calculate_expression(expr1_str, m, top);
                s = 15; 
            }
            else { 
                err_case = 14;
                s = 100;
            }
            break;
        } 
        case 15: { 
            expr2_str[expr2_idx++] = c;
            if (c == '+' || c == '-') {
                current_op = c;
                s = 24; 
            } else if (isdigit(c)) { 
                count_dig = 1; 
                s = 16; 
            } else if (isalpha(c)) {
                count = 1; 
                s = 17; 
            } else { 
                err_case = 15;
                s = 100;
            }
            break;
        }
        case 24: {
            if (isdigit(c)) {
                expr2_str[expr2_idx++] = c;
                count_dig = 1;
                s = 16; 
            } else { 
                err_case = 24;
                s = 100;
            }
            break;
        }
        case 16: { 
            if (isdigit(c)) {
                expr2_str[expr2_idx++] = c;
                count_dig++; 
                if (count_dig > SIZE_DIGIT) { 
                    err_case = 16;
                    s = 100;
                }
            }
            else { 
                if (count_dig != SIZE_DIGIT) { 
                    err_case = 16;
                    s = 100;
                    break;
                }
                
                if (c == '+' || c == '-' || c == '*' || c == '/') {
                    expr2_str[expr2_idx++] = c;
                    current_op = c;
                    s = 18; 
                }
                else if (c == ';') {
                    expr2_str[expr2_idx] = '\0';
                    expr2_value = calculate_expression(expr2_str, m, top);
                    return true;
                }
                else { 
                    err_case = 16;
                    s = 100;
                }
            }
            
            break;
        }
        case 17: { 
            if (isalpha(c) || isdigit(c)) {
                expr2_str[expr2_idx++] = c;
                count++; 
                if (count > m) { 
                    err_case = 17;
                    s = 100;
                }
            }
            else if (c == '+' || c == '-' || c == '*' || c == '/') {
                expr2_str[expr2_idx++] = c;
                current_op = c;
                s = 18; 
            }
            else if (c == ';') {
                expr2_str[expr2_idx] = '\0';
                expr2_value = calculate_expression(expr2_str, m, top);
                return true;
            }
            else { 
                err_case = 17;
                s = 100;
            }
            break;
        }
        case 18: { 
            expr2_str[expr2_idx++] = c;
            if (c == '+' || c == '-') {
                current_op = c;
                s = 26; 
            }
            else if (isdigit(c)) {
                count_dig = 1; 
                s = 19; 
            } 
            else if (isalpha(c)) {
                count = 1; 
                s = 27; 
            } 
            else { 
                err_case = 18;
                s = 100;
            }
            break;
        }
        case 26: { 
            if (isdigit(c)) {
                expr2_str[expr2_idx++] = c;
                count_dig = 1;
                s = 19; 
            } 
            else { 
                err_case = 26;
                s = 100;
            }
            break;
        }
        case 19: { 
            if (isdigit(c)) {
                expr2_str[expr2_idx++] = c;
                count_dig++; 
                if (count_dig > SIZE_DIGIT) { 
                    err_case = 19;
                    s = 100;
                }
            }
            else if (c == ';') {
                if (count_dig != SIZE_DIGIT) { 
                    err_case = 19;
                    s = 100;
                    break;
                } 
                expr2_str[expr2_idx] = '\0';
                expr2_value = calculate_expression(expr2_str, m, top);
                return true;
            }
            else { 
                err_case = 19;
                s = 100;
            }
            break;
        }
        case 27: {
            if (isalpha(c) || isdigit(c)) {
                expr2_str[expr2_idx++] = c;
                count++; 
                if (count > m) { 
                    err_case = 27;
                    s = 100;
                }
            }
            else if (c == ';') {
                expr2_str[expr2_idx] = '\0';
                expr2_value = calculate_expression(expr2_str, m, top);
                return true;
            }
            else { 
                err_case = 27;
                s = 100;
            }
            break;
        }

        case 100: {
            cout << "false in " << err_case << endl;
            return false;
        }
        default: {
            cout << "false in default " << s << endl;
            return false;
        }

        }
    }
    return s == 0;
}

int main() {
    int m;
    cout << "Enter m (2-4): \n";
    if (!(cin >> m)) {
        cerr << "Err: m is not entered\n";
        return 1;
    }
    if (m < 2 || m > 4) {
        cerr << "Err: m must be 2 or 3 or 4\n";
        return 1;
    }

    ifstream in("operator.txt"); 
    if (!in.is_open()) {
        cerr << "Err: open input file (operator.txt)\n" << endl;
        return 1;
    }

    char ident[MAX_IDENT_LENGTH + 1] = {0}; 
    bool condition_val = false;
    int expr1_value = 0;
    int expr2_value = 0;

    VariableNode* top = nullptr; 

    bool isValid = true;
    while (isValid && in.peek() != EOF) {
        memset(ident, 0, MAX_IDENT_LENGTH + 1);
        condition_val = false;
        expr1_value = 0;
        expr2_value = 0;

        if (task(in, m, ident, condition_val, expr1_value, expr2_value, top)) {

            int final_result = condition_val ? expr1_value : expr2_value;
            set_variable_value(top, ident, final_result, m);
        } else {
            isValid = false;
        }
    }

    cout << (isValid ? "All expressions are valid\n\n" : "Expression is not valid\n");

    if (isValid) {
        VariableNode* current = top;
        if (current == nullptr) {
            cout << "No variables assigned." << endl;
        } else {
            while(current != nullptr) {
                cout << current->name << " = " << current->value << endl;
                current = current->next;
            }
        }
    }

    in.close();
    clear_variable_list(top);

    return 0;
}