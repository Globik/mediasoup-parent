#include <uv.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
//#include <glib.h>

uv_loop_t*loop;

uv_pipe_t pip0;
uv_pipe_t pip1;
uv_pipe_t pip2;
uv_pipe_t pip3;
uv_pipe_t pip4;
uv_pipe_t pip5;
uv_pipe_t pip6;

uv_process_t child_req;
uv_process_options_t options;

static void on_exiti(uv_process_t*, int64_t, int);
static void after_write(uv_write_t* , int);
static void on_alloc(uv_handle_t* , size_t , uv_buf_t* );
static void on_read(uv_stream_t*, ssize_t , const uv_buf_t* );

int main(){
	
	int r;
	uv_write_t *write_req;
	uv_buf_t buf;
	
	loop = uv_default_loop();
	
	
	
	buf = uv_buf_init("hello world\n", 12);
	write_req = malloc(sizeof(*write_req));
	
	char* args[2];
	//args[0] = "./l";
	//args[0] = "./child";
	args[0] = "./medisaoup-worker";
	args[1] = NULL;
	
	uv_stdio_container_t child_stdio[7];
	//uv_stdio_container_t child_stdio[4];
	child_stdio[0].flags = UV_IGNORE;
	child_stdio[1].flags = UV_IGNORE;
	child_stdio[2].flags = UV_INHERIT_FD;
	
	child_stdio[2].data.fd = 2;
	child_stdio[3].flags = UV_CREATE_PIPE | UV_READABLE_PIPE;
	//child_stdio[3].data.fd = 3;
	child_stdio[3].data.stream = (uv_stream_t*) &pip3;
	
	
	child_stdio[4].flags = UV_CREATE_PIPE | UV_WRITABLE_PIPE;
	child_stdio[4].data.stream = (uv_stream_t*) &pip4;
	//child_stdio[4].data.fd = 4;
	
	child_stdio[5].flags = UV_CREATE_PIPE | UV_READABLE_PIPE;
	child_stdio[5].data.stream = (uv_stream_t*) &pip5;
	
	child_stdio[6].flags = UV_CREATE_PIPE | UV_WRITABLE_PIPE;
	child_stdio[6].data.stream = (uv_stream_t*) &pip6;
	
	
	memset(&options, 0, sizeof(options));
	options.stdio = child_stdio;
	options.stdio_count = 7;
	//options.stdio_count = 4;
	
	options.exit_cb = on_exiti;
	//options.file = "./child";
	//options.file = "./l";
	
	options.file = "./mediasoup-worker";
	options.args = args;
	options.flags = 0;
	
	//char * stri[] = {"MEDIASOUP_VERSION", "3.7.6", NULL};
	//options.env = stri; 
	
	r = putenv("MEDIASOUP_VERSION=3.7.6" );
	int b = 1;
	
	r = uv_pipe_init(loop, &pip0, b);
	printf("r => %d\n", r);
	r = uv_pipe_init(loop, &pip1, b);
	printf("r => %d\n", r);
	r = uv_pipe_init(loop, &pip2, b);
	printf("r => %d\n", r);
	r = uv_pipe_init(loop, &pip3, b);
	printf("r => %d\n", r);
	r = uv_pipe_init(loop, &pip4, b);
	printf("r => %d\n", r);
	
	r = uv_pipe_init(loop, &pip5, b);
	printf("r => %d\n", r);
	r = uv_pipe_init(loop, &pip6, b);
	printf("r => %d\n", r);
	
	

	//uv_stream_set_blocking((uv_stream_t*)&pip2, 0);
	int c = 0;
	uv_stream_set_blocking((uv_stream_t*)&pip1, c);
	uv_stream_set_blocking((uv_stream_t*)&pip2, c);
	uv_stream_set_blocking((uv_stream_t*)&pip3, c);
	uv_stream_set_blocking((uv_stream_t*)&pip4, c);
	uv_stream_set_blocking((uv_stream_t*)&pip5, c);
	uv_stream_set_blocking((uv_stream_t*)&pip6, c);
	
	
	
	
	printf("before spawn\n");
	
	if((r = uv_spawn(loop, &child_req, &options))){
		fprintf(stderr, "spawn err: %s\n", uv_strerror(r));
		return 1;
	}else{
		fprintf(stderr, "Launched process with id %d\n", child_req.pid);
	}
	
	printf("after spawn\n");
	
	//r = uv_write(write_req, (uv_stream_t*)&pip3 , &buf, 1, after_write);
	//if(r!=0)printf("r uv write => %s\n", uv_strerror(r));
	
	r = uv_read_start((uv_stream_t*)&pip4, on_alloc,( uv_read_cb)on_read);
	if(r !=0)printf("parent uv read start => %s\n", uv_strerror(r));
	
	r = uv_read_start((uv_stream_t*)&pip6, on_alloc,( uv_read_cb)on_read);
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
	uv_close((uv_handle_t*)&pip5, NULL);
	uv_close((uv_handle_t*)&pip6, NULL);
}
static void after_write(uv_write_t* req, int status){
	printf("parent status after write %d\n", status);
	free(req);
}
static void on_alloc(uv_handle_t* handle, size_t si, uv_buf_t* buf){
	//buf->base = malloc(si);
	//buf->len = si;
	
	*buf = uv_buf_init((char*) malloc(si), si);
	if(buf->base == NULL) fprintf(stderr, "bufa->base is NULL");
	printf("ON ALOC PARENT %ld\n", si);
	
}
static void on_read(uv_stream_t* tcp, ssize_t nread, const uv_buf_t* buf){

	printf("################ parent on read %s\n", buf->base);
	printf("buf.len in parent  %ld\n", buf->len);
	printf("nread parent: %ld\n", nread);
	free(buf->base);
uv_read_stop(tcp);
}
