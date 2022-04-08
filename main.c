#include <glib.h>
#include <gio/gio.h>
#include <gio/gunixsocketaddress.h>
#include <errno.h>
#include <glib-unix.h>

#define green "\x1b[32m"
#define yellow "\x1b[33m"
#define redc "\x1b[31m"
#define rst "\x1b[0m"

GMainLoop *loop;
int a = 0;

static gboolean writ(GIOChannel *gio, GIOCondition condition, gpointer data){
	a++;
	if(a > 8)return TRUE;
	const gchar *msg = "*** Hello from parent to the child!!!! ***.\0";
	g_print("is_writeable? => %d\n", gio->is_writeable);
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
	g_print("wrote %lu bytes\n", len);
	g_io_channel_flush(gio, NULL);
	return TRUE;
}

int c = 0;

static gboolean red(GIOChannel *gio, GIOCondition condition, gpointer data){
	
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


int main(int argc, char* argv[]){
	g_print("alles ok\n");
	
	GSubprocessLauncher *launcher = NULL;
	GSubprocess *subprocess = NULL;
	GInputStream *input_stream = NULL;
	GDataInputStream *data_stream = NULL;
	GError *error = NULL;
	


//launcher = g_subprocess_launcher_new (G_SUBPROCESS_FLAGS_NONE);
launcher = g_subprocess_launcher_new (G_SUBPROCESS_FLAGS_INHERIT_FDS);
//launcher = g_subprocess_launcher_new (G_SUBPROCESS_FLAGS_INHERIT_FDS | G_SUBPROCESS_FLAGS_STDOUT_PIPE | G_SUBPROCESS_FLAGS_STDIN_PIPE | G_SUBPROCESS_FLAGS_STDERR_PIPE);
//launcher = g_subprocess_launcher_new(G_SUBPROCESS_FLAGS_STDOUT_PIPE | G_SUBPROCESS_FLAGS_STDIN_PIPE | G_SUBPROCESS_FLAGS_STDOUT_PIPE);
//launcher = g_subprocess_launcher_new(G_SUBPROCESS_FLAGS_STDOUT_PIPE | G_SUBPROCESS_FLAGS_STDIN_PIPE );
//launcher = g_subprocess_launcher_new( G_SUBPROCESS_FLAGS_STDIN_PIPE);
//launcher = g_subprocess_launcher_new(G_SUBPROCESS_FLAGS_INHERIT_FDS | G_SUBPROCESS_FLAGS_STDOUT_PIPE);
	g_subprocess_launcher_setenv(launcher, "MEDIASOUP_VERSION", "3.7.6", TRUE);
	
	//subprocess = g_subprocess_launcher_spawn(launcher, &error, "./mediasoup-worker", NULL);
	subprocess = g_subprocess_launcher_spawn(launcher, &error, "./child", NULL);
	if(error){
		g_print("err %s\n", error->message);
		//g_error_free(error);
		return 1;
	}
	int fds[1];
	if(!g_unix_open_pipe(fds, FD_CLOEXEC, &error))if(error)g_error("creating pipes %s\n", error->message);
	//g_error_free(error);
	
	g_subprocess_launcher_take_stdin_fd(launcher, 3); //3 4 5 6 
	
	g_subprocess_launcher_take_stdout_fd(launcher, 3);
	//g_subprocess_launcher_take_stderr_fd(launcher, 3);//etwas no data
	//g_subprocess_launcher_take_stdout_fd(launcher, 4);
	
	//g_subprocess_launcher_take_stdin_fd(launcher, 4); //3 4 5 6 
	//g_subprocess_launcher_take_fd(launcher, 0, 3);
	//g_subprocess_launcher_take_fd(launcher, 1, 4);
	//g_subprocess_launcher_take_stdout_fd(launcher, 4);
	//g_subprocess_launcher_take_stderr_fd(launcher, 4);
	
	
	
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
	//GIOChannel *channel4 = g_io_channel_unix_new(4); // reads
	//if(!channel4) g_error("cannot create new giochannel\n");
	//g_io_channel_set_encoding(channel3, "UTF-8", NULL);
	//g_io_channel_set_buffered(channel3, FALSE);
	g_io_channel_set_flags(channel3, G_IO_FLAG_NONBLOCK, NULL); 
	
	if(!g_unix_set_fd_nonblocking(0, TRUE, &error))
	g_error("failed non blockin %s\n", error->message);
	if(!g_unix_set_fd_nonblocking(1, TRUE, &error))
	g_error("failed non blockin %s\n", error->message);
	if(!g_unix_set_fd_nonblocking(2, TRUE, &error))
	g_error("failed non blockin %s\n", error->message);
	if(!g_unix_set_fd_nonblocking(3, TRUE, &error))
	g_error("failed non blockin %s\n", error->message);
	//if(!g_unix_set_fd_nonblocking(4, TRUE, &error))
	//g_error("failed non blockin %s\n", error->message);
//	GIOChannel *channel5 = g_io_channel_unix_new(5);
	//if(!channel5) g_error("cannot create new giochannel\n");
	
//	GIOChannel *channel6 = g_io_channel_unix_new(6);
//	if(!channel6) g_error("cannot create new giochannel\n");
//	GIOChannel *channel7 = g_io_channel_unix_new(7);
	//if(!channel7) g_error("cannot create new giochannel\n");
	
	//g_data_input_stream_new (g_subprocess_get_stdout_pipe (subprocess));
	//if(!g_io_add_watch(channel1, G_IO_OUT | G_IO_HUP, writ, NULL)) g_error("cannot add watch on giochannel\n");
	//if(!g_io_add_watch(channel4, G_IO_IN | G_IO_HUP, red, NULL)) g_error("cannot add watch on giochannel\n");
	//if(!g_io_add_watch(channel0, G_IO_IN | G_IO_HUP, red, NULL)) g_error("cannot add watch on giochannel\n");
	//if(!g_io_add_watch(channel1, G_IO_OUT | G_IO_HUP, writ, NULL)) g_error("cannot add watch on giochannel\n");
	//if(!g_io_add_watch(channel2, G_IO_OUT | G_IO_HUP, writ, NULL)) g_error("cannot add watch on giochannel\n");
	if(!g_io_add_watch(channel3, G_IO_OUT | G_IO_HUP, writ, NULL)) g_error("cannot add watch on giochannel\n");
	
	//if(!g_io_add_watch(channel6, G_IO_IN | G_IO_HUP, red, NULL)) g_error("cannot add watch on giochannel\n");
	//if(!g_io_add_watch(channel7, G_IO_OUT | G_IO_HUP, writ, NULL)) g_error("cannot add watch on giochannel\n");
	
	//const gchar *msg = "the fucker the fuckerthe fuckerthe fuckerthe fuckerthe fuckerthe fuckerthe fuckerthe fuckerthe fuckerthe fuckerthe fuckerthe fuckerthe fuckerthe fuckervthe fuckerthe fucker";
	
	//GError *err = NULL;
	
	//g_print("producer_channel is writable: %d\n", producer_channel->is_writeable);
	//g_print("consumer_channel is writable: %d\n", consumer_channel->is_writeable);
	//g_print("producer_payload is writable: %d\n", producer_payload->is_writeable);
	//g_print("consumer payload is writable: %d\n", consumer_payload->is_writeable);
	
//g_timeout_add_seconds(1, (GSourceFunc) persona_active_cb, channel3);
	
	
	
GBytes* stdout_buf;
	GBytes* stderr_buf;
	GBytes* stdin_buf;

 gchar *output2;
     gchar *error2;
     
	//g_subprocess_communicate_utf8(subprocess, NULL, NULL, &output2, &error2, &error);
	//g_print("error: %s\n", error2);
	//g_print("Data in communicate: %s\n", output2); 
	GOutputStream * stream = NULL;


if(error){g_print("some error: %s\n", error->message);}


if(error){
	
	g_print("uhu %s\n", error->message);
}

	
	loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);
	//ret = g_io_channel_write_chars(consumer_channel, msg, -1, &len, &error);
	//if(ret == G_IO_STATUS_ERROR) g_error("error writing: %s\n", error->message);
	//g_print("wrote %u bytes\n", len);
	//g_io_channel_unref(mychannel);
	g_main_loop_unref(loop);
	g_object_unref(launcher);
	g_clear_object(&launcher);
	g_clear_object(&subprocess);
	return 0;
}
