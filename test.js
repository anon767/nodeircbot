'use strict';

var ircbot = require('./build/Release/ircbot');

ircbot.start("irc.thecout.com","#juice","6667","chillibot_",
	
	function(msg){
		if(msg=="test")
			ircbot.send("yay es läuft");
	}

);
