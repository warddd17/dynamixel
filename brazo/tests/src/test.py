import sys
sys.path.append('/home/robousr/ROS2Dev/dynamixel-ctrl/brazo/armAX/build')
import DynamixelManager
import armAX
import time

baudrate = 1000000
device = "/dev/ttyUSB0"
protocol = 1.0
manager = DynamixelManager.DynamixelManager(device, baudrate, protocol)
manager.connect()

brazo = armAX.armAX(manager)
print(brazo.connect(True))
print(brazo.get_messages())

# fin del programa, se desconecta el objeto manager
manager.disconnect()

def eaglePosition(timeTO = 2):
    ids = [2 ,3 ,4 ,6 ,1 ]
    positions = [-0.5 ,1.5 ,1.5 ,0,0 ]
    times = [timeTO ,timeTO ,timeTO ,1 ,timeTO ]
    brazo.setPosition(ids ,positions , times)
    time.sleep(4)
    print(brazo.get_messages())

def agarreUnCubo_izq(timeTO = 2.5):
    brazo.setPosition([4, 3, 2], [0.421426, 0.900571, 0.979], [timeTO, timeTO, timeTO])
    print(brazo.get_messages())
    time.sleep(timeTO + 0.5)

    brazo.setPosition([6],[0.614],[1])
    print(brazo.get_messages())
    time.sleep(1.5)

    brazo.setPosition([2],[0.6],[1])
    print(brazo.get_messages())
    time.sleep(1.5)

    brazo.setPosition([1, 2, 3, 4], [-0.575959, 0.872478, 1.20428, 0.645772], [timeTO, timeTO, timeTO, timeTO])
    print(brazo.get_messages())
    time.sleep(timeTO + 0.5)

    brazo.setPosition([6],[0],[1])
    print(brazo.get_messages())
    time.sleep(1.5)

def agarreUnCubo_der(timeTO = 2.5):
    brazo.setPosition([4, 3, 2],[0.421426, 0.900571, 0.979],[timeTO, timeTO, timeTO])
    print(brazo.get_messages())
    time.sleep(timeTO + 0.5)

    brazo.setPosition([6],[0.614],[1])
    print(brazo.get_messages())
    time.sleep(1.5)

    brazo.setPosition([2],[0.6],[1])
    print(brazo.get_messages())
    time.sleep(1.5)

    brazo.setPosition([1, 2, 3, 4], [0.575959, 0.872478, 1.20428, 0.645772], [timeTO, timeTO, timeTO, timeTO])
    print(brazo.get_messages())
    time.sleep(timeTO + 0.5)

    brazo.setPosition([6],[0],[1])
    print(brazo.get_messages())
    time.sleep(1.5)