import sys
# Añadir el directorios build al PATH de Python
sys.path.append("/home/robousr/ROS2Dev/dynamixel-ctrl/drivers/DriverAX/build/")
import DynamixelManagerP1
import DynamixelAXControl


# ----->> Se tiene que declarar y conectar un objeto manager <<-----
baudrate = 1000000
device = "/dev/ttyUSB0"
protocol = 1.0
manager = DynamixelManagerP1.DynamixelManagerP1(device, baudrate)
manager.connect()

# ----->> Se usa el objeto manager para que el motor pueda usarlo <<-----
motor1 = DynamixelAXControl.DynamixelAXControl(manager, 1)
# Conecttar con el motor hace una evaluacion de los valores y configuraciones actuales del motor, si falla retorna "false"
print(motor1.connect())
# Se usa este metodo para obtener el mensaje de error o de exito de la operacion del motor.
print(motor1.get_message())

#fin del programa, se desconecta el objeto manager.s
manager.disconnect()