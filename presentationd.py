#!/usr/bin/python2

from BaseHTTPServer import BaseHTTPRequestHandler,HTTPServer
import socket

print("Starting")


LOC=True


if LOC:
    from evdev import uinput, ecodes as e

def nextt():
    with uinput.UInput() as ui:
        ui.write(e.EV_KEY, e.KEY_SPACE, 1)
        ui.write(e.EV_KEY, e.KEY_SPACE, 0)
        ui.syn()




if LOC==False:
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    port = 12347
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind(('',port))
    s.listen(5)
    client, address = s.accept()
    print (address)


PORT_NUMBER = 12345

#This class will handles any incoming request from
#the browser 
class myHandler(BaseHTTPRequestHandler):
	
	def do_POST(self):
                if LOC:
                    nextt()
                else:
                    client.send("hi")
		print("got message")
		self.send_response(200)
		self.send_header('Content-type','application/json')
		self.end_headers()

		self.wfile.write("")
		return

try:
	#Create a web server and define the handler to manage the
	#incoming request
	server = HTTPServer(('', PORT_NUMBER), myHandler)
	print 'Started httpserver on port ' , PORT_NUMBER
	server.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	#Wait forever for incoming htto requests
	server.serve_forever()

except KeyboardInterrupt:
	print '^C received, shutting down the web server'
	server.socket.close() 
	client.close()
	s.close()
	
