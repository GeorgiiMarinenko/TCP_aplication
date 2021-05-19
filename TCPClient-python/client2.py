import socket
import string

sock = socket.socket()
sock.connect(("192.168.137.15",4335))

print ('Connected with server.')
print ('Enter "Quit" to exit.')

while True:
    data = raw_input()
    if data == 'Quit':
        sock.close()
        break
    else:
        sock.send(data.encode())
        answer = sock.recv(1024).decode()
        print ('Answer from server: ' + answer)
