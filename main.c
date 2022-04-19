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
	if(a > 8)return TRUE;
	const gchar *msg = "*** Hello from parent to the child!!!! ***.\0";
	g_print("is_writeable in parent? => %d\n", gio->is_writeable);
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
	g_print("Parent wrote %lu bytes\n", len);
	//g_io_channel_flush(gio, NULL);
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


int uv__nonblock_fcntl(int, int);

int main(int argc, char* argv[]){

	
	GSubprocessLauncher *launcher = NULL;
	GSubprocess *subprocess = NULL;
	
	GError *error = NULL;
	


		launcher = g_subprocess_launcher_new (G_SUBPROCESS_FLAGS_NONE);
		//launcher = g_subprocess_launcher_new (G_SUBPROCESS_FLAGS_INHERIT_FDS);
 // launcher = g_subprocess_launcher_new (G_SUBPROCESS_FLAGS_INHERIT_FDS | G_SUBPROCESS_FLAGS_STDOUT_PIPE | G_SUBPROCESS_FLAGS_STDIN_PIPE | G_SUBPROCESS_FLAGS_STDERR_PIPE);
  //launcher = g_subprocess_launcher_new(G_SUBPROCESS_FLAGS_STDOUT_PIPE | G_SUBPROCESS_FLAGS_STDIN_PIPE | G_SUBPROCESS_FLAGS_STDOUT_PIPE);
	//launcher = g_subprocess_launcher_new(G_SUBPROCESS_FLAGS_STDOUT_PIPE /*| G_SUBPROCESS_FLAGS_STDIN_PIPE*/ | G_SUBPROCESS_FLAGS_STDERR_PIPE);
	//launcher = g_subprocess_launcher_new( G_SUBPROCESS_FLAGS_STDIN_PIPE);
		//launcher = g_subprocess_launcher_new(G_SUBPROCESS_FLAGS_INHERIT_FDS | G_SUBPROCESS_FLAGS_STDOUT_PIPE);
		//launcher = g_subprocess_launcher_new(G_SUBPROCESS_FLAGS_STDOUT_PIPE);
	
	subprocess = g_subprocess_launcher_spawn(launcher, &error, "./l", NULL);
	if(error){
		g_print("err %s\n", error->message);
		g_error_free(error);
		return 1;
	}
	
	
	
	
	
	int (*pip)[2];
	sigset_t signewset;
	int pipes_storage[8][2];
  int close_fd;
  int use_fd;
  int fd;
  int n;
  int stdio_count = 4;
  pip = pipes_storage;
  /*
  pip[2][1] = fcntl(0, F_DUPFD_CLOEXEC, stdio_count);
  g_print("LALA %d\n", pip[2][1]);
  pip[3][1] = fcntl(1, F_DUPFD_CLOEXEC, stdio_count);
  g_print("LALA %d\n", pip[3][1]);
  pip[4][1] = fcntl(2, F_DUPFD_CLOEXEC, stdio_count);
  //fcntl(2, F_DUPFD_CLOEXEC, stdio_count);
  //fcntl(2, F_DUPFD_CLOEXEC, stdio_count);
  g_print("LALA %d\n", pip[4][1]);
  fcntl(3, F_DUPFD_CLOEXEC, stdio_count);
  fcntl(4, F_DUPFD_CLOEXEC, stdio_count);
  use_fd = pip[0][1];
  
  fd = dup2(0, 3);
  g_print("FD %d\n", fd);
  
  fd = dup2(1, 4);
  g_print("FD %d\n", fd);
  fd = dup2(2, 4);
  g_print("FD %d\n", fd);
  fd = dup2(2, 3);
  g_print("FD %d\n", fd);
  fd = dup2(1, 3);
  g_print("FD %d\n", fd);
  */
  
  g_print("HERE!\n");
 pip = pipes_storage;
 /*
 //if(stdio_count > 100;
 pip = g_malloc(stdio_count* sizeof(*pip));
 if(pip == NULL) g_print("pip is null\n");
  for(int i=0;i < stdio_count; i++){
	 pip[i][0]=-1;
	 pip[i][1] = -1; 
  }
  for (fd = 0; fd < stdio_count; fd++) {
	  g_print("wow!\n");
	  if(fd > 5) break;
   use_fd = pip[fd][1];
    g_print("use_fd: %d\n", use_fd);
    if (use_fd < 0 || use_fd >= fd)
      continue;
#ifdef F_DUPFD_CLOEXEC 
g_print("f dupfd cloexec\n");
    pip[fd][1] = fcntl(use_fd, F_DUPFD_CLOEXEC, stdio_count);
#else
g_print("f dupfd\n");
    pip[fd][1] = fcntl(use_fd, F_DUPFD, stdio_count);
#endif
    if (pip[fd][1] == -1) g_print("pipes -1\n");
      //uv__write_errno(error_fd);
#ifndef F_DUPFD_CLOEXEC 
   // n = uv__cloexec(pipes[fd][1], 1);
   // if (n){
    //  uv__write_int(error_fd, n);
      fprintf(stderr, "ifndef F_DUPFD_CLOEXEC");
      #endif
  }
  int gk = 0;
   for (fd = 0; fd < stdio_count; fd++) {
	   gk++;
	   if(gk > 10) break;
    close_fd = -1;
    use_fd = pip[fd][1];

    if (use_fd < 0) {
      if (fd >= 3)
        continue;
      else {
       // Redirect stdin, stdout and stderr to /dev/null even if UV_IGNORE is
  
    //    uv__close_nocheckstdio(fd); 
   close(fd);
       use_fd = open("/dev/null", fd == 0 ? O_RDONLY : O_RDWR);
        close_fd = use_fd;

        if (use_fd < 0)fprintf(stderr, "use fd %d\n", use_fd);
          //uv__write_errno(error_fd);
      }
    }
 g_print("fd use_fd %d %d %d\n", fd, use_fd, close_fd);
    if (fd == use_fd) {
      if (close_fd == -1) {
      //  n = uv__cloexec(use_fd, 0);
       // if (n)
       //   uv__write_int(error_fd, n);
       fprintf(stderr, "close fd %d\n", close_fd);
      }
    }
    else {
		fprintf(stderr, "we dup2 %d %d %d\n", close_fd, use_fd, fd);
		
      fd = dup2(use_fd, fd);
    }
printf("suka %d \n", fd);
    if (fd == -1) fprintf(stderr," uv__write_errno(error_fd)%d\n", fd);
printf("suka %d \n", fd);

    if (fd <= 2 && close_fd == -1) {
		printf("suka %d \n", fd);
    fprintf(stderr, "fd <=2\n");
      uv__nonblock_fcntl(fd, 0);
      printf("suka %d \n", fd);
}

    if (close_fd >= stdio_count){
		//fprintf(stderr,"close fd >=4 %d", close_fd);
      //uv__
     // close(close_fd);
  } 
  }*/
  printf("suka %d \n", fd);
  
  int fds[4];
	if(!g_unix_open_pipe(fds, FD_CLOEXEC, &error))if(error)g_error("creating pipes %s\n", error->message);
	
	//pipe(fds);
	g_subprocess_launcher_take_stdin_fd(launcher, 3); //3 4 5 6 
	g_subprocess_launcher_take_stdout_fd(launcher, 3);
	g_subprocess_launcher_take_stderr_fd(launcher, 3);//etwas no data
	//g_subprocess_launcher_take_stdout_fd(launcher, 4);
	
	g_subprocess_launcher_take_stdin_fd(launcher, 4); //3 4 5 6 
	
	g_subprocess_launcher_take_fd(launcher, 3, 4);
	g_subprocess_launcher_take_stdout_fd(launcher, 4);
	g_subprocess_launcher_take_stderr_fd(launcher, 4);
	
	int re = dup2(4,3);
	g_print("%d\n", re);
	
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
	/*
	if(!g_unix_set_fd_nonblocking(0, TRUE, &error))
	g_error("failed non blockin %s\n", error->message);
	if(!g_unix_set_fd_nonblocking(1, TRUE, &error))
	g_error("failed non blockin %s\n", error->message);
	if(!g_unix_set_fd_nonblocking(2, TRUE, &error))
	g_error("failed non blockin %s\n", error->message);
	if(!g_unix_set_fd_nonblocking(3, TRUE, &error))
	g_error("failed non blockin %s\n", error->message);
	//if(!g_unix_set_fd_nonblocking(4, TRUE, &error))
	g_error("failed non blockin %s\n", error->message);
	printf("suka fd  %d \n", fd);
	//fcntl(fds[2], F_DUPFD,4);*/
	
	//fcntl(fds[3], F_DUPFD,3);
	printf("suka %d \n", fd);
	int bu;
	//bu=dup2(0, 4);
	//g_print("bu %d\n", bu);
	
//	bu=dup2(3, 3);
	//g_print("bu %d\n", bu);
	
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
	g_print("ku ku\n");
	if(!g_io_add_watch(channel3, G_IO_OUT | G_IO_HUP, writ, NULL)) g_error("cannot add watch on giochannel\n");
	//if(!g_io_add_watch(channel3, G_IO_IN | G_IO_HUP, red, NULL)) g_error("cannot add watch on giochannel\n");
	if(!g_io_add_watch(channel4, G_IO_IN | G_IO_HUP, red, NULL)) g_error("cannot add watch on giochannel\n");
	//if(!g_io_add_watch(channel7, G_IO_OUT | G_IO_HUP, writ, NULL)) g_error("cannot add watch on giochannel\n");
	
	
	//g_print("producer_channel is writable: %d\n", producer_channel->is_writeable);
	//g_print("consumer_channel is writable: %d\n", consumer_channel->is_writeable);
	//g_print("producer_payload is writable: %d\n", producer_payload->is_writeable);
	//g_print("consumer payload is writable: %d\n", consumer_payload->is_writeable);
	
//g_timeout_add_seconds(1, (GSourceFunc) persona_active_cb, channel3);
	
	
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
   int uv__nonblock_fcntl(int fd, int set) {
  int flags;
  int r;

  do
    r = fcntl(fd, F_GETFL);
  while (r == -1 && errno == EINTR);

  if (r == -1) return 1;
  //  return UV__ERR(errno);

  /* Bail out now if already set/clear. */
  if (!!(r & O_NONBLOCK) == !!set)
    return 0;

  if (set)
    flags = r | O_NONBLOCK;
  else
    flags = r & ~O_NONBLOCK;

  do
    r = fcntl(fd, F_SETFL, flags);
  while (r == -1 && errno == EINTR);

  if (r)
    return 1;//UV__ERR(errno);

  return 0;
}
