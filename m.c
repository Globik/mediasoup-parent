#include <glib.h>
#include <gio/gio.h>
#include <gio/gunixsocketaddress.h>
#include <errno.h>
#include <glib-unix.h>
GMainLoop *loop;
static void child_watch_cb(GPid pid, gint status, gpointer user_data);

int a = 0;
static gboolean writ(GIOChannel *gio, GIOCondition condition, gpointer data){
	a++;
	g_print("!!!! gio is writeable? !!!! %d\n", gio->is_writeable);
	if(a > 3)return FALSE;
	const gchar *msg = "****** The Fucker from parent ************* ";
	GIOStatus ret;
	GError *err = NULL;
	gsize len;
	g_print("in write func PARENT\n");
	if(condition & G_IO_HUP){ g_error("write end of pipe died\n"); return FALSE;}
	//g_io_channel_set_buffered(gio, FALSE); 
	//g_io_channel_set_encoding(gio, NULL, &err);
	if(err) g_print("%s\n", err->message);
	  
	ret = g_io_channel_write_chars(gio, msg, -1, &len, &err);
	if(ret == G_IO_STATUS_ERROR) g_error("error writing: %s\n", err->message);
	g_print("wrote %lu bytes\n", len);
	return TRUE;
}

static gboolean red(GIOChannel *gio, GIOCondition condition, gpointer data){
	g_print("alik ept\n");
	a++;
	if(a > 6) return FALSE;
	g_print("parent gio is readable? %d\n", gio->is_readable);
	//if(!gio->is_readable){
		g_print("o fuck is readable\n"); 
		//return FALSE;
		
	GError *err = NULL;
	//gsize len;
	g_print("in readi func\n");
	if(condition & G_IO_HUP){ g_error("read end of pipe died\n"); 
		return FALSE;
		}

gchar buf[4096];

gsize bytes_read;
//g_io_channel_set_encoding(gio, NULL, &err);
g_io_channel_read_chars(gio, buf, sizeof(buf), &bytes_read, &err);
if(err){g_print("read chars failed %s\n", err->message);}
g_print("some data to read in the : %s\n", buf);
g_print("length %lu\n", bytes_read);


	
return TRUE;
}

void set_child(){
	g_print("Hello set_child!\n");
	g_setenv("MEDIASOUP_VERSION", "3.7.6", TRUE);
	int fds[4];
	dup2(3,3);
	/*
	if(!g_unix_open_pipe(fds, FD_CLOEXEC, NULL)){g_error("creating pipes error\n");}
	GIOChannel *channel3= g_io_channel_unix_new(3);
	if(!channel3) g_error("cannot create new giochannel\n");
	
	g_io_channel_set_flags(channel3, G_IO_FLAG_NONBLOCK, NULL); 
	if(!g_io_add_watch(channel3, G_IO_OUT | G_IO_HUP, writ, NULL)) g_error("cannot add watch on giochannel\n");
	*/ 
}
//client->in = g_data_output_stream_new (g_subprocess_get_stdin_pipe (subprocess));
 // client->out = g_data_input_stream_new (g_subprocess_get_stdout_pipe (subprocess));

