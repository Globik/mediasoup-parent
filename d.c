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
	
	if(a > 3)return TRUE;
	const gchar *msg = "*** Hello from parent to the child!!!! ***.\0";
	g_print("is_writeable in parent? => %d\n", gio->is_writeable);
	if(!gio->is_writeable) return FALSE;
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
	if(ret == G_IO_STATUS_EOF) g_print("it look like socket closed fern\n");
	if(ret == G_IO_STATUS_ERROR) {
		g_error("error writing: %s\n", err->message);
		return FALSE;
	}
	g_print("Parent wrote %lu bytes\n", len);
	g_io_channel_flush(gio, NULL);
	return TRUE;
}

int c = 0;

static gboolean red(GIOChannel *gio, GIOCondition condition, gpointer data){
	
	//if( c > 8) return TRUE;
	c++;
	GIOStatus ret;
	GError *err = NULL;
	gsize len;
	g_print(yellow "is_readable: %d\n" rst, gio->is_readable);
	if(!gio->is_readable) return FALSE;
	if(condition & G_IO_HUP){ g_error("read end of pipe died\n"); return FALSE;}
if((condition & G_IO_IN) || (condition & G_IO_PRI)){
gchar buf[4096];
gsize count;
gsize bytes_read;

	 while( G_IO_STATUS_AGAIN == (ret = g_io_channel_read_chars( gio, buf, sizeof(buf), &bytes_read, &err ))) {continue;}
	 if(err){
	g_print(redc "read chars failed %s\n" rst, err->message);
	g_error_free(err);
	return FALSE;
	}
	//g_io_channel_set_buffered(gio, FALSE);
	//g_io_channel_set_encoding(gio, "UTF-8", &err);
	//g_io_channel_set_buffered(gio, FALSE);
	if(err){
		g_print("%s\n", err->message);
	}
	g_print(green "Some data to read in parent: %s\n" rst, buf);
	g_print("len: %lu\n", bytes_read);
	if(ret==G_IO_STATUS_EOF){g_print("It seems shutup end\n");}
	if((ret == G_IO_STATUS_ERROR) || (ret == G_IO_STATUS_AGAIN)){
		g_print("io status error %d\n", G_IO_STATUS_ERROR);
		g_print("io status again %d\n", G_IO_STATUS_AGAIN);
		g_warning(  "exec callback - read error [%d] \n", ret);
		//return TRUE;
		
	}
	//buf[sizeof(buf)]="\0";
g_print(green "Some data to read in parent: %s\n" rst, buf);
g_io_channel_flush(gio, NULL);
}

	return TRUE;
}


