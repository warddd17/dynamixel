#include "DynamixelAXControl.h"
#include "DynamixelManagerP1.h"
#include <unistd.h>
#include <string>
#include <iostream>
#include <vector>

int main() {
    // ----->> Se tiene que declarar y conectar un objeto manager <<-----
    const int baudrate = 1000000;
    const std::string device = "/dev/ttyUSB0";
    DynamixelManagerP1 puerto(device,baudrate);
    puerto.connect();

    //  ----->> Se usa el objeto manager como puntero para que el motor pueda usarlo <<-----
    DynamixelAXControl motor(&puerto, 1);
    // Conecttar con el motor hace una evaluacion de los valores y configuraciones actuales del motor, si falla retorna "false"
    std::cout<<motor.connect();
    // Se usa este metodo para obtener el mensaje de error o de exito de la operacion del motor.
    std::cout<<motor.get_message();
    
    // fin del programa, se desconecta el objeto manager
    puerto.disconnect();
    return 0;
}