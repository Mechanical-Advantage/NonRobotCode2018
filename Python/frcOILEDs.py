#!/usr/bin/env python3
from time import sleep

from networktables import NetworkTables
from serial import Serial

NTSERVER = "127.0.0.1"
SERIAL_PORT = "/dev/cu.usbmodem241141"
SET_BITS = 2

last_value = []

# serial port setup
arduino = Serial(SERIAL_PORT, 9600)
# opening connection causes Arduino to reset and not recive data for a few seconds
sleep(2)

def update_values(table, key, value, isNew):
    global last_value
    diff = [] # Will have true if the bit changed
    serial_data = []
    if len(last_value) < len(value):
        last_value = [False]*len(value)
    for old, new in zip(last_value, value):
        diff.append(False if old == new else True)
    byte_diff = [diff[i:i + 8-SET_BITS] for i in range(0, len(diff), 8-SET_BITS)]
    send_bits = [value[i:i + 8-SET_BITS] for i in range(0, len(value), 8-SET_BITS)]
    for index, item_diff, bits in zip(range(len(send_bits)), byte_diff, send_bits):
        if True in item_diff:
            # Ensure bits is of the proper length, must be list
            bits = list(bits)
            bits.extend([False]*((8-SET_BITS)-len(bits)))
            byte_index = index<<8-SET_BITS
            data_string = ''.join(['1' if x else '0' for x in bits])
            serial_data.append(int(data_string, base=2)+byte_index)
            print(index, data_string, sep=", ")
    last_value = value
    arduino.write(bytes(serial_data))

# NetworkTables setup
NetworkTables.initialize(server=NTSERVER)
#NetworkTables.enableVerboseLogging()
table = NetworkTables.getTable("LEDs")
print("Connecting to NetworkTables...")
while NetworkTables.getRemoteAddress() is None:
    sleep(1)
print("Connected to NetworkTables")
table.addTableListener(update_values, immediateNotify=True, key="OI LEDs")

lastStateConnected = True
# main thread not needed anymore
while True:
    sleep(5)
    if not lastStateConnected and NetworkTables.getRemoteAddress() is not None:
        lastStateConnected = True
        print("Re-connected to NetworkTables")
    elif lastStateConnected and NetworkTables.getRemoteAddress() is None:
        lastStateConnected = False
        print("Lost connection to NetworkTables")
