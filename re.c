#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <inttypes.h>
#include <stdarg.h>
 
 int arr[] = {32, 33, 34, 35, 36}; 

 int simple[] = {45, 33, 35, 34, 40, 41, 42, 45, 47, 48};
 char *val[5] = {" ", "!", "\"","#", "$"};
 
 int brite(char *buf, int buf_size, const char *fmt, ...);
int main(){
	 //65
	 int a = 123;int len;
	 char c = a;
	 printf("%c\n", a);
	char buffer[100]; char duffer[255] = {0};
	int offset = 0;
	 char fname[20] = "fucker";
	len= brite(buffer, 27, " %s\n", fname);
	printf("buffer %s\n", buffer);
	for(int i = 0; i < 5; i++){
		offset += snprintf(duffer + offset, 255 - offset, "%c", arr[i]);
	}
	printf("duffer: %s\n",  duffer);
	char suffer[255 ] = {0};
	
	char fmtbuf[64];
	for(int i = 0; i < 5;i++){
		snprintf(fmtbuf, 64, "%c", i);
		strcat(suffer, fmtbuf);
	}
	printf("suffer: %s\n", suffer);
	 return 0;
 }
int brite(char *buf, int buf_size, const char *fmt, ...){
	int len;
	va_list args;
	va_start(args, fmt);
	len = vsnprintf(buf, buf_size, fmt, args);
	printf("len %d\n", len);
	va_end(args);
	return len;
}
