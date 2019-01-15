C ircbot binding for NodeJS based upon:

https://github.com/Themaister/simple-irc-bot

# Install
npm i nodeircbot

# Usage
```
const ircbot = require("nodeircbot");
ircbot.start("irc.thecout.com", "#test", "6667", "testbot",(nick,msg) =>{
ircbot.send(nick +" wrote " + msg);

};
```