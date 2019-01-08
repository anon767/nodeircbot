#include <unistd.h>
#include "socket.h"
#include "irc.h"
irc_t irc;
void bot_send (char * message){
 irc_msg((&irc)->s,(&irc)->channel,message);
}
void bot_close(){
irc_close(&irc);
}
void bot_start (const char *server,const char *channel,const char * port,const char *nick)
{
   char* _nick = (char*)nick;
   char* _channel = (char*)channel;
   if ( irc_connect(&irc, server, port) < 0 )
   { 
      fprintf(stderr, "Connection failed.\n");
      bot_close();
   }

   irc_set_output(&irc, "/dev/stdout");
   if ( irc_login(&irc, _nick) < 0 )
   {
      fprintf(stderr, "Couldn't log in.\n");
      bot_close();
   }
   int joined = 0; //need to wait for welcome message
   	while(irc_handle_data(&irc)>=0)
	{
	if(joined==2){
	 if ( irc_join_channel(&irc, _channel) < 0 )
        	{
       		fprintf(stderr, "Couldn't join channel.\n");
   	 	bot_close();
		}
	}else{
	 joined++;
	}
   }

   irc_close(&irc);

}

