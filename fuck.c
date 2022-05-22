// make fi

#include <uv.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>

uv_loop_t*loop;

uv_pipe_t pip0;
uv_pipe_t pip1;
uv_pipe_t pip2;
uv_pipe_t pip3;
uv_pipe_t pip4;
uv_pipe_t pip5;

uv_process_t child_req;
uv_process_options_t options;

static void on_exiti(uv_process_t*, int64_t, int);
static void after_write(uv_write_t* , int);
static void on_alloc(uv_handle_t* handle, size_t si, uv_buf_t* buf);
static void on_read(uv_stream_t* tcp, ssize_t nread, const uv_buf_t* buf);

int main(){
	

	int r;

	uv_buf_t buf;
	
	loop = uv_default_loop();
	char* args[2];
	args[0] = "./l"; //child2.c
	
	args[1] = NULL; 
	
	
	
	uv_stdio_container_t child_stdio[5];
	
	child_stdio[0].flags = UV_IGNORE;
	child_stdio[1].flags = UV_IGNORE;
	child_stdio[2].flags = UV_INHERIT_FD;
	child_stdio[2].data.fd = 2;
	child_stdio[3].flags = UV_CREATE_PIPE | UV_READABLE_PIPE; //ABLE_PIPE;
	child_stdio[3].data.stream = (uv_stream_t*) &pip3;
	
	child_stdio[4].flags = UV_CREATE_PIPE | UV_WRITABLE_PIPE;
	child_stdio[4].data.stream = (uv_stream_t*) &pip4;

	
	options.stdio = child_stdio;
	options.stdio_count = 5;
	
	options.exit_cb = on_exiti;
	options.file = "./l";
	options.args = args;
	options.flags = 0;
	
	r = uv_pipe_init(loop, &pip0, 0);
	printf("r => %d\n", r);
	r = uv_pipe_init(loop, &pip1, 0);
	printf("r => %d\n", r);
	r = uv_pipe_init(loop, &pip2, 0);
	printf("r => %d\n", r);
	
	r = uv_pipe_init(loop, &pip3, 0);
	printf("r => %d\n", r);

	if(r !=0)fprintf(stderr, "open 3 parent => %s\n", uv_strerror(r));
	
	r = uv_pipe_init(loop, &pip4, 0);
	printf("r => %d\n", r);
	
	if(r !=0)fprintf(stderr, "open 4 => %s\n", uv_strerror(r));
uv_write_t *write_req;
	write_req = malloc(sizeof(write_req));
	if(write_req == NULL)printf("write_req null\n");
	uv_buf_t bufi = uv_buf_init("b\n", 1);
	

	if((r= uv_spawn(loop, &child_req, &options))){
		fprintf(stderr, "*** spawn *** %s\n", uv_strerror(r));
		return 1;
	}else{
		fprintf(stderr, "pid %d\n", child_req.pid);
	}
	
	r = uv_write(write_req, (uv_stream_t*)&pip3 , &bufi, 1, after_write);
	if(r !=0)fprintf(stderr, "write parent uv write => %s\n", uv_strerror(r));
	
	r = uv_read_start((uv_stream_t*)&pip4, on_alloc,( uv_read_cb)on_read);
	if(r !=0)printf("parent uv read start => %s\n", uv_strerror(r));
	
	
	
	
	
	
	
	
	return uv_run(loop, UV_RUN_DEFAULT);
}

static void on_exiti(uv_process_t* req, int64_t exit_status, int term_signal){
	fprintf(stderr, "Process exited with status %" PRId64 ", signal %d\n", exit_status, term_signal);
	uv_close((uv_handle_t*) req, NULL);
	uv_close((uv_handle_t*)&pip0, NULL);
	uv_close((uv_handle_t*)&pip1, NULL);
	uv_close((uv_handle_t*)&pip2, NULL);
	uv_close((uv_handle_t*)&pip3, NULL);
	uv_close((uv_handle_t*)&pip4, NULL);
}
int a = 0;
  static void on_read(uv_stream_t *tcp, ssize_t nread, const uv_buf_t *bu){
	
	a++;
	
	fprintf(stderr, "data len: %ld\n", nread);
	fprintf(stderr, "parent buf %s\n", bu->base);
	
	free(bu->base);
	uv_read_stop(tcp);
}
static void after_write(uv_write_t* req, int status){
	fprintf(stderr, "Parent status after write %d\n", status);
	free(req);
}



static void on_alloc(uv_handle_t* handle, size_t si, uv_buf_t* buf){
	//buf->base = malloc(si);
	//buf->len = si;
//	uv_buf_init((char*) malloc(si), si);
	printf("ON ALLOK NREAD %ld\n", si);
	*buf = uv_buf_init((char*) malloc(si), si);
	if(buf->base == NULL) fprintf(stderr, "bufa->base is NULL");
}























