import socket

import eventlet
import socketio






class CyBotSocketServer():
    def __init__(self):
        self.sio = socketio.Server()
        self.app = socketio.WSGIApp(self.sio)

        @self.sio.event
        def connect(sid, environ):
            self.sio.emit("message", "hi")
            print('connect ', sid)

        @self.sio.event
        def my_message(sid, data):
            print('message ', data)

        @self.sio.event
        def disconnect(sid):
            print('disconnect ', sid)

        eventlet.wsgi.server(eventlet.listen(('', 5000)), self.app)


if __name__ == "__main__":
    useCybot = False
    if useCybot:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect(("192.168.1.1", 288))
        print("Connected")

    server = CyBotSocketServer()