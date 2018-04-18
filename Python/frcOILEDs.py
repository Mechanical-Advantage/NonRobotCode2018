#!/usr/bin/env python3
from time import sleep

from networktables import NetworkTables
from serial import Serial, SerialException

NTSERVER = "127.0.0.1"
SERIAL_PORT = "/dev/cu.usbmodem26231"
SET_BITS = 2

last_value = []
arduino_connected = False

def connect_to_arduino():
    # serial port setup
    try:
        global arduino
        arduino = Serial(SERIAL_PORT, 9600)
        # opening connection causes Arduino to reset and not recive data for a few seconds
        sleep(2)
        global arduino_connected
        arduino_connected = True
        print("Connected to Arduino")
        global last_value
        last_value = [] # Force resend of all LEDs
        global table
        try:
            update_values(table, "OI LEDs", table.getBooleanArray("OI LEDs"), False)
        except NameError:
            # Before NT init
            pass
    except SerialException:
        pass
        
def update_values(table, key, value, isNew):
    global last_value
    global arduino_connected
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
    try:
        arduino.write(bytes(serial_data))
    except SerialException:
        if arduino_connected:
            arduino_connected = False
            print("Lost connection to Arduino")
    except NameError:
        # Arduino has not been connected to
        pass

connect_to_arduino()

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
while True:
    sleep(2)
    if not lastStateConnected and NetworkTables.getRemoteAddress() is not None:
        lastStateConnected = True
        print("Re-connected to NetworkTables")
    elif lastStateConnected and NetworkTables.getRemoteAddress() is None:
        lastStateConnected = False
        print("Lost connection to NetworkTables")

    if not arduino_connected:
        connect_to_arduino();
