# https://wiki.python.org/moin/UdpCommunication

import socket

#UDP_IP = "192.168.0.23"
#UDP_PORT = 19988

#UDP_IP = "90.235.13.240"
#UDP_PORT = 59988

UDP_IP = "188.149.34.2"
UDP_PORT = 19988

MESSAGE = b"Hello, World!"

print("UDP target IP: %s" % UDP_IP)
print("UDP target port: %s" % UDP_PORT)
print("message: %s" % MESSAGE)

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))

