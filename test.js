'use strict';

var ircbot = require('./build/Release/ircbot');

ircbot.start("irc.thecout.com","#juice","6667","chillibot_",
	
	function(nick, msg){
		if(msg=="test")
			ircbot.send(nick + ":yay es läuft");
	}

);
