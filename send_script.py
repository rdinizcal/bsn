import socket
import random
from time import sleep

host = socket.gethostname()
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host, 8080))
s2 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s3 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s2.connect((host, 8081))
s3.connect((host, 8082))
while(True):
    num = random.uniform(30.1,41.9)
    num1 = random.uniform(40.0,150.0)
    num2 = random.uniform(50,110.1)
    packet1 = str(0) + '-' + str(num) + '*'
    packet2 = str(1) + '-' + str(num1) + '*'
    packet3 = str(2) + '-' + str(num2) + '*'
    s.sendall(packet1)
    s2.sendall(packet2)
    s3.sendall(packet3)
    sleep(0.5)