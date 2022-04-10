#Project: Shopping Cart with Billing System
#Author: ppotturi@oakland.edu
#File: OUMart_TCP_server.py
#Description: 
# 1. Connect to client when requested
# 2. Send the Item info if barcode data is matched
# 3. Disconnect from Client when requested

# Windows use:
# Run Script: cmd prompt: python "OUMart_TCP_server.py" 
# Kill Script: Ctrl+Pause or Ctrl+Fn+B

import socket # Import socket module
import time

print 'Started OU Mart Server...'

while True:

	empty_cnt = 0
	s = socket.socket()         # Create a socket object
	host = socket.gethostname() # Get local machine name
	port = 5002                # Reserve a port for your service.
	s.bind((host, port))        # Bind to the port
	print 'Waiting for Client connection...'
	s.listen(5)                 # Now wait for client connection.

	c, addr = s.accept()     # Establish connection with client.
	print 'Got connection from', addr
	while True:
		data = c.recv(12)
		print "received data:", data
		if data == "096619475735" :
			time.sleep(1)
			c.send('096619475735,Paper,5.00*\r\n')
			print '096619475735,Paper,5.00*\r\n'
		if data == "312843592186" :
			time.sleep(1)
			c.send('312843592186,Chips,1.00*\r\n')
			print '312843592186,Chips,1.00*\r\n'
		if data == "081555793715" :
			time.sleep(1)
			c.send('081555793715,Soda ,2.00*\r\n')
			print '081555793715,Soda ,2.00*\r\n'
			
		if data == "096619756803" :
			time.sleep(1)
			c.send('096619756803,Water,1.50*\r\n')
			print '096619756803,Water,1.50*\r\n'	
		
		if data == "083046000135" :
			time.sleep(1)
			c.send('083046000135,Water,1.50*\r\n')
			print '083046000135,Water,1.50*\r\n'
			
		if data == "" :
			empty_cnt = empty_cnt+1
			if empty_cnt >= 5 :
				 c.close()
				 empty_cnt = 0
				 print 'connection closed from', addr
				 break

# End of Script

