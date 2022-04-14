#include <uv.h>
#include <string.h>
//#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

uv_loop_t*loop;
uv_process_t child_req;
uv_process_options_t options;
uv_pipe_t pip0;
uv_pipe_t pip1;
uv_pipe_t pip2;
uv_pipe_t pip3;

char output[12];

static void on_exiti(uv_process_t*, int64_t, int);
static void after_write(uv_write_t* , int);

int main(){
	int r;
	uv_write_t *write_req;
	uv_buf_t buf;
	
	loop = uv_default_loop();
	
	
	memcmp("hello world\n", output, 12);
	buf = uv_buf_init(output, 12);
	write_req = malloc(sizeof(*write_req));
	
	char* args[2];
	args[0] = "./child";
	args[1] = NULL;
	
	uv_stdio_container_t child_stdio[4];
	child_stdio[0].flags = UV_IGNORE;
	child_stdio[1].flags = UV_IGNORE;
	child_stdio[2].flags = UV_IGNORE;
	child_stdio[3].flags = UV_CREATE_PIPE | UV_WRITABLE_PIPE;
	child_stdio[3].data.stream = (uv_stream_t*) &pip3;
	
	options.stdio = child_stdio;
	options.stdio_count = 3;
	
	options.exit_cb = on_exiti;
	options.file = "./child";
	options.args = args;
	
	
	if((r = uv_spawn(loop, &child_req, &options))){
		fprintf(stderr, "spawn err: %s\n", uv_strerror(r));
		return 1;
	}else{
		fprintf(stderr, "Launched process with id %d\n", child_req.pid);
	}
	
	r = uv_pipe_init(loop, &pip0, 1);
	printf("r => %d\n", r);
	r = uv_pipe_init(loop, &pip1, 1);
	printf("r => %d\n", r);
	r = uv_pipe_init(loop, &pip2, 1);
	printf("r => %d\n", r);
	r = uv_pipe_init(loop, &pip3, 1);
	printf("r => %d\n", r);
	
	uv_pipe_open(&pip0, 0);
	uv_pipe_open(&pip1, 1);
	uv_pipe_open(&pip2, 2);
	uv_pipe_open(&pip3, 3);
	r = uv_write(write_req, (uv_stream_t*)&pip3, &buf, 1, after_write);
	printf("r => %d\n", r);
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
	printf("status after write %d\n", status);
	free(req);
}
