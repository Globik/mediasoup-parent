#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void){
	//char msg[] = "hello world\n";
	//write(3, msg, sizeof(msg));
	char buf[100];
			memset(buf, 0, sizeof(buf));
			read(3, buf, sizeof(buf));
			//printf("child: %s", buf);
	close(3);
	return 0;
}
