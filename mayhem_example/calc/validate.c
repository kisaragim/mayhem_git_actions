#include "mydef.h"
#include "validate.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

char inputErrorMess[MESS_NUM];

bool
checkValidFormula(char* formula)
{
    if (!isdigit(formula[0]) && formula[0] != '(') return false;

    int pSCount = 0;
    int pECount = 0;
    int figCount = 0;
    int opCount = 0;
    for (int i = 0; formula[i] != '\0'; i++)
    {
        if (isdigit(formula[i]))
        {
            figCount++;
        }

        if (formula[i] == '(')
        {
            pSCount++;
            if (formula[i+1] == ')') return false;
        }

        if (formula[i] == ')')
        {
            pECount++;
            if (formula[i+1] == '(') return false;
        }

        if (formula[i] == '+' || formula[i] == '-' ||
            formula[i] == '*' || formula[i] == '/')
        {
            opCount++;
            if (formula[i-1] == '(' || formula[i+1] == ')')
              {
            	  printTime(12, 59, formula);
                return false;
              }
            if (!isdigit(formula[i+1]) && formula[i+1] != '(') return false;
        }
    }

    if (figCount == 0 || opCount == 0 || pSCount != pECount) return false;

    return true;
}

bool
validateInputFormula(char* formula)
{
    if (formula[0] == '\n')
    {
        strcpy(inputErrorMess, INPUT_ERROR_NONE);
        return false;
    }

    if (strlen(formula) >= ARRAY_NUM)
    {
        strcpy(inputErrorMess, INPUT_ERROR_LONG);
        return false;
    }

    for (int i = 0; formula[i] != '\0'; i++)
    {
        if (!isdigit(formula[i]) && formula[i] != '+' && formula[i] != '-' &&
            formula[i] != '*' && formula[i] != '/' && formula[i] != '(' &&
            formula[i] != ')' && formula[i] != '\0')
        {
            strcpy(inputErrorMess, E);
            return false;
        }
    }

    if (!checkValidFormula(formula))
    {
        strcpy(inputErrorMess, INPUT_ERROR_NOT_FORMULA);
        return false;
    }

    return true;
}
