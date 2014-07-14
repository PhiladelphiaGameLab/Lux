import os, signal, operator
import sys
import socket
import threading
import time

BUFSIZE = 65507
global sock
lock = threading.Lock()

class MyThread(threading.Thread):    
    def __init__(self):
        threading.Thread.__init__(self)

    def run(self):
        global sock
        global BUFSIZE
        while True:
            data, addr = sock.recvfrom(BUFSIZE)
            print "receive message:", data
            

def display():
    pass


def mainLoop():
    global sock
    

if __name__ == "__main__":
    argc = len(sys.argv)
    if argc != 4:
        print "usage:", sys.argv[0], "<hostname> <hostport> <EUID>(2byte)"
        sys.exit(1)

    # get command line inputs
    hostName = sys.argv[1]
    hostPort = sys.argv[2]
    euid = sys.argv[3]
    serverAddr = (hostName, int(hostPort))

    global sock
    sock = socket.socket(socket.AF_INET, # Internet
                         socket.SOCK_DGRAM) # UDP
    sock.bind(('localhost', 0)) #let OS to pick up an open port to bind with
    
    receiving = MyThread()
    receiving.daemon = True
    receiving.start()

    mainLoop();
