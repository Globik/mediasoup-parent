#include <stdio.h>
int main(){
	int num, bin =0;
	int i=0,rem;
	char hex_arr[50];
	printf("enter a decimal number : ");
	scanf("%d",&num);
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
		printf("%c", hex_arr[j]);
	}
	
	return 0;
}
