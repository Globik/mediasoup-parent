#include <glib.h>
#include <gio/gio.h>
#include <gio/gunixsocketaddress.h>
#include <errno.h>
#include <glib-unix.h>
#include <stdio.h>
#define green "\x1b[32m"
#define yellow "\x1b[33m"
#define redc "\x1b[31m"
#define rst "\x1b[0m"

GMainLoop *loop;
int a = 0;

static gboolean writ(GIOChannel *gio, GIOCondition condition, gpointer data){
	a++;
	g_print("HALLO\n");
	fprintf(stderr, "write function parent\n");
	if(a > 8)return TRUE;
	const gchar *msg = "*** Hello from parent to the child!!!! ***.\0";
	fprintf(stderr, "is_writeable in parent? => %d\n", gio->is_writeable);
	GIOStatus ret;
	GError *err = NULL;
	gsize len;
	gint leni = strlen(msg);
	g_print(green "in writ parent func\n" rst);
	if(condition & G_IO_HUP){ g_error("write end of pipe died\n"); return FALSE;}
	//g_io_channel_set_buffered(gio, FALSE); 
	//g_io_channel_set_encoding(gio, NULL, &err);
	if(err != NULL) {
		g_print("%s\n", err->message);
		g_error_free(err);
	}
	  
	ret = g_io_channel_write_chars(gio, msg, -1, &len, &err);
	if(ret == G_IO_STATUS_ERROR) g_error("error writing: %s\n", err->message);
	fprintf(stderr, "Parent wrote %lu bytes\n", len);
	//g_io_channel_flush(gio, NULL);
	return TRUE;
}

int c = 0;

static gboolean red(GIOChannel *gio, GIOCondition condition, gpointer data){
	fprintf(stderr, "reading in patent\n");
	fprintf(stdout, "reading in patent\n");
	if( c > 8) return TRUE;
	c++;
	GIOStatus ret;
	GError *err = NULL;
	gsize len;
	g_print(yellow "is_readable: %d\n" rst, gio->is_readable);
	if(condition & G_IO_HUP){ g_error("read end of pipe died\n"); return FALSE;}
if((condition & G_IO_IN) || (condition & G_IO_PRI)){
gchar buf[4096];
gsize count;
gsize bytes_read;
ret = g_io_channel_read_chars(gio, buf, sizeof(buf), &bytes_read, &err);
if(err != NULL){
	g_print(redc "read chars failed %s\n" rst, err->message);
	g_error_free(err);
	return FALSE;
	}
	if((ret == G_IO_STATUS_ERROR) || (ret == G_IO_STATUS_AGAIN)){
		g_warning( yellow "exec callback - read error [%d]\n" rst, ret);
		return FALSE;
	}
g_print(green "Some data to read in parent: %s\n" rst, buf);
}

	return TRUE;
}


int uv__nonblock_fcntl(int, int);

GSpawnChildSetupFunc child_setup(){
	fprintf(stderr, "child setup\n");
	g_print("CHILD SETUP\n");
}


GDestroyNotify destroy_notify(){
	fprintf(stderr, "destroy notify");
}

