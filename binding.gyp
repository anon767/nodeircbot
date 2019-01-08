{
  "targets": [
    {
      "target_name": "ircbot",
      "sources": [ "src/binding.cpp","src/bot.c","src/irc.c","src/socket.c"],
	"link_settings": {
        "libraries": [
          "-lcurl",
        ],
        "ldflags": [
        ]
      },
    }
  ]
}
