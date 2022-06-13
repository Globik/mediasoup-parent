#include <uv.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <jansson.h>


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
static void after_write(uv_write_t*, int);

static void on_alloc(uv_handle_t*, size_t, uv_buf_t* );
static void on_read(uv_stream_t*, ssize_t, const uv_buf_t* );
static void signal_handler(uv_signal_t *, int); 
static json_t *load_json(const char*text, size_t buflen);

int main() {

    int r;
   // uv_write_t *write_req;
   // uv_buf_t buf;

    loop = uv_default_loop();



  //  buf = uv_buf_init("hello world\n", 12);
  //  write_req = malloc(sizeof(*write_req));

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
    int c = 1;
    uv_stream_set_blocking((uv_stream_t*)&pip1, c);
    uv_stream_set_blocking((uv_stream_t*)&pip2, c);
    uv_stream_set_blocking((uv_stream_t*)&pip3, c);
    uv_stream_set_blocking((uv_stream_t*)&pip4, c);
    uv_stream_set_blocking((uv_stream_t*)&pip5, c);
    uv_stream_set_blocking((uv_stream_t*)&pip6, c);


uv_signal_t sig;
uv_signal_init(loop, &sig);
uv_signal_start(&sig, signal_handler, SIGINT);// sighup sigterm sigpipe sigchld sigtstp

    printf("before spawn\n");

    if((r = uv_spawn(loop, &child_req, &options))) {
        fprintf(stderr, "spawn err: %s\n", uv_strerror(r));
        return 1;
    } else {
        fprintf(stderr, "Launched process with id %d\n", child_req.pid);
    }

    printf("after spawn\n");

    //r = uv_write(write_req, (uv_stream_t*)&pip3 , &buf, 1, after_write);
    //if(r!=0)printf("r uv write => %s\n", uv_strerror(r));

   // r = uv_read_start((uv_stream_t*)&pip2, on_alloc,( uv_read_cb)on_read);
   // if(r !=0)printf("parent uv read start => %s\n", uv_strerror(r));

    r = uv_read_start((uv_stream_t*)&pip4, on_alloc,( uv_read_cb)on_read);
    if(r !=0)printf("parent uv read start => %s\n", uv_strerror(r));

    r = uv_read_start((uv_stream_t*)&pip6, on_alloc,( uv_read_cb)on_read);
    if(r !=0)printf("parent uv read start => %s\n", uv_strerror(r));

    return uv_run(loop, UV_RUN_DEFAULT);
}

static void on_exiti(uv_process_t* req, int64_t exit_status, int term_signal) {
    fprintf(stderr, "Process exited with status %" PRId64 ", signal %d\n", exit_status, term_signal);
    printf("Process exited with status %" PRId64 ", signal %d\n", exit_status, term_signal);
    uv_close((uv_handle_t*) req, NULL);
uv_loop_close(loop);
    
}
static void after_write(uv_write_t* req, int status) {
    printf("parent status after write %d\n", status);
    free(req);
}

static void on_alloc(uv_handle_t* handle, size_t si, uv_buf_t* buf) {
    //buf->base = malloc(si);
    //buf->len = si;

    *buf = uv_buf_init((char*) malloc(si), si);
    if(buf->base == NULL) fprintf(stderr, "bufa->base is NULL");
    printf("ON ALOC PARENT %ld\n", si);

}

