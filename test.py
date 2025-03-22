import socket
import struct

s = socket.socket()
s.connect(('localhost', 5000))

req = struct.pack('!c31s8s', b'W', b'mykey', b'5')
s.sendall(req)
s.close()
