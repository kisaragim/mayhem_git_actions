#ifndef _POLISHNOTATION_H_
#define _POLISHNOTATION_H_

#include "mydef.h"

void pushChar(char*,
          char,
          int*);
char popChar(char*, int*);
void pushDouble(double*,
                double,
                int*);
double popDouble(double*, int*);
int calcPriority(char);
void pushOperator(char);
double calcFormula(char,
                   double,
                   double);
void toPolishNotation(char*);
int calcPolishNotation(char*);

#endif