int a = 0;
static void on_read(uv_stream_t* tcp, ssize_t nread, const uv_buf_t* buf) {
    a++;
    printf("*******************************a: %d\n", a);
    char duffer[255];
    int offseti = 0;
    int r;
    printf("################ parent on read %d\n", buf->base[0]);
    printf("################ parent on read %d\n", buf->base[1]);
    printf("################ parent on read %d\n", buf->base[2]);
    printf("################ parent on read %d\n", buf->base[3]);
    printf("################ parent on read %d\n", buf->base[4]);
    printf("################ parent on read %c\n", buf->base[4]);
    printf("************** buf.len in parent ****** %ld\n", buf->len);
    printf("*********** nread parent *****************: %ld\n", nread);
    int num = 11;
    printf("A => %x\n", num);
    for(int i = 4; i < nread; i++) {
        char b = buf->base[i];
        offseti+= snprintf(duffer + offseti, 255 - offseti, "%c", buf->base[i]);
        //printf("Here c %c", buf->base[i]);
    }
    //duffer[6]="\0";

    printf("duffer: %s\n", duffer);
    /*
    json_t * root = load_json(buf->base, nread);
    if(root){
		char * foo = json_dumps(root, 0);
		printf("*** json *** %s\n", foo);
		//free(foo);
		json_decref(root);
	}else{
		printf("NO JANSSON ***\n");
	}
*/
json_error_t error;
json_t *root;
if(a == 1){
//json_error_t error;
//json_t *
root = json_loads(duffer, 0, &error);
if(!root) printf("NO ROOT\n");
json_t *t=json_object_get(root, "event");
const char*txt = json_string_value(t);
printf("******************** ROOT ******************* %s\n", txt);
}
if(!root){
	fprintf(stderr, "error on line %d %s\n", error.line, error.text);
}
    free(buf->base);
   // char * dura = "{\"id\":1,\"method\":\"worker.createRouter\",\"internal\":{\"routerId\":\"e2cee2ed-484e-4113-951f-61c0619ca6bd\"}}";
    //char*dura = "{\"nid\":\"f\"}";
    //uint8_t buffer[15]= { 0x0b ,0x00, 0x00, 0x00,  '{','"','n', 'i','d', '"',':','"','f','"','}' };
    char *method_str;
    method_str = strdup("worker.createRouter");
    
    json_auto_t * reply = json_object();
    json_object_set_new(reply, "id", json_integer(1));
    json_object_set_new(reply, "method", json_string(method_str));
    
    json_auto_t * intobj = json_object();
    json_object_set_new(intobj, "routerId", json_string("1"));
    
    json_object_set_new(reply, "internal", intobj);
    
    char * our = json_dumps(reply, 0);
    
    printf("Data json %s\n", our);
    
    uv_write_t *write_req;

    //size_t str_len = strlen(dura);
    size_t str_len = strlen(our);
    printf("str_len %ld\n", str_len);
    if(a == 2) return;
    size_t buf_len=str_len+sizeof(uint32_t);
    uint8_t * buffer=malloc(sizeof(buffer) *buf_len);
    if(buffer==NULL) {
        printf("buffer is null\n");
    }

    write_req = malloc(sizeof(write_req));
    if(write_req == NULL)printf("write_req null\n");
   
  // buffer[0]= 0xb;
  char details[4];
	sprintf(details, "%lx", str_len);
  char b = strtoul(details, NULL, 16);
   buffer[0]=b;
   
    buffer[1]=0x00;
    buffer[2]=0x00;
    buffer[3]=0x00;
    printf("************* BUFFER ********* %x\n", buffer[0]);
    for(int i=4; i < buf_len; i++) {
        buffer[i]=our[i-4];
    }


    uv_buf_t bufi = uv_buf_init((char*)buffer, buf_len);


    r = uv_write(write_req, (uv_stream_t*)&pip3, &bufi, 1, after_write);
    if(r !=0)fprintf(stderr, "r uv write => %s\n", uv_strerror(r));
    free(buffer);
    free(our);
free(method_str);
    //uv_read_stop(tcp);
   
}




void signal_handler(uv_signal_t *handle, int signum){
	printf("Signal received %d\n", signum);
	uv_close((uv_handle_t*)&pip0, NULL);
	uv_close((uv_handle_t*)&pip1, NULL);
uv_close((uv_handle_t*)&pip2, NULL);
    uv_close((uv_handle_t*)&pip3, NULL);
    uv_close((uv_handle_t*)&pip4, NULL);
    uv_close((uv_handle_t*)&pip5, NULL);
    uv_close((uv_handle_t*)&pip6, NULL);
    
	uv_signal_stop(handle);
	
}

static json_t *load_json(const char*text, size_t buflen){
	json_t *root;
	json_error_t error;
	root = json_loadb(text, buflen, 0, &error);
	if(root){
		return root;
	}else{
		fprintf(stderr, "json error on line %d: %s\n",error.line, error.text);
		return (json_t*)0;
	}
}
















