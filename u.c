#include <uv.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>

uv_loop_t*loop;
uv_process_t child_req;
uv_process_options_t options;
uv_pipe_t pip0;
uv_pipe_t pip1;
uv_pipe_t pip2;
uv_pipe_t pip3;
uv_pipe_t pip4;
char output[12];

static void on_exiti(uv_process_t*, int64_t, int);
static void after_write(uv_write_t* , int);
static void on_alloc(uv_handle_t* handle, size_t si, uv_buf_t* buf);
static void on_read(uv_stream_t tcp, ssize_t nread, const uv_buf_t* buf);

int main(){
	int r;
	uv_write_t *write_req;
	uv_buf_t buf;
	
	loop = uv_default_loop();
	
	
	//memcmp("hello world\n", output, 12);
	buf = uv_buf_init("hello world\n", 12);
	write_req = malloc(sizeof(*write_req));
	
	char* args[2];
	//args[0] = "./child";
	args[0] = "./l";
	args[1] = NULL;
	
	uv_stdio_container_t child_stdio[5];
	child_stdio[0].flags = UV_IGNORE;
	child_stdio[1].flags = UV_IGNORE;
	child_stdio[2].flags = UV_IGNORE;
	//child_stdio[2].data.fd = 2;
	child_stdio[3].flags = UV_CREATE_PIPE | UV_WRITABLE_PIPE;
	child_stdio[3].data.fd = 3;
	child_stdio[3].data.stream = (uv_stream_t*) &pip3;
	
	child_stdio[4].flags = UV_CREATE_PIPE | UV_READABLE_PIPE;
	child_stdio[4].data.stream = (uv_stream_t*) &pip4;
	child_stdio[4].data.fd = 4;
	options.stdio = child_stdio;
	options.stdio_count = 5;
	
	options.exit_cb = on_exiti;
	options.file = "./l";
	//options.file = "./l";
	options.args = args;
	
	
	
	//uv_pipe_open(&pip0, 0);
	//uv_pipe_open(&pip1, 1);
	//uv_pipe_open(&pip2, 2);
	
	
	r = uv_pipe_init(loop, &pip2, 0);
	printf("r => %d\n", r);
	r = uv_pipe_init(loop, &pip3, 0);
	printf("r => %d\n", r);
	r = uv_pipe_init(loop, &pip4, 0);
	printf("r => %d\n", r);
	uv_stream_set_blocking((uv_stream_t*)&pip2, 0);
	uv_stream_set_blocking((uv_stream_t*)&pip3, 1);
	uv_stream_set_blocking((uv_stream_t*)&pip4, 1);
	
	
	
	
	printf("before spawn\n");
	
	if((r = uv_spawn(loop, &child_req, &options))){
		//printf("inner spawn\n");
		//fprintf(stderr, "spawn err: %s\n", uv_strerror(r));
		return 1;
	}else{
		fprintf(stderr, "Launched process with id %d\n", child_req.pid);
	}
	
	printf("after spawn\n");
	r = uv_pipe_open(&pip3, 3);
		if(r !=0)printf("open 3=> %s\n", uv_strerror(r));
		r = uv_pipe_open(&pip4, 4);
		if(r !=0)printf("open 4=> %s\n", uv_strerror(r));
		r = uv_write(write_req, (uv_stream_t*)&pip3 , &buf, 1, after_write);
		if(r!=0)printf("r uv write => %s\n", uv_strerror(r));
	//uv_buf_t buffer = uv_buf_init("hello_world\n", 12);
	//int written     = uv_try_write((uv_stream_t*)&pip3, &buffer, 1);
	//printf("written %d\n", written);
	
	
	
	r = uv_read_start((uv_stream_t*)&pip4, on_alloc,( uv_read_cb)on_read);
	if(r !=0)printf("r uv read start => %s\n", uv_strerror(r));
	
	
	
	return uv_run(loop, UV_RUN_DEFAULT);
}

static void on_exiti(uv_process_t* req, int64_t exit_status, int term_signal){
	fprintf(stderr, "Process exited with status %" PRId64 ", signal %d\n", exit_status, term_signal);
	uv_close((uv_handle_t*) req, NULL);
	uv_close((uv_handle_t*)&pip0, NULL);
	uv_close((uv_handle_t*)&pip1, NULL);
	uv_close((uv_handle_t*)&pip2, NULL);
	uv_close((uv_handle_t*)&pip3, NULL);
}
static void after_write(uv_write_t* req, int status){
	printf("FUCK status after write %d\n", status);
	free(req);
}
static void on_alloc(uv_handle_t* handle, size_t si, uv_buf_t* buf){
	buf->base = malloc(si);
	buf->len = si;
}
static void on_read(uv_stream_t tcp, ssize_t nread, const uv_buf_t* buf){
	printf("################ on read\n");
//	printf("buf %s\n", buf->base);
//	free(buf->base);
}
