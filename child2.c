// make u

#include <uv.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>

uv_loop_t *loop;
uv_pipe_t pipi0;
uv_pipe_t pipi1;
uv_pipe_t pipi2, pipi3,pipi4;
//uv_buf_t buf;
uv_loop_t*loop;

static void on_alloc(uv_handle_t* handle, size_t si, uv_buf_t* );
static void on_read(uv_stream_t *tcp, ssize_t nread, const uv_buf_t* );
static void after_write(uv_write_t* req, int status);

int main(){
	fprintf(stderr, "child2\n");
	int r;
	//uv_write_t *write_req;
	//uv_buf_t buf;
	
	loop = uv_default_loop();
	/*r = uv_pipe_init(loop, &pipi, 1);
	if(r !=0)fprintf(stderr, "r => %s\n", uv_strerror(r));
	//r = uv_pipe_open(&pipi, 3);
	if(r !=0)fprintf(stderr, "r => %s\n", uv_strerror(r));
	*/
	r = uv_pipe_init(loop, &pipi3, 1);
	if(r !=0)fprintf(stderr, "r => %s\n", uv_strerror(r));
	r = uv_pipe_open(&pipi3, 3);
	if(r !=0)fprintf(stderr, "r => %s\n", uv_strerror(r));
	
	r = uv_pipe_init(loop, &pipi4, 1);
	if(r !=0)fprintf(stderr, "r => %s\n", uv_strerror(r));
	r = uv_pipe_open(&pipi4, 4);
	if(r !=0)fprintf(stderr, "r => %s\n", uv_strerror(r));

	
	
	
	uv_write_t *write_req;
	write_req = malloc(sizeof(*write_req));
	uv_buf_t bufi = uv_buf_init("fuck", 4);
	
	r = uv_write(write_req, (uv_stream_t*)&pipi4 , &bufi, 1, after_write);
	if(r !=0)fprintf(stderr, "r uv write => %s\n", uv_strerror(r));
	
	
	r = uv_read_start((uv_stream_t*)&pipi3, on_alloc,( uv_read_cb)on_read);
	if(r !=0)printf("child uv read start => %s\n", uv_strerror(r));
	
	return uv_run(loop, UV_RUN_DEFAULT);
}
static void on_alloc(uv_handle_t* handle, size_t si, uv_buf_t* buf){
	fprintf(stderr, "si: %ld\n", si);
	*buf = uv_buf_init((char*) malloc(si), si);
	if(buf->base == NULL) fprintf(stderr, "bufa->base is NULL");
}

int a = 0;
  static void on_read(uv_stream_t *tcp, ssize_t nread, const uv_buf_t *buf){
	
	a++;
	fprintf(stderr, "on_red child\n");
	fprintf(stderr, "child data len: %ld\n", nread);
	fprintf(stderr, "buf %ld\n", buf->len);
	fprintf(stderr, "################## child buf %s\n", buf->base);
	
	free(buf->base);
	
	uv_read_stop(tcp);
}
static void after_write(uv_write_t* req, int status){
	fprintf(stderr, "child status after write %d\n", status);
	free(req);
}
