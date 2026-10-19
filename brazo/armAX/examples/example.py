import sys
# Añadir el directorios build al PATH de Python
sys.path.append('/home/robousr/ROS2Dev/dynamixel-ctrl/brazo/armAX/build')
import DynamixelManager
import armAX

# ----->> Se tiene que declarar y conectar un objeto manager <<-----
baudrate = 1000000
device = "/dev/ttyUSB0"
protocol = 1.0
manager = DynamixelManager.DynamixelManager(device, baudrate, protocol)
manager.connect()

#  ----->> Se usa el objeto manager como puntero para que el brazo maneje los motores <<-----
brazo = armAX.armAX(manager)
# Conectar con el brazo hace una evaluacion de los valores y configuraciones actuales d los motores, si falla retorna "false"
print(brazo.connect())
# Se usa este metodo para obtener los mensajes de error o de exito de la operacion de los motores.
print(brazo.get_messages())

# fin del programa, se desconecta el objeto manager
manager.disconnect()