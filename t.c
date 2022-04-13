#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>



int main(int argc, char*argv[]){
	int pipeends[4];
	if(pipe(pipeends) !=0){
		return 1;
		printf("error1\n");
	}else{
		pid_t pid = fork();
		if(pid == 0){
			//(void) close(pipeends[0]);
			(void) close(pipeends[1]);
			//if(dup2(pipeends[1], 3) == -1){
			if(dup2(pipeends[3], 3) == -1){
				printf("error\n");
				return 1;
			}
			//(void) close(pipeends[1]);
			(void) close(pipeends[0]);
			//char fn[] = "/home/globi/mediasoup-parent/f";
			char fn[] = "/home/globi/mediasoup-parent/child";
			execl(fn, fn, NULL);
		}else{
			/*
			close(pipeends[1]);
			char buf[100];
			memset(buf, 0, sizeof(buf));
			read(pipeends[0], buf, sizeof(buf));
			printf("%s\n", buf);
			close(pipeends[0]);
			*/
			//close(pipeends[0]);
			
			char msga[] = "hello world2\n";
			write(3, msga, sizeof(msga));
			close(1);
			
			
			
			waitpid(pid, NULL,0);
		}
		
	}
	printf("bye\n");
	return 0;
}
