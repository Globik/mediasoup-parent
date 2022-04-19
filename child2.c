#include <uv.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>

uv_loop_t *loop;
uv_pipe_t pipi;
uv_pipe_t pipi2;
uv_pipe_t pipi3, pipi4;
static char output[1024];

static void on_alloc(uv_handle_t* handle, size_t si, uv_buf_t* );
static void on_read(uv_stream_t tcp, ssize_t nread, const uv_buf_t* );
static void after_write(uv_write_t* req, int status);

int main(){
	printf("child\n");
	int r;
	//uv_write_t *write_req;
	uv_buf_t buf;
	
	loop = uv_default_loop();
	r = uv_pipe_init(loop, &pipi, 1);
	if(r !=0)fprintf(stderr, "r => %s\n", uv_strerror(r));
	//r = uv_pipe_open(&pipi, 3);
	if(r !=0)fprintf(stderr, "r => %s\n", uv_strerror(r));
	
	r = uv_pipe_init(loop, &pipi4, 1);
	if(r !=0)fprintf(stderr, "r => %s\n", uv_strerror(r));
	r = uv_pipe_open(&pipi, 4);
	if(r !=0)fprintf(stderr, "r => %s\n", uv_strerror(r));
	
	r = uv_pipe_init(loop, &pipi2, 1);
	if(r !=0)fprintf(stderr, "r => %s\n", uv_strerror(r));
	r = uv_pipe_open(&pipi2, 4);
	if(r !=0)fprintf(stderr, "r => %s\n", uv_strerror(r));
	
	//r = uv_read_start((uv_stream_t*)&pipi, on_alloc,( uv_read_cb)on_read);
	if(r !=0)fprintf(stderr, "r uv read start => %s\n", uv_strerror(r));
	
	
	r = uv_read_start((uv_stream_t*)&pipi3, on_alloc,( uv_read_cb)on_read);
	if(r !=0)printf("r uv read start => %s\n", uv_strerror(r));
	
	
	
	
	
	uv_write_t *write_req;
	write_req = malloc(sizeof(*write_req));
	uv_buf_t bufi = uv_buf_init("b\n", 1);
	
	r = uv_write(write_req, (uv_stream_t*)&pipi4 , &bufi, 1, after_write);
	if(r !=0)fprintf(stderr, "r uv write => %s\n", uv_strerror(r));
	return uv_run(loop, UV_RUN_DEFAULT);
}
static void on_alloc(uv_handle_t* handle, size_t si, uv_buf_t* bufa){
	fprintf(stderr, "si: %ld\n", si);
	//buf=malloc(sizeof(*buf));
	bufa->base = malloc(si);
	
	bufa->len = si;
//	fprintf(stderr, "buf %s\n", buf->base);
	// bufa = uv_buf_init((char*) malloc(100), 100);
	// buf->base="unit\n";
}

int a = 0;
static void on_read(uv_stream_t tcp, ssize_t nread, const uv_buf_t *bu){
	
	a++;
	
	fprintf(stderr, "data len: %ld\n", nread);
	fprintf(stderr, "buf %s\n", bu->base);
	//doc(buf);
	//free(buf->base);
	//free(buf);
	//if(bu->len)fprintf(stderr, "buffer len: %ld\n", bu->len);
	
	
	//free(&bufi.base);
}
static void after_write(uv_write_t* req, int status){
	fprintf(stderr, "status after write %d\n", status);
	free(req);
}
