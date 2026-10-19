#include "armAX.h"
#include "DynamixelManager.h"
#include <unistd.h>
#include <string>
#include <iostream>
#include <vector>

int main(){
    // ----->> Se tiene que declarar y conectar un objeto manager <<-----
    const int baudrate = 1000000;
    const std::string device = "/dev/ttyUSB0";
    const float protocol = 1.0;
    DynamixelManager manager(device,baudrate,protocol);
    manager.connect();

    //  ----->> Se usa el objeto manager como puntero para que el brazo maneje los motores <<-----
    armAX brazo(&manager);
    // Conectar con el brazo hace una evaluacion de los valores y configuraciones actuales d los motores, si falla retorna "false"
    std::cout<<brazo.connect();
    // Se usa este metodo para obtener los mensajes de error o de exito de la operacion de los motores.
    std::cout<<brazo.get_messages()<<std::endl;
    
    // fin del programa, se desconecta el objeto manager
    manager.disconnect();
    return 0;
}
