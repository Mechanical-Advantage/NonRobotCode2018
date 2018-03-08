#!/usr/bin/env python3
from time import sleep

from networktables import NetworkTables
from serial import Serial

NTSERVER = "10.63.28.2"
SERIAL_PORT = "/dev/cu.usbmodem411"

# serial port setup
arduino = Serial(SERIAL_PORT, 9600)
# opening connection causes Arduino to reset and not recive data for a few seconds
sleep(2)

def update_values(table, key, value, isNew):
    binStr = ''.join(['1' if x else '0' for x in value])
    arduino.write(bytes([int(binStr, base=2)]))
    print(binStr)

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
