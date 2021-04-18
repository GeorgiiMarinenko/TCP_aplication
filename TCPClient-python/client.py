import socket
s =socket.socket()
host=socket.gethostname()
port=4335
s.connect(('192.168.1.78',port))
s.send('Client 1 - python'.encode())
with open('received.txt','wb') as f:
    print ('file opened')
    print ('receiving data...')
    while True :
        data=s.recv(1024)
        if not data:
            break
        # print ('Data=>',data.decode())
        f.write(data)
        if (len(data) < 1024):
            break
f.close()
print ('Successfully get the file')
s.close()
print ('connection closed')
