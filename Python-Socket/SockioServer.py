import socket

import eventlet
import socketio
import CyBotSock
import threading
import socket
import re
from flask import Flask
import queue
from gevent import pywsgi

eventlet.monkey_patch()


sio = socketio.Server(cors_allowed_origins='*')


startChar = "start"
endChar = "end"

def bg_emit(_str):
    sio.emit('message', _str)
def listen(sock):
    while True:
        line = re.split("start.+?end", sock.recv(1024).decode('utf8'))
        for l in line:
            print("found: ", str(l))
            bg_emit(l)
            eventlet.sleep(2)
def listenFake():
    while True:
        msg = input("input message: ")
        bg_emit(msg)
class CyBotSocketServer():
    def __init__(self):

        self.app = socketio.WSGIApp(sio)

        @sio.event
        def connect(sid, environ):
            sio.emit("message", "Welcome")
            print('connect ', sid)

        @sio.on("message")
        def my_message(sid, data):
            print('message ', data)
            s.send(bytes(data, 'utf8'))

        @sio.event
        def disconnect(sid):
            print('disconnect ', sid)

        eventlet.wsgi.server(eventlet.listen(('', 5000)), self.app)


if __name__ == "__main__":
    useCybot = True
    if useCybot:
        global s
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect(("192.168.1.1", 288))
        print("Connected")
        eventlet.spawn(listen, s)
    else:
        eventlet.spawn(listenFake)
    server = CyBotSocketServer()
