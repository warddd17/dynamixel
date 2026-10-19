import sys
sys.path.append("/home/robousr/ROS2Dev/dynamixel-ctrl/drivers/DriverXL/build/")
import DynamixelManager
import DynamixelXLControl


# ----->> Se tiene que declarar y conectar un objeto manager <<-----
baudrate = 1000000
device = "/dev/ttyUSB0"
protocol = 1.0
manager = DynamixelManager.DynamixelManager(device, baudrate, protocol)
manager.connect()

# ----->> Se usa el objeto manager para que el motor pueda usarlo <<-----


#fin del programa, se desconecta el objeto manager.s
manager.disconnect()