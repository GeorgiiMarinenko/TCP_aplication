import socket

client_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_sock.connect(('192.168.1.78', 4335))
client_sock.sendall(b'Hello, world')
data = client_sock.recv(1024)
client_sock.close()
print('Received', repr(data))
