# import socket
# import struct

# s = socket.socket()
# s.connect(('localhost', 5000))

# # Pack Write request: op='W', name='mykey', len='5'
# req = struct.pack('!c31s8s', b'W', b'mykey', b'5')
# # Send header + data
# s.sendall(req + b'Hello')

# # Receive response (always 40 bytes)
# resp = s.recv(40)
# status, name, length = struct.unpack('!c31s8s', resp)
# print("Write Response Status:", status.decode())

# s.close()

import socket
import struct

s = socket.socket()
s.connect(('localhost', 5000))

# Read request for key 'mykey'
req = struct.pack('!c31s8s', b'D', b'mykey', b'0')
s.sendall(req)

# Read the 40-byte response
resp = s.recv(40)
status, name, length = struct.unpack('!c31s8s', resp)
print("Read Response Status:", status.decode())

s.close()


