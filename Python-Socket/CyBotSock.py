import threading
import socket
import re
class CybotSockListener(threading.Thread):
    startChar = "start"
    endChar = "end"

    def __init__(self, sock: socket):
        super().__init__()
        self.sock = sock

    def run(self):
        while True:
            line = re.split("start.+?end", self.sock.recv(1024).decode('utf8'))
            for l in line:
                print("found: ", l)

    def receiveData(self, str):
        if str[0] == 'o':
            self.receiveObject(str)
        elif str[0] == 'd':
            self.distanceTravelled(str)
        elif str[0] == 't':
            self.turn(str)



    def receiveObject(self, str):
        pass
    def distanceTravelled(self, str):
        pass


