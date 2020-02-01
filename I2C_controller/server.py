import datetime
import time
import serial

if __name__ == "__main__":
    ser = serial.Serial(
        port='/dev/ttyACM0',
        baudrate=115200,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS
    )

    while True:
        byte_response = ser.readline()
        char_response = byte_response.decode('UTF-8')
        print(char_response)
