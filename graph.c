#include <stdio.h>
#include <stdlib.h>

#define WIDTH 80
#define HEIGHT 25
#define X_MIN 0
#define X_MAX (4 * 3.14159265358979323846)  // 4π
#define Y_MIN -1
#define Y_MAX 1
#define ORIGIN_X 0
#define ORIGIN_Y 12
#define PI 3.14159265358979323846
#define EPSILON 1e-10

typedef enum {
    NUMBER,
    VARIABLE,
    OPERATOR,
    FUNCTION,
    LEFT_PAREN,
    RIGHT_PAREN,
    END
} TokenType;

typedef struct {
    TokenType type;
    char value[20];
    double number;
} Token;

typedef struct StackNode {
    Token token;
    struct StackNode* next;
} StackNode;

typedef struct {
    StackNode* top;
} Stack;

// Function prototypes
int IsEmpty(const Stack* stack);
Token* ShuntingYard(const Token* tokens);
double EvaluateRPN(const Token* rpn, double x);
void InitStack(Stack* stack);
void Push(Stack* stack, Token token);
Token Pop(Stack* stack);
Token Peek(Stack* stack);
void FreeStack(Stack* stack);
int IsDigitOrDot(char c);
int IsAlpha(char c);
int GetPrecedence(const char* op);
int IsRightAssociative(const char* op);
int IsFunction(const char* token);
Token* Tokenize(const char* expression);
void PlotGraph(const char* expression);
int MyStrlen(const char* str);
int MyStrcmp(const char* s1, const char* s2);
void MyStrcpy(char* dest, const char* src);
double MyAtof(const char* str);
double MySin(double x);
double MyCos(double x);
double MyTan(double x);
double MySqrt(double x);
double MyLog(double x);
int MyRound(double x);
double MyPow(double base, double exp);
double MyFabs(double x);
double MyFactorial(int n);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s \"expression\"\n", argv[0]);
        return 1;
    }

    PlotGraph(argv[1]);
    return 0;
}

int MyStrlen(const char* str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

int MyStrcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void MyStrcpy(char* dest, const char* src) {
    while ((*dest++ = *src++)) {
    }
}

double MyFabs(double x) {
    return x < 0 ? -x : x;
}

#if 0
double MyFactorial(int n) {
    double result = 1.0;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}
#endif

double MyPow(double base, double exp) {
    if (exp == (int)exp) {
        int int_exp = (int)exp;
        double result = 1.0;
        
        if (int_exp < 0) {
            base = 1.0 / base;
            int_exp = -int_exp;
        }
        
        while (int_exp > 0) {
            if (int_exp % 2 == 1) {
                result *= base;
            }
            base *= base;
            int_exp /= 2;
        }
        
        return result;
    }
    
    if (base <= 0) {
        return 0.0;
    }
    
    return MySin(exp * MyLog(base));
}

double MyAtof(const char* str) {
    double result = 0.0;
    double fraction = 0.0;
    double factor = 0.1;
    int sign = 1;
    int i = 0;

    if (str[i] == '-') {
        sign = -1;
        i++;
    } else if (str[i] == '+') {
        i++;
    }

    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10.0 + (str[i] - '0');
        i++;
    }

    if (str[i] == '.') {
        i++;
        while (str[i] >= '0' && str[i] <= '9') {
            fraction += (str[i] - '0') * factor;
            factor *= 0.1;
            i++;
        }
    }

    return sign * (result + fraction);
}

double MySin(double x) {
    while (x > PI) {
        x -= 2 * PI;
    }
    while (x < -PI) {
        x += 2 * PI;
    }

    double result = 0.0;
    double term = x;
    int sign = 1;
    
    for (int i = 1; i <= 15; i += 2) {
        result += sign * term;
        sign = -sign;
        term = term * x * x / ((i + 1) * (i + 2));
    }

    return result;
}

double MyCos(double x) {
    while (x > PI) {
        x -= 2 * PI;
    }
    while (x < -PI) {
        x += 2 * PI;
    }

    double result = 0.0;
    double term = 1.0;
    int sign = 1;
    
    for (int i = 0; i <= 14; i += 2) {
        result += sign * term;
        sign = -sign;
        term = term * x * x / ((i + 1) * (i + 2));
    }

    return result;
}

double MyTan(double x) {
    double cos_val = MyCos(x);
    if (MyFabs(cos_val) < EPSILON) {
        return 1e10;
    }
    return MySin(x) / cos_val;
}

