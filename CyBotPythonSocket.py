from flask import Flask, render_template, session, request
from flask_cors import CORS, cross_origin
from flask_socketio import SocketIO
from datetime import datetime
import socket
import threading


home = True

app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins="*")

def setupSocket():

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(("192.168.1.1", 288))

    print("Connected")
    return s
def cyBotRead(sock):
    line = ''
    global killThread
    killThread = False
    while True:
        if killThread:
            break
        line = sock.recv(1024).decode('utf8').strip().replace('\r', '')
        arr = line.split("\n")
        for l in arr:
            socketio.emit('message', l)


s = setupSocket()
t1 = threading.Thread(target = cyBotRead, args = [s])


@socketio.on('connect')
def on_connect():
    print('Client connected')
    socketio.emit('my response', {'data': 'Connected'})

@socketio.on('disconnect')
def on_disconnect():
    users.pop(request.sid,'No user found')
    socketio.emit('current_users', users)
    print("User disconnected!\nThe users are: ", users)

@socketio.on('connect_error')
def printErr(data):
    print(data)
    pass
@socketio.on('message')
def messaging(message, methods=['GET', 'POST']):
    print('received message: ' + str(message))
    socketio.emit("message", message)

@socketio.on('*')
def catch_all(event, sid, data):
    print(event, sid, data)
if __name__ == '__main__':
    socketio.run(app, host="0.0.0.0", debug=True)

