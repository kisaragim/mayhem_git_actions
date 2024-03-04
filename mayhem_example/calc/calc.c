#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mydef.h"
#include "polishnotation.h"
#include "validate.h"

extern char array[ARRAY_NUM];
extern char inputErrorMess[MESS_NUM];

int main(int argc, char *argv[]);

int
calc_func(char *formula)
{
    printf("計算式は%sです\n", formula);
    if (!validateInputFormula(formula))
    {
        printf("%s", inputErrorMess);
        return 0;
    }

    toPolishNotation(formula);

    printf("計算結果は%dです\n", calcPolishNotation(array));

    return 0;
}

int main(int argc, char *argv[]){
	FILE *fp;
	char buf[MAX_NUM] = {0};
	if(argc != 2){
		fprintf(stderr, "don’t exist text file¥n");
		return -1;
	}
	fp = fopen(argv[1], "r");
	if(fp == NULL){
		fprintf(stderr, "Could not open %s¥n", argv[1]);
		return -1;
	}
	if(fgets(buf, sizeof(buf), fp) == NULL){
		fprintf(stderr, "Could not read from %s¥n", argv[1]);
		return -1;
	}
	calc_func(buf);
	return 0;
}
