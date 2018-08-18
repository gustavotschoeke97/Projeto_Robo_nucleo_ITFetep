import * from time 
import ctime

ctrCmd = ['S','N']

HOST = ''
PORT = 21567
BUFSIZE = 1024
ADDR = (HOST,PORT)

tcpSerSock = socket(AF_INET, SOCK_STREAM)
tcpSerSock.bind(ADDR)
tcpSerSock.listen(5)

while True:
        print 'Waiting for connection'
        tcpCliSock,addr = tcpSerSock.accept()
        print '...connected from :', addr
        try:
                while True:
                        data = ''
                        data = tcpCliSock.recv(BUFSIZE)
                        if not data:
                                break
                        if data == ctrCmd[0]:
                                print 'Comando',ctrCmd[0],'enviado:'
                        if data == ctrCmd[1]:
                                print 'Comando',ctrCmd[1],'enviado:'
        except KeyboardInterrupt:
              #  Servomotor.close()
              #  GPIO.cleanup()
tcpSerSock.close();

