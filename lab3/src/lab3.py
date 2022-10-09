import serial
import csv

ser = serial.Serial("/dev/ttyUSB0",9600)
file = open('datos.csv', 'w')
writer = csv.writer(file)

while True:
    tempd = ser.readline()
    humd = ser.readline()
    temp = tempd.decode().strip()
    hum = humd.decode().strip()
    data= ["Temperatura:", temp, "Humedad:", hum]
    writer.writerow(data)
