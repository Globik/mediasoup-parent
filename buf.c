#include <uv.h>
#include <assert.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>

uv_process_t process;
uv_process_options_t options;

static void on_exiti(uv_process_t*, int64_t, int);
static void write_cb(uv_write_t* , int);
static void on_alloc(uv_handle_t* , size_t , uv_buf_t* );
static void on_read(uv_stream_t*, ssize_t , const uv_buf_t* );


int main(){
	int r;
	uv_pipe_t out;
	uv_pipe_t in;
	uv_write_t write_req;
	uv_buf_t buf;
	uv_stdio_container_t stdio[2];
	uv_pipe_init(uv_default_loop(), &out, 0);
	uv_pipe_init(uv_default_loop(), &in, 0);
	char* args[2];
	args[0] = "./ls";
	args[1] = NULL;
	options.stdio = stdio;
	options.stdio_count = 2;
	options.exit_cb = on_exiti;
	options.file = "ls";
	options.args = args;
	options.flags = 0;
	
	options.stdio[0].flags = UV_CREATE_PIPE | UV_READABLE_PIPE;
	options.stdio[0].data.stream = (uv_stream_t*) &in;
	options.stdio[1].flags = UV_CREATE_PIPE | UV_WRITABLE_PIPE;
	options.stdio[1].data.stream = (uv_stream_t*) &out;
	
	char buffer[] = "hello from spawn stdin";
	
	r = uv_spawn(uv_default_loop(), &process, &options);
	assert(r == 0);
	buf.base = buffer;
	buf.len = sizeof(buffer);
	r = uv_write(&write_req, (uv_stream_t*) &in, &buf, 1, write_cb);
	assert(r == 0);
	r = uv_read_start((uv_stream_t*) &out, on_alloc, ( uv_read_cb)on_read);
	assert(r == 0);
	r = uv_run(uv_default_loop(), UV_RUN_DEFAULT);
	assert( r == 0);
	return 0;
}




static void on_exiti(uv_process_t* req, int64_t exit_status, int term_signal){
	fprintf(stderr, "Process exited with status %" PRId64 ", signal %d\n", exit_status, term_signal);
	uv_close((uv_handle_t*) req, NULL);
}
static void write_cb(uv_write_t* req, int status){
	printf("FUCK status after write %d\n", status);
	//free(req);
}
static void on_alloc(uv_handle_t* handle, size_t si, uv_buf_t* buf){
	buf->base = malloc(si);
	buf->len = si;
	
//	*buf = uv_buf_init((char*) malloc(si), si);
//	if(buf->base == NULL) fprintf(stderr, "bufa->base is NULL");
	printf("ON ALOC PARENT\n");
	
}
static void on_read(uv_stream_t *tcp, ssize_t nread, const uv_buf_t* buf){
	printf("on read: \n");
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
	//printf("################ on read %s\n", buf->base);
	
     printf("buf in parent  %s\n", buf->base);
     printf("len %d\n", (int)buf->len);
	free(buf->base);
uv_read_stop(tcp);
}





