double MySqrt(double x) {
    if (x < 0) {
        return (0.0 * 1.0) / (0.0 + 0.0);
    }
    if (x == 0) {
        return 0;
    }

    double guess = x / 2.0;
    
    for (int i = 0; i < 20; i++) {
        double prev_guess = guess;
        guess = (guess + x / guess) / 2.0;
        if (MyFabs(guess - prev_guess) < EPSILON) {
            break;
        }
    }

    return guess;
}

double MyLog(double x) {
    if (x <= 0) {
        return (0.0 * 1.0) / (0.0 + 0.0);
    }

    int exponent = 0;
    while (x > 1.5) {
        x /= 2.0;
        exponent++;
    }
    while (x < 0.5) {
        x *= 2.0;
        exponent--;
    }

    double y = x - 1.0;
    double result = 0.0;
    double term = y;
    double y_power = y;
    
    for (int i = 1; i <= 30; i++) {
        result += term / i;
        y_power *= -y;
        term = y_power;
    }

    return result + exponent * 0.693147180559945;
}

int MyRound(double x) {
    return (int)(x + (x >= 0 ? 0.5 : -0.5));
}

void InitStack(Stack* stack) {
    stack->top = NULL;
}

int IsEmpty(const Stack* stack) {
    return stack->top == NULL;
}

void Push(Stack* stack, Token token) {
    StackNode* new_node = (StackNode*)malloc(sizeof(StackNode));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    new_node->token = token;
    new_node->next = stack->top;
    stack->top = new_node;
}

Token Pop(Stack* stack) {
    if (IsEmpty(stack)) {
        Token empty = {END, "", 0};
        return empty;
    }
    StackNode* temp = stack->top;
    Token token = temp->token;
    stack->top = temp->next;
    free(temp);
    return token;
}

Token Peek(Stack* stack) {
    if (IsEmpty(stack)) {
        Token empty = {END, "", 0};
        return empty;
    }
    return stack->top->token;
}

void FreeStack(Stack* stack) {
    while (!IsEmpty(stack)) {
        Pop(stack);
    }
}

int IsDigitOrDot(char c) {
    return (c >= '0' && c <= '9') || c == '.';
}

int IsAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int GetPrecedence(const char* op) {
    if (MyStrcmp(op, "+") == 0 || MyStrcmp(op, "-") == 0) {
        return 1;
    } else if (MyStrcmp(op, "*") == 0 || MyStrcmp(op, "/") == 0) {
        return 2;
    } else if (MyStrcmp(op, "^") == 0) {
        return 3;
    } else if (MyStrcmp(op, "u-") == 0) {
        return 4;
    }
    return 0;
}

int IsRightAssociative(const char* op) {
    return MyStrcmp(op, "u-") == 0 || MyStrcmp(op, "^") == 0;
}

int IsFunction(const char* token) {
    return MyStrcmp(token, "sin") == 0 || MyStrcmp(token, "cos") == 0 || MyStrcmp(token, "tan") == 0 ||
           MyStrcmp(token, "ctg") == 0 || MyStrcmp(token, "sqrt") == 0 || MyStrcmp(token, "ln") == 0;
}

Token* Tokenize(const char* expression) {
    int len = MyStrlen(expression);
    Token* tokens = (Token*)malloc((len * 2 + 1) * sizeof(Token));
    if (tokens == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    int i = 0;
    int j = 0;
    int last_was_operator_or_paren = 1;

    while (i < len) {
        if (expression[i] == ' ' || expression[i] == '\t') {
            i++;
            continue;
        }

        if (IsDigitOrDot(expression[i])) {
            tokens[j].type = NUMBER;
            int k = 0;
            while (i < len && IsDigitOrDot(expression[i])) {
                tokens[j].value[k++] = expression[i++];
            }
            tokens[j].value[k] = '\0';
            tokens[j].number = MyAtof(tokens[j].value);
            last_was_operator_or_paren = 0;
            j++;
            continue;
        }

        if (expression[i] == 'x') {
            tokens[j].type = VARIABLE;
            MyStrcpy(tokens[j].value, "x");
            last_was_operator_or_paren = 0;
            i++;
            j++;
            continue;
        }

        if (IsAlpha(expression[i])) {
            int k = 0;
            while (i < len && IsAlpha(expression[i])) {
                tokens[j].value[k++] = expression[i++];
            }
            tokens[j].value[k] = '\0';

            if (IsFunction(tokens[j].value)) {
                tokens[j].type = FUNCTION;
            } else {
                fprintf(stderr, "Unknown function or variable: %s\n", tokens[j].value);
                free(tokens);
                exit(1);
            }
            last_was_operator_or_paren = 1;
            j++;
            continue;
        }

        if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || 
            expression[i] == '/' || expression[i] == '^') {
            tokens[j].type = OPERATOR;

            if (expression[i] == '-' && last_was_operator_or_paren) {
                MyStrcpy(tokens[j].value, "u-");
            } else {
                tokens[j].value[0] = expression[i];
                tokens[j].value[1] = '\0';
            }

            last_was_operator_or_paren = 1;
            i++;
            j++;
            continue;
        }

        if (expression[i] == '(') {
            tokens[j].type = LEFT_PAREN;
            MyStrcpy(tokens[j].value, "(");
            last_was_operator_or_paren = 1;
            i++;
            j++;
            continue;
        }

        if (expression[i] == ')') {
            tokens[j].type = RIGHT_PAREN;
            MyStrcpy(tokens[j].value, ")");
            last_was_operator_or_paren = 0;
            i++;
            j++;
            continue;
        }

        fprintf(stderr, "Unknown character: %c\n", expression[i]);
        free(tokens);
        exit(1);
    }

    tokens[j].type = END;
    MyStrcpy(tokens[j].value, "");
    tokens[j].number = 0;

    return tokens;
}

