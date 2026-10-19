# Se agrega temporalmente al path el modulo para que pueda ser detectado.
import sys
### !!! Cambia la ruta dependiendo de tu propio sistema o añadelo al path general
sys.path.append("/home/robousr/ROS2Dev/dynamixel-ctrl/drivers/DynamixelManagerP1/build")
import DynamixelManagerP1

###//---------------------->> Codigo de ejemplo <<----------------------//###
## Modulo probado con dynamixel AX-12A y AX-18A
#//---------------------->> Metodos de conexion <<----------------------//#
baudrate = 1000000
device = "/dev/ttyUSB0"
manager = DynamixelManagerP1.DynamixelManagerP1(device, baudrate)
print(manager.connect())
print(manager.isConnect())
#//---------------------->> Métodos de conexión a motor <<----------------------//#
print(manager.pingServo(1))
#//---------------------->> Getters <<----------------------//#
print(manager.get_baudrate())
print(manager.get_dxl_comm_result())
print(manager.get_dxl_error())
print(manager.get_port())
#//---------------------->> Operaciones de lectura <<----------------------//#
    # Lectura única de un byte
print(manager.read1byte(1, 6))
print(manager.read1byte(1, 7))
    # Lectura única de dos bytes
print(manager.read2byte(1, 6))
print(manager.read2byte(1, 30))
    # Lecturas múltiples
ids = [1,2,3,4,5]
address = 30
data_length = 2
out_data = manager.readMultiple(ids, address, data_length)
if len(out_data) == 0:
    if not out_data:
        print("Éxito pero ningún motor respondió", file=sys.stderr)
    else:
        for id, data in out_data.items():
            if len(data) >= 2:  # Verificar que tenemos suficientes datos
                position = (data[1] << 8) | data[0]
                print(f"Motor {id}: Posición = {position}")
            else:
                print(f"Datos insuficientes para el motor {id}", file=sys.stderr)
else:
    print("Error en la comunicación con los motores", file=sys.stderr)
#//---------------------->> Operaciones de escritura <<----------------------//#
    # Escritura única de un byte
print(manager.write1byte(1, 25, 1))
print(manager.write1byte(1, 24, 1))
    # Escritura única de dos bytes
print(manager.write2byte(1, 32, 100))
print(manager.write2byte(1, 30, 500))
    # Escrituras múltiples
        # Preparación de los valores
values1 = [[1] for _ in range(5)]  # 5 motores con valor 1 (para torque)
values2 = [[100, 0] for _ in range(5)]  # 5 motores con velocidad 100
values3 = [[0, 2] for _ in range(5)]  # 5 motores con posición 512 (0x0200)
    # Activación del torque
print(manager.writeMultiple(ids, 24, 1, values1))
    # Modificación de la velocidad de movimiento
print(manager.writeMultiple(ids, 32, 2, values2))
    # Establecimiento de posición objetivo
print(manager.writeMultiple(ids, 30, 2, values3))

# Fin del programa
manager.disconnect()