int main(int argc, char* argv[]){
g_print("GLIB VERSION: %d.%d.%d\n", GLIB_MAJOR_VERSION, GLIB_MINOR_VERSION, GLIB_MICRO_VERSION);
	
	GSubprocessLauncher *launcher = NULL;
	GSubprocess *subprocess = NULL;
	
	GError *error = NULL;
	


		//launcher = g_subprocess_launcher_new (G_SUBPROCESS_FLAGS_NONE);
		launcher = g_subprocess_launcher_new (G_SUBPROCESS_FLAGS_INHERIT_FDS);
 //launcher = g_subprocess_launcher_new (G_SUBPROCESS_FLAGS_INHERIT_FDS | G_SUBPROCESS_FLAGS_STDERR_PIPE  | G_SUBPROCESS_FLAGS_STDOUT_PIPE );//| G_SUBPROCESS_FLAGS_STDIN_PIPE );//| G_SUBPROCESS_FLAGS_STDERR_PIPE);
 // launcher = g_subprocess_launcher_new(G_SUBPROCESS_FLAGS_STDOUT_PIPE | G_SUBPROCESS_FLAGS_STDIN_PIPE | G_SUBPROCESS_FLAGS_STDOUT_PIPE);
	//launcher = g_subprocess_launcher_new(G_SUBPROCESS_FLAGS_STDOUT_PIPE /*| G_SUBPROCESS_FLAGS_STDIN_PIPE*/ | G_SUBPROCESS_FLAGS_STDERR_PIPE);
	//launcher = g_subprocess_launcher_new( G_SUBPROCESS_FLAGS_STDOUT_PIPE);
		//launcher = g_subprocess_launcher_new(G_SUBPROCESS_FLAGS_INHERIT_FDS | G_SUBPROCESS_FLAGS_STDOUT_PIPE);
	//launcher = g_subprocess_launcher_new(G_SUBPROCESS_FLAGS_STDOUT_PIPE);
	
	
	
	
	
	
	subprocess = g_subprocess_launcher_spawn(launcher, &error, "./l", NULL);
	if(error){
		g_print("err %s\n", error->message);
		g_error_free(error);
		return 1;
	}
	
	g_subprocess_launcher_set_child_setup(launcher, (GSpawnChildSetupFunc)child_setup, NULL, (GDestroyNotify)destroy_notify);
 fcntl(3,F_DUPFD, 3);
	dup2(2, 3);
  
  int fds[4];
	if(!g_unix_open_pipe(fds, FD_CLOEXEC, &error))if(error)g_error("creating pipes %s\n", error->message);
	
	
	//g_subprocess_launcher_set_child_setup(launcher, (GSpawnChildSetupFunc)child_setup, NULL, (GDestroyNotify)destroy_notify);
	
	//if(!g_unix_set_fd_nonblocking(5, TRUE, &error))g_error("failed non blockin %s\n", error->message);
	//if(!g_unix_set_fd_nonblocking(6, TRUE, &error))g_error("failed non blockin %s\n", error->message); 
	//consumer socket  #{"event":"running","targetId":7920}

	
			// fd 0 (stdin)   : Just ignore it.
            // fd 1 (stdout)  : Pipe it for 3rd libraries that log their own stuff.
            // fd 2 (stderr)  : Same as stdout.
            // fd 3 (channel) : Producer Channel fd. writes
            // fd 4 (channel) : Consumer Channel fd. reads
            // fd 5 (channel) : Producer PayloadChannel fd. writes
            // fd 6 (channel) : Consumer PayloadChannel fd  reads
       
      /* 
       GIOChannel *channel1 = g_io_channel_unix_new(0);
	if(!channel1) g_error("cannot create new giochannel\n");
	GIOChannel *channel2 = g_io_channel_unix_new(1);
	if(!channel2) g_error("cannot create new giochannel\n");
	
       GIOChannel *channel3 = g_io_channel_unix_new(2);
	if(!channel3) g_error("cannot create new giochannel\n");
       */
       GIOChannel *channel0 = g_io_channel_unix_new(0);
       if(!channel0) g_error("cannot create new giochannel\n");
       GIOChannel *channel1 = g_io_channel_unix_new(1);
      // if(!channel1) g_error("cannot create new giochannel\n");
    GIOChannel *channel2 = g_io_channel_unix_new(2);
     //  if(!channel2) g_error("cannot create new giochannel\n");
       
	
	GIOChannel *channel3 = g_io_channel_unix_new(3); // writes
	if(!channel3) g_error("cannot create new giochannel\n");
	GIOChannel *channel4 = g_io_channel_unix_new(4); // reads
	if(!channel4) g_error("cannot create new giochannel\n");
	g_io_channel_set_encoding(channel3, NULL, NULL);
	g_io_channel_set_buffered(channel3, FALSE);
	//g_io_channel_set_encoding(channel4, NULL, NULL);
	//g_io_channel_set_buffered(channel4, FALSE);
	//g_io_channel_set_flags(channel3, G_IO_FLAG_NONBLOCK, NULL); 
	//g_subprocess_launcher_take_fd(launcher, 0, 3);
	//int sa=dup2(3,3);
	g_print("dup \n");
	
	if(!g_unix_set_fd_nonblocking(0, TRUE, &error))
	g_error("failed non blockin %s\n", error->message);
	if(!g_unix_set_fd_nonblocking(1, TRUE, &error))
	g_error("failed non blockin %s\n", error->message);
	if(!g_unix_set_fd_nonblocking(2, TRUE, &error))
	g_error("failed non blockin %s\n", error->message);
	if(!g_unix_set_fd_nonblocking(3, TRUE, &error))
	g_error("failed non blockin %s\n", error->message);
	if(!g_unix_set_fd_nonblocking(4, TRUE, &error))
	g_error("failed non blockin %s\n", error->message);

	
	
	//g_data_input_stream_new (g_subprocess_get_stdout_pipe (subprocess));
	//if(!g_io_add_watch(channel1, G_IO_OUT | G_IO_HUP, writ, NULL)) g_error("cannot add watch on giochannel\n");
	//if(!g_io_add_watch(channel4, G_IO_IN | G_IO_HUP, red, NULL)) g_error("cannot add watch on giochannel\n");
	if(!g_io_add_watch(channel0, G_IO_IN | G_IO_HUP, red, NULL)) g_error("cannot add watch on giochannel\n");
	if(!g_io_add_watch(channel1, G_IO_OUT | G_IO_HUP, writ, NULL)) g_error("cannot add watch on giochannel\n");
	if(!g_io_add_watch(channel2, G_IO_IN | G_IO_HUP, red, NULL)) g_error("cannot add watch on giochannel\n");
	
	if(!g_io_add_watch(channel3, G_IO_OUT | G_IO_HUP, writ, NULL)) g_error("cannot add watch on giochannel\n");
	if(!g_io_add_watch(channel4, G_IO_IN | G_IO_HUP, red, NULL)) g_error("cannot add watch on giochannel\n");
	//if(!g_io_add_watch(channel4, G_IO_IN | G_IO_HUP, red, NULL)) g_error("cannot add watch on giochannel\n");
	//if(!g_io_add_watch(channel7, G_IO_OUT | G_IO_HUP, writ, NULL)) g_error("cannot add watch on giochannel\n");
	
	
	
	
 gchar *output2;
     gchar *error2;
     
	g_subprocess_communicate_utf8(subprocess, NULL, NULL, &output2, &error2, &error);
	g_print("error: %s\n", error2);
	g_print("Data in communicate: %s\n", output2); 
	

	
	loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);

	g_main_loop_unref(loop);
	g_object_unref(launcher);
	g_clear_object(&launcher);
	g_clear_object(&subprocess);
	return 0;
}
   
