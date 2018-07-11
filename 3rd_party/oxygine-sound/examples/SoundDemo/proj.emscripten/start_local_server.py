import SimpleHTTPServer
import SocketServer
PORT = 80
Handler = SimpleHTTPServer.SimpleHTTPRequestHandler
httpd = SocketServer.TCPServer(("", PORT), Handler)
httpd.serve_forever()