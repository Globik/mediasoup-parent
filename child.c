/*  FOR NODE.JS m.js !!!   */

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
int a = 0;int c = 0;

static gboolean readi(GIOChannel *gio, GIOCondition condition, gpointer data){
	g_print(yellow"Read func in child\n"rst);
	a++;
	if(a > 4) return FALSE;
	g_print("gio is readable? %d\n", gio->is_readable);
	g_print("gio is writeable? %d\n", gio->is_writeable);
	if(gio->is_readable == 0){
		g_print(redc "o fuck is not readable in child\n"); 
	//	return FALSE;
	}
	GError *err = NULL;
	
	if(condition & G_IO_HUP){ 
		g_error(redc "READ end of pipe died in child\n" rst); 
		return FALSE;
		}

gchar buf[4096];

gsize bytes_read;

gunichar thechar;

gsize tpos;

//gchar buf[ BUFSIZ + 1 ]; mama papa
//GString *bufi = g_string_sized_new(512);
// g_io_cha g_io_channel_read_line_string( chann, buffer, 0, &err ) 
//g_io_channel_read_chars(gio, buf, sizeof(buf), &bytes_read, &err);
//g_io_channel_read_unichar(gio, &thechar, &err);

//g_print(green "Some data to read in child: %s\n" rst, buf);
//g_print("Some data to read in child %d\n", thechar);
//g_print("length in child %lu\n", bytes_read);
/*
 while( G_IO_STATUS_NORMAL == g_io_channel_read_line_string( gio, bufi, &tpos, &err ) ) {
   }
      g_print(green "Some data to read in child1: %s\n" rst, bufi->str);
      g_print("length in child line_string: %lu\n", tpos);
    */  
   while( G_IO_STATUS_NORMAL == g_io_channel_read_chars( gio, buf, sizeof(buf), &bytes_read, &err )) {
      
     // if( n < BUFSIZ ) break;
   }
   buf[sizeof(buf)]="\0";// any need?
   g_print(green "Some data to read in child2: %s\n" rst, buf);
g_print("length in child %lu\n", bytes_read);





return TRUE;
}

static gboolean writi(GIOChannel *gio, GIOCondition condition, gpointer data)
{
	
	g_print("gio is writeable in child? %d\n", gio->is_writeable);
	g_print("gio is readable in child? %d\n", gio->is_readable);
	if(c == 4){return FALSE;}
GIOStatus ret;
	GError *error = NULL;
	gsize len;

	const char *msg = "fuck you AHA ";
	 ret = g_io_channel_write_chars(gio, msg, -1, &len, &error);
	if(ret == G_IO_STATUS_ERROR) {
		g_error("error writing: %s\n", error->message);
		g_error_free(error);
	}
	g_print("Wrote %lu bytes\n", len);
	c++;
	return TRUE;
}
 

int main(int argc, char* argv[]){
	
	GError *error = NULL;
int fds[1];
	
	//if(!g_unix_open_pipe(fds, FD_CLOEXEC, &error)){g_error("creating pipes %s\n", error->message);}
	
	//if(!g_unix_set_fd_nonblocking(3, TRUE, &error)){g_error("failed 3 non blockin \n");}
    //if(!g_unix_set_fd_nonblocking(4, TRUE, &error)){g_error("Oh now failed non blockin %s\n", error->message);}

	//GIOChannel *channel0 = g_io_channel_unix_new(0);if(!channel0) g_error("cannot create new giochannel\n");
	//GIOChannel *channel1 = g_io_channel_unix_new(1);if(!channel1) g_error("cannot create new giochannel\n");
	//GIOChannel *channel2 = g_io_channel_unix_new(2); if(!channel2) g_error("cannot create new giochannel\n");
	
	GIOChannel *channel3 = g_io_channel_unix_new(3);if(!channel3) g_error("cannot create new giochannel\n");//producer writes
//GIOChannel *channel4 = g_io_channel_unix_new(4);if(!channel4) g_error("cannot create new giochannel\n");//consumer reads
	
	//g_io_channel_set_flags(channel0, G_IO_FLAG_NONBLOCK, NULL);
	//g_io_channel_set_flags(channel1, G_IO_FLAG_NONBLOCK, NULL);
	//g_io_channel_set_flags(channel2, G_IO_FLAG_NONBLOCK, NULL);
	//g_io_channel_set_encoding(channel0, "UTF-8", NULL);
	//g_io_channel_set_buffered(channel0, FALSE);
	g_io_channel_set_flags(channel3, G_IO_FLAG_NONBLOCK, NULL);  //writes
	//g_io_channel_set_flags(channel4, G_IO_FLAG_NONBLOCK, NULL); //reads
	
	//GIOChannel *channel6 = g_io_channel_unix_new(4);if(!channel6) g_error("cannot create new giochannel\n");//producer writes
	//GIOChannel *channel7 = g_io_channel_unix_new(5);if(!channel7) g_error("cannot create new giochannel\n");//consumer reads
	
	//g_print("is_readable 1: %d\n", channel1->is_readable);
	//g_print("is_readable 2: %d\n", channel2->is_readable);
	//g_print("is_readable 3: %d\n", channel3->is_readable);
	//g_print("is_readable 4: %d\n", channel4->is_readable);
	//g_print("is_readable 5: %d\n", channel5->is_readable);
	//dup2(3,3);
	
	//if(!g_io_add_watch(channel0, G_IO_IN | G_IO_HUP, readi, NULL)) g_error("cannot add watch on giochannel\n");
	//if(!g_io_add_watch(channel1, G_IO_OUT | G_IO_HUP, writi, NULL)) g_error("cannot add watch on giochannel\n");
	//if(!g_io_add_watch(channel2, G_IO_OUT | G_IO_HUP, writi, NULL)) g_error("cannot add watch on giochannel\n");
	if(!g_io_add_watch(channel3, G_IO_IN | G_IO_HUP, readi, NULL)) g_error("cannot add watch on giochannel\n");
	//if(!g_io_add_watch(channel3, G_IO_OUT | G_IO_HUP, writi, NULL)) g_error("cannot add watch on giochannel\n");
	//if(!g_io_add_watch(channel4, G_IO_IN | G_IO_HUP, writi, NULL)) g_error("cannot add watch on giochannel\n");
//if(!g_io_add_watch(channel5, G_IO_IN | G_IO_HUP, readi, NULL)) g_error("cannot add watch on giochannel\n");
	//if(!g_io_add_watch(channel6, G_IO_OUT | G_IO_HUP, writi, NULL)) g_error("cannot add watch on giochannel\n");
	//if(!g_io_add_watch(channel7, G_IO_OUT | G_IO_HUP, writi, NULL)) g_error("cannot add watch on giochannel\n");
	
	

	loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop); 
	//g_io_channel_unref(channel0);
	//g_io_channel_unref(channel1);
	//g_io_channel_unref(channel2);
	//g_io_channel_unref(channel3);
	//g_io_channel_unref(channel4);
	g_print("Quit the loop\n");
	return 0;
}