int main(int argc, char* argv[]){
	g_print("alles ok\n");
	
	GSubprocessLauncher *launcher = NULL;
	GSubprocess *subprocess = NULL;
	
	GError *error = NULL;
	
	
	
	GSpawnFlags flags = G_SUBPROCESS_FLAGS_NONE;//mjiji
	//GSpawnFlags flags = G_SUBPROCESS_FLAGS_INHERIT_FDS;
	//GSpawnFlags flags = G_SUBPROCESS_FLAGS_INHERIT_FDS | G_SUBPROCESS_FLAGS_STDOUT_PIPE | G_SUBPROCESS_FLAGS_STDIN_PIPE | G_SUBPROCESS_FLAGS_STDERR_PIPE;
	//GSpawnFlags flags = G_SUBPROCESS_FLAGS_STDOUT_PIPE | G_SUBPROCESS_FLAGS_STDIN_PIPE | G_SUBPROCESS_FLAGS_STDERR_PIPE;
	//GSpawnFlags flags = G_SUBPROCESS_FLAGS_STDOUT_PIPE | G_SUBPROCESS_FLAGS_STDERR_PIPE;
	//GSpawnFlags flags = G_SUBPROCESS_FLAGS_STDOUT_PIPE | G_SUBPROCESS_FLAGS_STDIN_PIPE;
	//GSpawnFlags flags = G_SUBPROCESS_FLAGS_STDIN_PIPE;
	//GSpawnFlags flags = G_SUBPROCESS_FLAGS_INHERIT_FDS | G_SUBPROCESS_FLAGS_STDIN_PIPE;
	//GSpawnFlags flags = G_SUBPROCESS_FLAGS_INHERIT_FDS | G_SUBPROCESS_FLAGS_STDOUT_PIPE;
	//GSpawnFlags flags = G_SUBPROCESS_FLAGS_INHERIT_FDS | G_SUBPROCESS_FLAGS_STDERR_PIPE;// | G_SUBPROCESS_FLAGS_STDIN_PIPE;
	
	launcher = g_subprocess_launcher_new (flags);
	g_subprocess_launcher_setenv(launcher, "MEDIASOUP_VERSION", "3.7.6", TRUE);
	//g_subprocess_launcher_setenv(launcher, "MEDIASOUP_BUILDTYPE", "Debug", TRUE);
	
	subprocess = g_subprocess_launcher_spawn(launcher, &error, "./mediasoup-worker", "--logLevel=dedug", NULL);
	
	if(error){
		g_print("err %s\n", error->message);
		g_error_free(error);
		return 1;
	}
	int fds[7];
	if(!g_unix_open_pipe(fds, FD_CLOEXEC, &error))if(error)g_error("creating pipes %s\n", error->message);
	
	 GIOChannel *channel0 = g_io_channel_unix_new(0); if(!channel0) g_error("cannot create new giochannel\n");
	 GIOChannel *channel1 = g_io_channel_unix_new(1); if(!channel1) g_error("cannot create new giochannel\n");
	 GIOChannel *channel2 = g_io_channel_unix_new(2); if(!channel2) g_error("cannot create new giochannel\n");
	 
	 GIOChannel *channel3 = g_io_channel_unix_new(3); if(!channel3) g_error("cannot create new giochannel\n");
	 GIOChannel *channel4 = g_io_channel_unix_new(4); if(!channel4) g_error("cannot create new giochannel\n");
	 
	// GIOChannel *channel5 = g_io_channel_unix_new(5); if(!channel5) g_error("cannot create new giochannel\n");
	 GIOChannel *channel6 = g_io_channel_unix_new(6); if(!channel6) g_error("cannot create new giochannel\n");
	 /*
	 if(!g_unix_set_fd_nonblocking(0, TRUE, &error)) g_error("failed non blockin 0 %s\n", error->message);
	 if(!g_unix_set_fd_nonblocking(1, TRUE, &error)) g_error("failed non blockin 1 %s\n", error->message);
	 if(!g_unix_set_fd_nonblocking(2, TRUE, &error)) g_error("failed non blockin 2 %s\n", error->message);
	 
	 if(!g_unix_set_fd_nonblocking(3, TRUE, &error)) g_error("failed non blockin 3 %s\n", error->message);
	 if(!g_unix_set_fd_nonblocking(4, TRUE, &error)) g_error("failed non blockin 4 %s\n", error->message);
	 
	 if(!g_unix_set_fd_nonblocking(5, TRUE, &error)) g_error("failed non blockin 5 %s\n", error->message);
	 if(!g_unix_set_fd_nonblocking(6, TRUE, &error)){ g_error("failed non blockin 6 %s\n", error->message); return 1;}
	 */
	 /*
	 g_io_channel_set_encoding(channel3, NULL, NULL);
	 g_io_channel_set_buffered(channel3, FALSE);
	 
	 g_io_channel_set_encoding(channel4, NULL, NULL);
	 g_io_channel_set_buffered(channel4, FALSE);
	 
	 g_io_channel_set_encoding(channel5, NULL, NULL);
	 g_io_channel_set_buffered(channel5, FALSE);
	 
	 g_io_channel_set_encoding(channel6, NULL, NULL);
	 g_io_channel_set_buffered(channel6, FALSE);*/
	 gsize len;
	 const char * msg = "hello world";
	/* 
	 g_io_channel_write_chars(channel3, msg, -1, &len, &error);
	 if(error){
		 g_print("err %s\n", error->message);
	 }
	 */
	 gchar *output2;
     gchar *error2;
     
	g_subprocess_communicate_utf8(subprocess, NULL, NULL, &output2, &error2, &error);
	g_print("error: %s\n", error2);
	g_print("Data in communicate: %s\n", output2); 
	/* 
	if(!g_io_add_watch(channel0, G_IO_IN | G_IO_HUP, red, NULL)) g_error("cannot add watch on giochannel\n");//t
	if(!g_io_add_watch(channel1, G_IO_OUT | G_IO_HUP, writ, NULL)) g_error("cannot add watch on giochannel\n");
	 if(!g_io_add_watch(channel2, G_IO_IN | G_IO_HUP, red, NULL)) g_error("cannot add watch on giochannel\n");
	 
	if(!g_io_add_watch(channel3, G_IO_OUT | G_IO_HUP, writ, NULL)) g_error("cannot add watch on giochannel\n");
	 if(!g_io_add_watch(channel4, G_IO_IN | G_IO_HUP, red, NULL)) g_error("cannot add watch on giochannel\n");
	 
	if(!g_io_add_watch(channel5, G_IO_OUT | G_IO_HUP, writ, NULL)) g_error("cannot add watch on giochannel\n");
	 if(!g_io_add_watch(channel6, G_IO_OUT | G_IO_HUP, writ, NULL)) g_error("cannot add watch on giochannel\n");
	 */
	loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);
	g_main_loop_unref(loop);
	g_main_loop_quit(loop);
	g_object_unref(launcher);
	g_clear_object(&launcher);
	g_clear_object(&subprocess);
	g_print("*** Bye ***\n");
	return 0;
}
