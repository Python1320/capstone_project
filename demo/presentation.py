from evdev import uinput, ecodes as e

def nextt():
    with uinput.UInput() as ui:
         ui.write(e.EV_KEY, e.KEY_SPACE, 1)
         ui.write(e.EV_KEY, e.KEY_SPACE, 0)
         ui.syn()


import socket
import sys

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect the socket to the port where the server is listening
server_address = ('127.0.0.1', 12347)
sock.connect(server_address)

print("connected")
while True:
    data =  sock.recv(2)
    if data:
        print(data)
        if data==b"hi":
            nextt()
            break

    else:
        print("wot")
    
    
