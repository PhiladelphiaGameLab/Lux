import socket

sendSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sendSocket.bind (("",5005))

print ("Ready to receive message")
print( sendSocket.recvfrom(1024))
print ("Finished receiving");			

