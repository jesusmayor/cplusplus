server = "localhost"
port = "4567"
botPath = "Debug/MyBotClient"
serverPath = "../../botserver/botserver"
system("#{serverPath} --port #{port} &")
system("#{botPath} #{server} #{port} &")
system("#{botPath} #{server} #{port}")
