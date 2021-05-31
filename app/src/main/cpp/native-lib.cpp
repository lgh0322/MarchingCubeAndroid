#include <jni.h>
#include <string>
#include "table.h"
#include <math.h>
#include <android/log.h>
#define  LOG_TAG    "your-log-tag"

#define  LOGE(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#define isnumber(c)            ((c >= '0' && c <= '9') || c == '.')
#define isvariable(c)            (c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f')
#define isunknown(c)            (c == 'x' || c == 'y' || c == 'z')
#define isoperator(c)            (c == 131)

#define MAX_TOKEN                300
#define MAX_VARIABLE            6
#define MAX_PARAMETER            6

#define MAX_FUNCTION            21

#define STACK_DEPTH                100
#define NAME_LENGTH                20


#define NUMBER_TYPE                0
#define FUNCTION_TYPE            1
#define VARIABLE_TYPE            2
#define UNKNOWN_TYPE            3
#define OPERATOR_TYPE            4

#define    X                        0
#define    Y                        1
#define    Z                        2

#define EPSILON_E5                    (float) (1E-5)


typedef struct {
    float value[MAX_VARIABLE];
    int flag[MAX_VARIABLE];
} VARIABLE;

typedef struct {
    int type;
    union {
        float value;
        int index;
    };
} TOKEN;

typedef struct {
    const char *name;
    int argnum;

    double (*function)();
} FUNCTION;

typedef struct {
    char *expression;
    char *parameter;
    char *variable;
    TOKEN xyz_token[MAX_TOKEN];
    int xyz_tokennumber;
} EQUATION;

typedef struct {
    char *xyz;
} EQUATION_INFO;


int token = 0;


float execution_stack[STACK_DEPTH];
int stack_index;

double addition() { return execution_stack[stack_index - 1] + execution_stack[stack_index]; };

double subtraction() { return execution_stack[stack_index - 1] - execution_stack[stack_index]; };

double multiplication() { return execution_stack[stack_index - 1] * execution_stack[stack_index]; };

double division() {
    if (execution_stack[stack_index] == 0.0) execution_stack[stack_index] = EPSILON_E5;
    return execution_stack[stack_index - 1] / execution_stack[stack_index];
}

double neg() { return -execution_stack[stack_index]; }

double inv() {
    if (execution_stack[stack_index] == 0.0) execution_stack[stack_index] = EPSILON_E5;
    return 1.0 / execution_stack[stack_index];
}

double sq() { return execution_stack[stack_index] * execution_stack[stack_index]; };

double sinhp() {
    return (exp(execution_stack[stack_index]) - exp(-execution_stack[stack_index])) * 0.5;
}

double coshp() {
    return (exp(execution_stack[stack_index]) + exp(-execution_stack[stack_index])) * 0.5;
}

double tanhp() {
    double q;
    if ((q = coshp()) == 0.0) q = EPSILON_E5;
    return (sinhp() / q);
}

double sina() { return sin(execution_stack[stack_index]); }

double cosa() { return cos(execution_stack[stack_index]); }

double tana() { return tan(execution_stack[stack_index]); }

double asina() { return asin(execution_stack[stack_index]); }

double acosa() { return acos(execution_stack[stack_index]); }

double atana() { return atan(execution_stack[stack_index]); }

double lna() { return log(execution_stack[stack_index]); }

double loga() { return log10(execution_stack[stack_index]); }

double absa() { return fabs(execution_stack[stack_index]); }

double expa() { return exp(execution_stack[stack_index]); }

double powa() { return pow(execution_stack[stack_index - 1], execution_stack[stack_index]); }

FUNCTION function[] = {
        {"SIN",  1, sina},
        {"COS",  1, cosa},
        {"TAN",  1, tana},
        {"SINH", 1, sinhp},
        {"COSH", 1, coshp},
        {"TANH", 1, tanhp},
        {"ASIN", 1, asina},
        {"ACOS", 1, acosa},
        {"ATAN", 1, atana},
        {"LN",   1, lna},
        {"LOG",  1, loga},
        {"NEG",  1, neg},
        {"INV",  1, inv},
        {"ABS",  1, absa},
        {"EXP",  1, expa},
        {"SQ",   1, sq},
        {"^",    2, powa},
        {"+",    2, addition},
        {"-",    2, subtraction},
        {"*",    2, multiplication},
        {"/",    2, division},
};


VARIABLE variable = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
EQUATION equation;

char function_name[NAME_LENGTH];









double i10powx(int a){
    return pow((double)10,(double)a);
}



char *
lower(char *t, const char *s)
{
    char *r = t;
    while (*s)
        *t++ = tolower(*s++) ;
    *t = '\0';
    return r;
}




//--------------------------------------这个是解析字符串常数的， 例如"2343.563e-2，    1423445346"

