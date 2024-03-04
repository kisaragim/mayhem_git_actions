#include <stdio.h>
#include <string.h>

int AuthCheck(char *password)
{
    int auth_flag = 0 ;
    char buffer[10] ;

#ifdef MODIFY
    if(strlen(password) < sizeof(buffer))
    {
#endif
        strcpy(buffer, password) ;
        if(strcmp(buffer, "1+10") == 0)
         {
            auth_flag = 1 ;
         }
#ifdef MODIFY
    }
#endif

    return auth_flag;
}

void btoa(unsigned char dat, char *buff)
{
	unsigned char dec[3] = {100, 10, 1};
	unsigned char work[3];
	int	i;

	for(i = 0; i < 3; i++)
	{
		work[i] = dat / dec[i];
		dat = dat % dec[i];
		buff[i] = work[i] | '0';
	}
	for(i = 0; i < 3; i++)
	{
		work[i] = dat / dec[i];
		dat = dat % dec[i];
		buff[i] = work[i] | '0';
	}
	for(i = 0; i < 3; i++)
	{
		work[i] = dat / dec[i];
		dat = dat % dec[i];
		buff[i] = work[i] | '0';
	}

	buff[i] = 0;
	return;
}

void printTime(char hour, char min, char *pass)
{
	char strHour[3];
	char strMin[3];

	btoa(hour, strHour);
	btoa(min, strMin);
	printf("%s:%s\n", &strHour[1], &strMin[1]);
	AuthCheck(pass);
}
