import os, signal, operator
import sys
import socket
import threading
import time

BUFSIZE = 65507
global sock
global serverAddr
chatting = False
displayLock = threading.Lock()
historyLock = threading.Lock()

recent = ''
history = []

width = 66

msgId = 0
global euid
recvPort = 0
pollPort = 0

class MyThread(threading.Thread):    
    def __init__(self):
        threading.Thread.__init__(self)

    def run(self):
        global sock
        global BUFSIZE
        global recent
        while True:
            data, addr = sock.recvfrom(BUFSIZE)
            recent = "receive message\n"
            parseMessage(data)
            display()

class REQUEST_TYPE:
    CONNECT = 0
    DISCONNECT = 1
    POLLING = 2
    CREATE_CHAT = 3
    QUIT_CHAT = 4
    ADD_USER_TO_CHAT = 5
    SEND_MESSAGE = 6
    TESTER_LOBBY = 7
    
class MESSAGE_TYPE:
    NO_MORE_SERVERS = 0
    EXCEED_CHAT_CAP = 1
    CHAT_NOT_EXIST = 2
    USER_NOT_IN_CHAT = 3
    CONNECT_ERROR = 4
    USER_LIST = 5
    PORTS = 6
    CHAT_ID = 7
    TEXT_MSG = 8
    AUDIO_MSG = 9
    VIDEO_MSG = 10
    CHAT_INFO = 11
    SERVER_INFO = 12
    RE_CONNECT = 13
    CONFIRM = 14
    CREATE_CHAT_INVALID_USER = 15
                        
def display():
    displayLock.acquire()
    os.system('cls' if os.name == 'nt' else 'clear')
    if chatting:
        print "==============================================================="
        print "                          Chat room                            "
        print "==============================================================="
        print "0. quit"
        print "Send message: enter one line message"
        print "==============================================================="
    else:
        print "==============================================================="
        print "                            Menu                               "
        print "==============================================================="
        print "1. connect"
        print "2. enter tester lobby"
        print "0. quit"
        print "==============================================================="

    print recent
    
    for data in history:
        formatPrint(data)
    
    displayLock.release()

def formatPrint(data):
    prefix = ''
    if data[0] == "Me:":
        prefix = ' ' * (width / 2 - 3) + ' | ';
    print prefix, data[0]

    length = len(data[1])
    start = 0
    half = width / 2 - 4
    
    while length > 0:
        sublength = half
        if sublength > length:
            sublength = length
        print prefix, data[1][start:start + sublength]
        start += sublength
        length -= sublength
    
def mainLoop():
    global sock
    while True:
        display()
        data = raw_input()
        if len(data) > 0:
            f = ord(data[0]) - ord('0')
            if not chatting:
                if f == 0:
                    print "Bye..."
                    exit()
                elif f == 1:
                    connect()
                elif f == 2:
                    chat()
            else:
                if f == 0:
                    print "Bye..."
                    exit()
                else:
                    sendMessage(data)

def int32ToBytes(number):
    arr = bytearray()
    arr += chr((number >> 24) & 0xff)
    arr += chr((number >> 16) & 0xff)
    arr += chr((number >> 8)  & 0xff)
    arr += chr(number & 0xff)
    return arr

def int16ToBytes(number):
    arr = bytearray()
    arr += chr((number >> 8) & 0xff)
    arr += chr(number & 0xff)
    return arr
                    
def connect():
    global recent
    recent = "connect to server..."
    msg = bytearray()
    msg += int16ToBytes(recvPort)
    msg += int16ToBytes(pollPort)
    packet = makePacket(euid, REQUEST_TYPE.CONNECT,
                        MESSAGE_TYPE.PORTS, msg)
    sendtoServer(packet)

def chat():
    print "chat"
    global chatting
    chatting = True

def sendMessage(data):
    global recent
    recent = "sent message" 
    saveMessage(("Me:", data))
    

def saveMessage(data):
    historyLock.acquire()

    global history
    history.append(data)
    
    historyLock.release()

def makePacket(senderId, reqType, msgType, msg):
    global msgId
    msgId += 1
    packet = bytearray()
    packet += int32ToBytes(msgId)
    packet += str.encode(senderId)
    packet += chr(reqType)
    packet += chr(msgType)
    print "p", msg[0] << 8 | msg[1]
    print "p", msg[2] << 8 | msg[3]
    print msg[0]
    print msg[1]
    print msg[2]
    print msg[3]
    print "==="
    for x in msg:
        print x
        packet += chr(x)
    print "==="
    print packet[8]
    print packet[9]
    print packet[10]
    print packet[11]
    
    return packet
    
def sendtoServer(buf):    
    sock.sendto(buf, serverAddr)
    data, addr = sock.recvfrom(BUFSIZE)
    global recent
    recent = "receive"

if __name__ == "__main__":
    argc = len(sys.argv)
    if argc != 4:
        print "usage:", sys.argv[0], "<hostname> <hostport> <EUID>(2byte)"
        sys.exit(1)

    global euid
    global serverAddr
    global sock
    
    # get command line inputs
    hostName = sys.argv[1]
    hostPort = sys.argv[2]
    euid = sys.argv[3]
    serverAddr = (hostName, int(hostPort))
    sock = socket.socket(socket.AF_INET, # Internet
                         socket.SOCK_DGRAM) # UDP
    sock.bind(('localhost', 5000)) #let OS to pick up an open port to bind with

    recvPort = pollPort = int(sock.getsockname()[1]);

    #receiving = MyThread()
    #receiving.daemon = True
    #receiving.start()

    mainLoop();

    