//g_spawn_async_with_pipes_and_fds();
int main(int argc, char* argv[]){
	// G_SPAWN_LEAVE_DESCRIPTORS_OPEN G_SPAWN_DO_NOT_REAP_CHILD G_SPAWN_CHILD_INHERITS_STDIN G_SPAWN_CLOEXEC_PIPES  | G_SPAWN_SEARCH_PATH 
	GSpawnFlags flags = G_SPAWN_LEAVE_DESCRIPTORS_OPEN ;// |  G_SPAWN_DO_NOT_REAP_CHILD ;//| G_SPAWN_CLOEXEC_PIPES;// | G_SPAWN_CHILD_INHERITS_STDIN;
	GError *error = NULL;
	const gint s;
	const gint g;
	GPid pid;
	// gchar * argvi[] = { "./mediasoup-worker", NULL};
	gchar * argvi[] = { "./child", NULL};
	
	gint stdin_pipe; //*stdin pipe,
	gint stdout_pipe; //*stdout pipe,
	gint stderr_pipe;
	// GSubprocessLauncher *launcher = NULL;
	//GSubprocess *subprocess = NULL;
	//g_spawn_async_with_pipes_and_fds();
	//launcher = g_subprocess_launcher_new (G_SUBPROCESS_FLAGS_NONE);
	//g_subprocess_launcher_setenv(launcher, "MEDIASOUP_VERSION", "3.7.6", TRUE);
	//g_setenv("MEDIASOUP_VERSION", "3.7.6", TRUE);
	g_spawn_async_with_pipes("/home/globi/mediasoup-parent", argvi, NULL, flags,(GSpawnChildSetupFunc) set_child,NULL, &pid, /*&stdin_pipe*/ NULL, /*&stdout_pipe*/ NULL,/* &stderr_pipe*/NULL, &error);
	if(error){
		g_print("%s\n", error->message);
		return 1;
	}
	//g_spawn_async_with_pipes_and_fds("/home/globi/mediasoup-parent", argvi, NULL, flags, (GSpawnChildSetupFunc) set_child, NULL, 0, 1, 2,3,4,5,6, NULL, &stdout_pipe, &stderr_pipe, &error);
	g_print("pid: %d\n",  pid);
	//g_print("stdin_%d\n", stdin_pipe);
	//g_print("stdout %d\n", stdout_pipe);
	//g_print("stderr %d\n", stderr_pipe);
	
	int fds[4];
	if(!g_unix_open_pipe(fds, FD_CLOEXEC, &error)){g_error("creating pipes %s\n", error->message);}
	
	
	
	GIOChannel *channel3= g_io_channel_unix_new(3);if(!channel3) g_error("cannot create new giochannel\n");
	
	g_io_channel_set_flags(channel3, G_IO_FLAG_NONBLOCK, NULL); 
	

GIOChannel *channel0= g_io_channel_unix_new(/*stdin_pipe*/0);if(!channel0) g_error("cannot create new giochannel\n");
	
	GIOChannel *channel1= g_io_channel_unix_new(/*stdout_pipe*/1);if(!channel1) g_error("cannot create new giochannel\n");
	GIOChannel *channel2= g_io_channel_unix_new(/*stderr_pipe*/2);if(!channel2) g_error("cannot create new giochannel\n");
	g_io_channel_set_flags(channel0, G_IO_FLAG_NONBLOCK, NULL); 
	g_io_channel_set_flags(channel1, G_IO_FLAG_NONBLOCK, NULL); 
	g_io_channel_set_flags(channel2, G_IO_FLAG_NONBLOCK, NULL); 
	dup2(3, 3);
if(!g_io_add_watch(channel0, G_IO_IN | G_IO_HUP, red, NULL)) g_error("cannot add watch on giochannel\n");
if(!g_io_add_watch(channel1, G_IO_IN | G_IO_HUP, red, NULL)) g_error("cannot add watch on giochannel\n");
if(!g_io_add_watch(channel2, G_IO_OUT | G_IO_HUP, writ, NULL)) g_error("cannot add watch on giochannel\n");
if(!g_io_add_watch(channel3, G_IO_OUT | G_IO_HUP, writ, NULL)) g_error("cannot add watch on giochannel\n");

//if(!g_io_add_watch(channel5, G_IO_OUt | G_IO_HUP, writ, NULL)) g_error("cannot add watch on giochannel\n");
	
	g_child_watch_add(pid, child_watch_cb, NULL);
	loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);
	return 0;
}
static void child_watch_cb(GPid pid, gint status, gpointer user_data){
	g_print("hello watch cb!\n");
	g_spawn_close_pid(pid);
}
