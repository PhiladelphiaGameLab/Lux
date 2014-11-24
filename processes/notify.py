import threading
import json
import socket

	
sockets = {}
numUsers = 0
socketsLock = threading.Lock()
# Connect to user collection to access tokens and ids
# connect to other collections to access more info

# Start listening for messages on a port
def getClientId(access_token):
	# look in user collection for a matching access token
	# get the id from the returned documetn

	if( id != null):
		return id;
	else:
		return "loggonError"

def add(fromAddr, mongoId):
	# lock the sockets strut before adding this guy
	socketsLock.acquire()
	sockets[mongoId] = fromAddr
	socketsLock.release()
	numUsers = numUsers + 1

def upsert(data, fromAddr, mongoId):
	# upsert the message into the collection
	# add the document to the Published collection with references resolved

def query(data, fromAddr, mongoId):
	# Query the Assets collection and return the results to the fromAddr
	# Add the Query to the Subscribers collection and the client Id from to the 
		# Criteria's document

def disconnect(fromAddr, mongoId):
	# lock the sockets dict before removal
	socketsLock.acquire()
	del sockets[mongoId]
	socketsLock.release()
	numUsers = numUsers -1 

# create a fork here to do two seperate processes:

# Process 1: listen for connections
port = 5004
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(("127.0.0.1", port))

def recieveMessages()
	while True:
		try:
			packet = s.recvfrom(4096)
			msg = json.loads(packet[0])
			fromAddr = packet[1]
			mongoId = getClientId(msg['data']['id'])
			if( mongoId == "loggonError"):
				s.sendto("Please Log on, it's rude to be Anon", fromAddr)
			else:
				switch(msg['request']):
					case "add":
						add(fromAddr, mongoId)
					case "upsert":
						upsert(msg['data'], fromAddr, mongoId)
					case "query":
						query(msg['data'], fromAddr, mongoId)
					case "disconnect":
						disconnect(fromAddr, mongoId)
					default:
						print("message is an error")
		except socket.error:
			print("something went wrong on the socket")


# Process 2: send out messages
def emitMessages():
	while True:
		# Cycle through Subscribers criterium
		# Query the Published collection for the criteria
		# Look at the user's subscribed in subscribers
		# send message to client if [id] exists in subscribers collection
			# need to lock sockets structure before accessing it
		socketsLock.acquire()
		clientAddr = sockets[id]
		socketLock.release()
		if not clientAddr == null:
			s.sendto(msg, clientAddr)
		# mark as "sent on udp" in the published document


if __name__ == '__main__':
	# Split off multiple forks- this is annoying
	p1 = threading.Thread(target=recieveMessages)
	p1.start()
	p2 = threading.Thread(target=emitMessages)
	p2.start()
