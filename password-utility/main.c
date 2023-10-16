#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "scaletoscale.h"
#include "passwordmask.h"



char* fgetstr(FILE* fd)
{
	char* ptr = (char*)malloc(1);
	char buf[81];
	int n, len = 0;
	*ptr = '\0';
	do {
		n = fscanf(fd, "%80[^\n]", buf);
		//if (n < 0) {
		//	free(ptr);
		//	ptr = NULL;
		//	continue;
			//break;
		//}
		if (n == 0) {
			fscanf(fd, "%*c");
			//break;
		}
		else if (n>0){
			len += strlen(buf);
			ptr = (char*)realloc(ptr, len + 1);
			int k = strlen(buf);
			int l = strlen(ptr) + k + 1;
			strncat(ptr,buf, k);
		}
	} while (n > 0);
	return ptr;
}

int load(char* fname, char* output_file)
{

	FILE* fd;
	int t = 0;
	fd = fopen(fname, "r");
	FILE* ptr;
	ptr = fopen(output_file, "w");
	if (fd == NULL)
		return 0;
	while (t != EOF) {
		char* str = "\0";// (char*)malloc(99);
		str = fgetstr(fd);
		if (*str == '\0')
			break;
		str[strlen(str)] = '\0';
		char* first = str;
		if(strlen(str) > 64){
			char * mask;
			mask = mask_fun(str);
			char* hex = bintohex(mask);
			printf("CAN'T GENERATE (LEN > 64). WRONG INPUT\n");
			printf("%ld_%s\n", strlen(str), hex);
			free(str);
			free(hex);
			hex=NULL;
			str = NULL;
			free(mask);
			mask = NULL;
		}
		else{
			while(*str != '\0'){
				unsigned long long int_mask = int_mask_fun(str);
				printf("%ld_%llX\n", strlen(str), int_mask);
				all_passwords(int_mask, strlen(first), ptr);
				str++;
			}
			free(first);
			first = NULL;
		}
		if (t > 0)
			t++;
	}
	fclose(ptr);
	fclose(fd);
	return 0;
}

char* getstr()
{
	char* ptr = (char*)malloc(1);
	char buf[81];
	int n, len = 0;
	*ptr = '\0';
	do {
		n = scanf("%80[^\n]", buf);
		if (n < 0) {
			free(ptr);
			ptr = NULL;
			continue;
			break;
		}
		if (n == 0) {
			scanf("%*c");
			//break;
		}
		else {
			len += strlen(buf);
			ptr = (char*)realloc(ptr, len + 1);
			int k = strlen(buf);
			int l = strlen(ptr) + k + 1;
			strncat(ptr, buf, k);
		}
	} while (n > 0);
	return ptr;
}



int main(int argc, char** argv) {
	char* file_name;
	if (argv[1] == NULL){
		printf("Enter input file name --> ");
		file_name = getstr();
	}
	else
		file_name = argv[1];
	char* output_file;
	if (argv[2] == NULL){
		printf("Enter output file name --> ");
		output_file = getstr();
	}
	else
		output_file = argv[2];
	load(file_name, output_file);
	if (argv[1] == NULL){
		free(file_name);
	}
	if (argv[2] == NULL){
		free(output_file);
	}
}
