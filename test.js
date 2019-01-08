'use strict';

var ircbot = require('./build/Release/ircbot');
var hn = require('hackernews-api');


function getnewstories(i){
let ids = hn.getNewStories();
return hn.getItem(ids[i])["title"] + " " + hn.getItem(ids[i])["url"];
}


ircbot.start("irc.thecout.com","#juice","6667","chillibot_",
	
	function(nick, msg){
		if(msg=="!hackernews"){
			ircbot.send("hackernews feed:");
			ircbot.send(getnewstories(0));
			ircbot.send(getnewstories(1));
			ircbot.send(getnewstories(2));
		
		}
	}

);