Token* ShuntingYard(const Token* tokens) {
    int size = 0;
    for (size = 0; tokens[size].type != END; size++) {
    }

    Token* output = (Token*)malloc((size + 1) * sizeof(Token));
    if (output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    Stack operator_stack;
    InitStack(&operator_stack);

    int i = 0;
    int j = 0;

    while (tokens[i].type != END) {
        switch (tokens[i].type) {
            case NUMBER:
            case VARIABLE:
                output[j++] = tokens[i];
                break;

            case FUNCTION:
                Push(&operator_stack, tokens[i]);
                break;

            case OPERATOR:
                while (!IsEmpty(&operator_stack) && Peek(&operator_stack).type != LEFT_PAREN &&
                       (GetPrecedence(Peek(&operator_stack).value) > GetPrecedence(tokens[i].value) ||
                        (GetPrecedence(Peek(&operator_stack).value) == GetPrecedence(tokens[i].value) &&
                         !IsRightAssociative(tokens[i].value)))) {
                    output[j++] = Pop(&operator_stack);
                }
                Push(&operator_stack, tokens[i]);
                break;

            case LEFT_PAREN:
                Push(&operator_stack, tokens[i]);
                break;

            case RIGHT_PAREN:
                while (!IsEmpty(&operator_stack) && Peek(&operator_stack).type != LEFT_PAREN) {
                    output[j++] = Pop(&operator_stack);
                }

                if (IsEmpty(&operator_stack) || Peek(&operator_stack).type != LEFT_PAREN) {
                    fprintf(stderr, "Mismatched parentheses\n");
                    free(output);
                    FreeStack(&operator_stack);
                    exit(1);
                }

                Pop(&operator_stack);

                if (!IsEmpty(&operator_stack) && Peek(&operator_stack).type == FUNCTION) {
                    output[j++] = Pop(&operator_stack);
                }
                break;

            default:
                break;
        }
        i++;
    }

    while (!IsEmpty(&operator_stack)) {
        if (Peek(&operator_stack).type == LEFT_PAREN) {
            fprintf(stderr, "Mismatched parentheses\n");
            free(output);
            FreeStack(&operator_stack);
            exit(1);
        }
        output[j++] = Pop(&operator_stack);
    }

    output[j].type = END;
    MyStrcpy(output[j].value, "");

    FreeStack(&operator_stack);
    return output;
}

double EvaluateRPN(const Token* rpn, double x) {
    Stack stack;
    InitStack(&stack);

    for (int i = 0; rpn[i].type != END; i++) {
        switch (rpn[i].type) {
            case NUMBER:
                Push(&stack, rpn[i]);
                break;

            case VARIABLE: {
                Token x_token = {NUMBER, "", x};
                Push(&stack, x_token);
            } break;

            case OPERATOR: {
                if (MyStrcmp(rpn[i].value, "u-") != 0) {
                    if (IsEmpty(&stack)) {
                        fprintf(stderr, "Invalid expression: not enough operands\n");
                        FreeStack(&stack);
                        exit(1);
                    }
                    Token b = Pop(&stack);

                    if (IsEmpty(&stack)) {
                        fprintf(stderr, "Invalid expression: not enough operands\n");
                        FreeStack(&stack);
                        exit(1);
                    }
                    Token a = Pop(&stack);

                    Token result = {NUMBER, "", 0};

                    if (MyStrcmp(rpn[i].value, "+") == 0) {
                        result.number = a.number + b.number;
                    } else if (MyStrcmp(rpn[i].value, "-") == 0) {
                        result.number = a.number - b.number;
                    } else if (MyStrcmp(rpn[i].value, "*") == 0) {
                        result.number = a.number * b.number;
                    } else if (MyStrcmp(rpn[i].value, "/") == 0) {
                        if (MyFabs(b.number) < EPSILON) {
                            result.number = 1e10;
                        } else {
                            result.number = a.number / b.number;
                        }
                    } else if (MyStrcmp(rpn[i].value, "^") == 0) {
                        result.number = MyPow(a.number, b.number);
                    }

                    Push(&stack, result);
                } else {
                    if (IsEmpty(&stack)) {
                        fprintf(stderr, "Invalid expression: not enough operands\n");
                        FreeStack(&stack);
                        exit(1);
                    }
                    Token a = Pop(&stack);

                    Token result = {NUMBER, "", -a.number};
                    Push(&stack, result);
                }
            } break;

            case FUNCTION: {
                if (IsEmpty(&stack)) {
                    fprintf(stderr, "Invalid expression: not enough operands for function\n");
                    FreeStack(&stack);
                    exit(1);
                }
                Token a = Pop(&stack);

                Token result = {NUMBER, "", 0};

                if (MyStrcmp(rpn[i].value, "sin") == 0) {
                    result.number = MySin(a.number);
                } else if (MyStrcmp(rpn[i].value, "cos") == 0) {
                    result.number = MyCos(a.number);
                } else if (MyStrcmp(rpn[i].value, "tan") == 0) {
                    result.number = MyTan(a.number);
                } else if (MyStrcmp(rpn[i].value, "ctg") == 0) {
                    double tan_val = MyTan(a.number);
                    if (MyFabs(tan_val) < EPSILON) {
                        result.number = 1e10;
                    } else {
                        result.number = 1.0 / tan_val;
                    }
                } else if (MyStrcmp(rpn[i].value, "sqrt") == 0) {
                    if (a.number < 0) {
                        result.number = (0.0 * 1.0) / (0.0 + 0.0);;
                    } else {
                        result.number = MySqrt(a.number);
                    }
                } else if (MyStrcmp(rpn[i].value, "ln") == 0) {
                    if (a.number <= 0) {
                        result.number = (0.0 * 1.0) / (0.0 + 0.0);;
                    } else {
                        result.number = MyLog(a.number);
                    }
                }

                Push(&stack, result);
            } break;

            default:
                break;
        }
    }

    if (IsEmpty(&stack)) {
        fprintf(stderr, "Invalid expression: empty result\n");
        FreeStack(&stack);
        exit(1);
    }

    Token result = Pop(&stack);

    if (!IsEmpty(&stack)) {
        fprintf(stderr, "Invalid expression: too many operands\n");
        FreeStack(&stack);
        exit(1);
    }

    double value = result.number;
    FreeStack(&stack);
    return value;
}

void PlotGraph(const char* expression) {
    char graph[HEIGHT][WIDTH + 1];
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            graph[i][j] = '.';
        }
        graph[i][WIDTH] = '\0';
    }

    Token* tokens = Tokenize(expression);
    Token* rpn = ShuntingYard(tokens);

    double y_values[WIDTH];
    for (int j = 0; j < WIDTH; j++) {
        double x = X_MIN + (X_MAX - X_MIN) * j / (WIDTH - 1);
        
        y_values[j] = EvaluateRPN(rpn, x);
        
        if (y_values[j] < Y_MIN) y_values[j] = Y_MIN;
        if (y_values[j] > Y_MAX) y_values[j] = Y_MAX;
    }

    for (int j = 0; j < WIDTH; j++) {
        int screen_y = MyRound((Y_MAX - y_values[j]) * (HEIGHT - 1) / (Y_MAX - Y_MIN));
        
        if (screen_y >= 0 && screen_y < HEIGHT) {
            graph[screen_y][j] = '*';
        }
        
        if (j > 0) {
            int prev_screen_y = MyRound((Y_MAX - y_values[j-1]) * (HEIGHT - 1) / (Y_MAX - Y_MIN));
            
            if (MyFabs(screen_y - prev_screen_y) > 1) {
                int start = prev_screen_y < screen_y ? prev_screen_y : screen_y;
                int end = prev_screen_y < screen_y ? screen_y : prev_screen_y;
                
                for (int k = start + 1; k < end; k++) {
                    if (k >= 0 && k < HEIGHT) {
                        graph[k][j-1] = '*';
                    }
                }
            }
        }
    }

    for (int i = 0; i < HEIGHT; i++) {
        printf("%s\n", graph[i]);
    }

    free(tokens);
    free(rpn);
}
