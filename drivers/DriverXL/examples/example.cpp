#include "DynamixelXLControl.h"
#include "DynamixelManager.h"
#include <unistd.h>
#include <string>
#include <iostream>
#include <vector>

int main() {
    // ----->> Se tiene que declarar y conectar un objeto manager <<-----
    const int baudrate = 1000000;
    const std::string device = "/dev/ttyUSB0";
    const float protocol = 1.0;
    DynamixelManager puerto(device,baudrate,protocol);
    puerto.connect();

    //  ----->> Se usa el objeto manager como puntero para que el motor pueda usarlo <<-----
    
    // fin del programa, se desconecta el objeto manager
    puerto.disconnect();
    return 0;
}