#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "scaletoscale.h"

char* bintohex(char* bin){
	char * str = "0123456789ABCDEF";
	char* num = (char*)malloc(5);
	int len = strlen(bin);
	int lenhex = len / 4;
	int count = len % 4;
	if (count == 0){
		count = 4;
	}
	else{
		lenhex ++;
	}
	char* hex = (char*)malloc(lenhex + 1);
	hex[lenhex] = '\0';
	int ind = 0;
	int c = lenhex;
	for (int i = 0; i < c; i++){
		num[count] = '\0';
		for(int j = 0; j<count; j++){
			num[j]=bin[j];
		}
		int n = (unsigned char)strtol(num, NULL, 2);
		//printf("%d\n", n);
		if(n == 0 && ind == 0){
			//printf("Zero first\n");
			lenhex--;
			hex = (char*)realloc(hex,lenhex + 1);
			hex[lenhex] = '\0';
		}
		else{
			//printf("%d\n", n);
			//printf("%c\n", str[n]);
			hex[ind] = str[n];
			ind++;
		}
		bin += count;
		count = 4;
	}
	//printf("%s\n", hex);
	return hex;
}
