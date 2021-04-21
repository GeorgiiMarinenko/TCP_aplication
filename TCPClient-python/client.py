import socket
import string
from datetime import datetime

choice = 1
message = 'test'
port = input('port: ')
host= raw_input('host: ')
connected = 0
wasInFour = 0
while choice < 5:
    print('Press:\n1-to connect to host - necessarily!\n2-to write & send the message\n3-to send the default message\n4-to close the connection\n5-to quit')
    choice = input()
    if choice == 1:
        s =socket.socket()
        s.connect((host,port))
        wasInFour = 0
        current_datetime_connection = datetime.now()
        print('Client succesfully connected')
    if choice == 2: #Write a message
        # print()
        message = raw_input('Write a client message: ')
        choice = 3
    if choice == 3: #Connect+send+record+close
        current_datetime_sent = datetime.now()
        s.send(message.encode())
        # print ('sent: ' + str(current_datetime_sent) + " - " + message)
        with open('log_client.txt','a') as f:
            print ('file opened')
            print ('receiving data...')
            while True :
                data=s.recv(1024)
                if not data:
                    break
                current_datetime_responce = datetime.now()
                # mes = current_datetime + " - " + data
                if connected == 0:
                    f.write('**********************************************************************************************\n')
                    f.write('time: ' + str(current_datetime_connection) + ' - connection to host: ' + str(host) + ' port: ' + str(port))
                    f.write('\n\n')
                    connected = 1
                f.write('sent: ' + str(current_datetime_sent) + " - " + message)
                f.write('\n')
                # print(str(current_datetime_responce) + " - " + data)
                f.write('response: ' + str(current_datetime_responce) + " - " + data[1:])
                f.write('\n----------------------------------------------------------------\n\n')
                if (len(data) < 1024):
                    break
        f.close()
        print ('Successfully recieved logs to the file')
    if choice >= 4:
        if wasInFour == 0:
            wasInFour = 1
            with open('log_client.txt','a') as f:
                current_datetime_disconnection = datetime.now()
                f.write('time: ' + str(current_datetime_disconnection) + ' - client has disconnected from host: ' + str(host) + ' port: ' + str(port))
                f.write('\n*********************************************************************************************\n\n')
            f.close
            s.close()
            print ('connection closed')



