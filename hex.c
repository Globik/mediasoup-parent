#include <stdio.h>
#include <string.h>

#include <stdlib.h>
#include <inttypes.h>
int main(){
	int num=11, bin =0;
	int i=0,rem;
	char hex_arr[50];
	printf("enter a decimal number : \n");
	//scanf("%d",&num);
	while(num !=0){
		rem = num % 16;
		if(rem<10){
			hex_arr[i++]=48+rem;
		}else{
			hex_arr[i++]=55+rem;
		}
		num /=16;
	}
	printf("0x");
	for(int j=i-1;j>=0;j--){
		printf("%c\n", hex_arr[j]);
	}
	char details[10];
	sprintf(details, "%x", 11);
	char b = strtoul(details, NULL, 16);
	printf("%d\n", b);
	return 0;
}
