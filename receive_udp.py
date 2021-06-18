# https://wiki.python.org/moin/UdpCommunication

import socket

UDP_IP = "192.168.0.23"
UDP_PORT = 19988

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

while True:
    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    print("received message: %s" % data, flush=True)