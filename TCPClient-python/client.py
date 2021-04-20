import socket
import string

choice = 1
message = 'test'
port = input('port: ')
while choice < 5:
    print('Press:\n1-to connect to host - necessarily!\n2-to write & send the message\n3-to send the default message\n4-to close the connection\n5-to quit')
    choice = input()
    if choice == 1:
        s =socket.socket()
        host=socket.gethostname()
        s.connect(('192.168.1.78',port))
        print('Client succesfully connected')
    if choice == 2: #Write a message
        # print()
        message = raw_input('Write a client message:')
        choice = 3
    if choice == 3: #Connect+send+record+close
        s.send(message.encode())
        with open('received.txt','wb') as f:
            print ('file opened')
            print ('receiving data...')
            while True :
                data=s.recv(1024)
                if not data:
                    break
                print ('Data=>',data.decode())
                f.write(data)
                if (len(data) < 1024):
                    break
        f.close()
        print ('Successfully get the file')
    if choice == 4:
        s.close()
        print ('connection closed')


