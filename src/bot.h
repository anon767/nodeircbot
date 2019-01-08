#ifndef __Bot_H
#define __Bot_H

#include <stdio.h>

void bot_start(const char * server,const char * channel,const char* port,const char * nick);
void bot_send(char * message);
#endif