double atof_local(const char * _buf) {
    double val,expo;
    int minus,fraction_digits;
    double integral,ifraction;

    char c,*__buf,*buf;

    if( (__buf = (char *)  malloc((strlen(_buf) + 1) * sizeof(char)) ) == NULL )
        return 0;

    buf = lower(__buf,_buf); // just handle lowercase chars

    integral = ifraction = 0;
    fraction_digits = 0;
    val = 0.0;
    expo = 1.0;

    if((minus = (*buf == '-')))
        buf++;
    if (*buf == '+')
        buf++;

    while (*buf && *buf != '.') {
        c = *buf++;
        if (isdigit(c))
            integral = 10 * integral + c - '0';
        else {

            if (c == 'e') {
                buf--;
                goto parse_expo;
            }
            goto finish;
        }
    }


    if (*buf == '.') {
        buf++;
        while (*buf && *buf != 'e' ) {
            c = *buf++;
            if (! isdigit(c)) goto finish;
            fraction_digits++;
            ifraction = 10 * ifraction + c - '0';
        }
    }

    parse_expo:

    if (*buf == 'e') {
        int expo_minus,ee = 0;
        buf++;
        if ((expo_minus = (*buf == '-')))
            buf++;
        if (*buf == '+')
            buf++;
        while (*buf) {
            c = *buf++;
            if (! isdigit(c))
                goto finish;
            ee = 10 * ee + c - '0';
        }
        // REPLACED IPOW(10,X) BY I10POWX(X) - CLAUDIO MAR 2006

        expo = i10powx(expo_minus ? -ee : ee);
    }

    finish:

    free(__buf);

    val = expo * ((double) integral + (double) ifraction / i10powx(fraction_digits));
    return minus ? -val : val;
}




int parse_expression() {
    char *expression = equation.expression;

    if(*expression != '{' || expression[strlen(expression) - 1] != '}')
    {
        LOGE("Error: not a valid expression\n");
        return 0;
    }

    expression += 2; equation.xyz_tokennumber = 0;

    while(*expression != '}') {

        if(equation.xyz_tokennumber >= MAX_TOKEN)
        {
            LOGE("Error: too many tokens!\n");
            return 0;
        }

        if( isnumber(*expression) || ( *expression == '-' &&  isnumber(* (expression + 1)))) {
            equation.xyz_token[equation.xyz_tokennumber].value = atof_local(expression);
            equation.xyz_token[equation.xyz_tokennumber++].type = NUMBER_TYPE;
            while(*expression != ' ') ++expression; ++expression;
        }

        else if( isvariable(*expression) && *(expression + 1) == ' ') {
            int index = *expression - 'a';
            equation.xyz_token[equation.xyz_tokennumber].index = index;
            equation.xyz_token[equation.xyz_tokennumber++].type = VARIABLE_TYPE;
            variable.flag[index] = 1; expression += 2;
        }

        else if( isunknown(*expression) && *(expression + 1) == ' ') {
            equation.xyz_token[equation.xyz_tokennumber].index = *expression - 'x';
            equation.xyz_token[equation.xyz_tokennumber++].type = UNKNOWN_TYPE;
            expression += 2;
        }

        else if( isoperator((int) *expression) && *(expression + 1) == ' ') {
            equation.xyz_token[equation.xyz_tokennumber].index = (int) *expression;
            equation.xyz_token[equation.xyz_tokennumber++].type = OPERATOR_TYPE;
            expression += 2;
        }

        else {
            int i = 0;
            while(*expression != ' ') function_name[i++] = *expression++;
            function_name[i] = '\0'; ++expression;
            int found = 0;
            for(i = 0; i < MAX_FUNCTION; i++) {
                if(!strcmp(function[i].name, function_name)) {
                    equation.xyz_token[equation.xyz_tokennumber].index = i;
                    equation.xyz_token[equation.xyz_tokennumber++].type = FUNCTION_TYPE;
                    found = 1;break;
                }
            }
            if(!found) {LOGE("Error: token unknown in expression: %s\n", function_name);  return 0;}
        }
    }
    return 1;
}







double equationFunction(double x, double y, double z) {
    stack_index = -1;
    for(token = 0; token < equation.xyz_tokennumber; token++) {
        switch(equation.xyz_token[token].type) {
            case NUMBER_TYPE:	execution_stack[++stack_index] = equation.xyz_token[token].value;
                break;

            case UNKNOWN_TYPE:	if(equation.xyz_token[token].index == X) execution_stack[++stack_index] = x;
                else if(equation.xyz_token[token].index == Y) execution_stack[++stack_index] = y;
                else execution_stack[++stack_index] =z;
                break;

            case VARIABLE_TYPE:	execution_stack[++stack_index] = variable.value[equation.xyz_token[token].index];
                break;

            case OPERATOR_TYPE:	execution_stack[stack_index] = sqrt(execution_stack[stack_index]);
                break;

            case FUNCTION_TYPE:if(function[equation.xyz_token[token].index].argnum == 1) {
                    execution_stack[stack_index] =
                            function[equation.xyz_token[token].index].function();
                }
                else {
                    execution_stack[stack_index - 1] =
                            function[equation.xyz_token[token].index].function();
                    --stack_index;
                }
                break;
        }
    }

    return execution_stack[stack_index];
}



















using namespace std;


extern "C" JNIEXPORT jstring JNICALL
Java_com_vaca_myapplication_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    equation.expression="{ x 3 + }";
    parse_expression();
    double x=equationFunction(2,0,0);
    hello+=to_string(x);
    return env->NewStringUTF(hello.c_str());
}