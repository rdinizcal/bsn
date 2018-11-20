import socket
import random
from time import sleep

host = socket.gethostname()
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s2 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s3 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s4 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

s.connect((host, 8080))
s2.connect((host, 8081))
s3.connect((host, 8082))
s4.connect((host, 8083))


while(True):
    thermometer = random.uniform(30.1,41.9)
    ecg = random.uniform(40.0,150.0)
    oximeter = random.uniform(50.0,110.1)
    systolic_pressure  = random.uniform(70.0,190.0)
    diastolic_pressure = random.uniform(40.0,100.0)
    
    packet1 = 'thermometer' + '-' + str(thermometer) + '*'
    packet2 = 'ecg'         + '-' + str(ecg) + '*'
    packet3 = 'oximeter'    + '-' + str(oximeter) + '*'
    packet4 = 'bpms' + '-' + str(systolic_pressure) + '/' + 'bpmd' + '-' + str(diastolic_pressure) + '*'
    s.sendall(packet1)
    s2.sendall(packet2)
    s3.sendall(packet3)
    s4.sendall(packet4)
    sleep(3)