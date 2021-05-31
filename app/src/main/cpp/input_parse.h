//
// Created by liguanghao on 5/31/2021.
//

#ifndef MY_APPLICATION_INPUT_PARSE_H
#define MY_APPLICATION_INPUT_PARSE_H

#include <jni.h>
#include <string>
#include "table.h"

#include <android/log.h>


#include <iostream>
#include <iterator>
#include <stack>
#include <sstream>
#include <vector>
#include <math.h>

using namespace std;


#define  LOG_TAG    "your-log-tag"

#define  LOGE(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)


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

bool TryVariable(const string &symbol);

bool TryParse(const string &symbol);

int Priority(const string &c);

bool isOperator(const string &c);


int parse_expression();

double equationFunction(double x, double y, double z);
string infix2Rpn(string infix);
#endif //MY_APPLICATION_INPUT_PARSE_H
