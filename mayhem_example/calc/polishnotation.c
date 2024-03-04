#include "polishnotation.h"
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

char array[MAX_NUM];
char arrayTemp[ARRAY_NUM];
double calcArray[ARRAY_NUM];

int sp;
int spTemp;
int calcsp;

void
pushChar(char* stack,
         char data,
         int* sp)
{
    if (*sp < ARRAY_NUM)
    {
        stack[(*sp)++] = data;
    }
}

char
popChar(char* stack, int* sp)
{
    if (*sp > 0)
    {
        return stack[--(*sp)];
    }
    else
    {
        return 0;
    }
}

void
pushDouble(double* stack,
           double data,
           int* sp)
{
    if (*sp < ARRAY_NUM)
    {
        stack[(*sp)++] = data;
    }
}

double
popDouble(double* stack, int* sp)
{
    if ( *sp > 0 )
    {
        return stack[--(*sp)];
    }
    else
    {
        return 0;
    }
}

int
calcPriority(char op)
{
    int priority = 0;

    if (op == '/')
    {
        priority = 3;
    }
    else if (op == '*')
    {
        priority = 2;
    }
    else if (op == '+' || op == '-')
    {
        priority = 1;
    }

    return priority;
}

void
pushOperator(char op)
{
    bool flag = false;
    while (true)
    {
        if (!flag)
        {
            char opTemp = popChar(arrayTemp, &spTemp);

            int priorityA = calcPriority(op);
            int priorityB = calcPriority(opTemp);

            if (priorityA >= priorityB)
            {
                pushChar(arrayTemp, opTemp, &spTemp);
                pushChar(arrayTemp, op, &spTemp);
                break;
            }

            pushChar(array, opTemp, &sp);
            pushChar(array, ',', &sp);
            pushChar(arrayTemp, op, &spTemp);
            flag = true;
        }
        else
        {
            char opTempA = popChar(arrayTemp, &spTemp);
            char opTempB = popChar(arrayTemp, &spTemp);

            int priorityA = calcPriority(opTempA);
            int priorityB = calcPriority(opTempB);

            if (priorityA >= priorityB)
            {
                pushChar(arrayTemp, opTempB, &spTemp);
                pushChar(arrayTemp, opTempA, &spTemp);
                break;
            }

            pushChar(array, opTempB, &sp);
            pushChar(array, ',', &sp);
            pushChar(arrayTemp, opTempA, &spTemp);
        }
    }
}

double
calcFormula(char op,
            double figA,
            double figB)
{
    double result = 0;

    if (figA > INT_MAX || figB > INT_MAX)
    {
        printf(ERROR_LONG);
        exit(1);
    }

    if (op == '/')
    {
        if (figA == 0)
        {
            printf(CALC_ERROR);
            exit(1);
        }

        result = figB / figA;
    }
    else if (op == '*')
    {
        result = figB * figA;
    }
    else if (op == '+')
    {
        result = figB + figA;
    }
    else if (op == '-')
    {
        result = figB - figA;
    }

    return result;
}

void
toPolishNotation(char* formula)
{
    char temp = 0;

    for (int i = 0; formula[i] != '\0'; i++)
    {
        if (formula[i] == '(')
        {
            pushChar(arrayTemp, formula[i], &spTemp);
        }
        else if (formula[i] == ')')
        {
            while ((temp = popChar(arrayTemp, &spTemp)) != '(')
            {
                pushChar(array, temp, &sp);
                pushChar(array, ',', &sp);
            }
        }
        else if (isdigit(formula[i]))
        {
            pushChar(array, formula[i], &sp);

            while (true)
            {
                if (!isdigit(formula[i+1]))
                {
                    pushChar(array, ',', &sp);
                    break;
                }
                pushChar(array, formula[i+1], &sp);
                i++;
            }
        }
        else if (formula[i] == '+' || formula[i] == '-' ||
                 formula[i] == '*' || formula[i] == '/')
        {
            pushOperator(formula[i]);
        }
    }

    while (spTemp != 0)
    {
        pushChar(array, popChar(arrayTemp, &spTemp), &sp);
        pushChar(array, ',', &sp);
    }
}

double myround(double x)
{
  return x > 0.0 ? (long)(x + 0.5) : (long)(x - 0.5);
}

int
calcPolishNotation(char* polishFormula)
{
    int calcResult = 0;

    for (int i = 0; polishFormula[i] != '\0'; i++)
    {
        if (isdigit(polishFormula[i]))
        {
            char temp[ARRAY_NUM] = {polishFormula[i]};

            int j = 1;
            while (true)
            {
                if (!isdigit(polishFormula[i+1]))
                {
                    pushDouble(calcArray, atof(temp), &calcsp);
                    break;
                }
                temp[j++] = polishFormula[i+1];
                i++;
            }
        }

        if (polishFormula[i] == '+' || polishFormula[i] == '-' ||
            polishFormula[i] == '*' || polishFormula[i] == '/')
        {
            double figA = popDouble(calcArray, &calcsp);
            double figB = popDouble(calcArray, &calcsp);
            double result = calcFormula(polishFormula[i], figA, figB);

            if (result > INT_MAX)
            {
                printf(ERROR_LONG);
                exit(1);
            }
            pushDouble(calcArray, result, &calcsp);
        }
    }

    calcResult = (int)myround(calcArray[0]);

    return calcResult;
}
