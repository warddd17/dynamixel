#include "DynamixelManagerP1.h"
#include <unistd.h>
#include <string>
#include <iostream>

int main() {
    //###---------------------->> Codigo de ejemplo <<----------------------###//
    // Clase probada con dynamixel AX-12A y AX-18A
    //---------------------->> Metodos de conexion <<----------------------//
    const int baudrate = 1000000;
    const std::string device = "/dev/ttyUSB0";
    DynamixelManagerP1 puerto(device,baudrate);
    std::cout<<puerto.connect()<<"\n";
    std::cout<<puerto.isConnect()<<"\n";
    //---------------------->> Metodos de conexion a motor <<----------------------//
    std::cout<<puerto.pingServo(1)<<"\n";
    //---------------------->> Getters <<----------------------//
    std::cout<<puerto.get_baudrate()<<"\n";
    std::cout<<puerto.get_dxl_comm_result()<<"\n";
    std::cout<<puerto.get_dxl_error()<<"\n";
    std::cout<<puerto.get_port()<<"\n";
    //---------------------->> Operaciones de lectura <<----------------------//
        // unica lectura de un byte.
    std::cout<<puerto.read1byte(1, 6)<<"\n";
        // unica lectura de dos bytes.
    std::cout<<puerto.read2byte(1, 6)<<"\n";
        // lecturas multiples
    std::vector<uint8_t> ids = {1,2,3,4,5};
    uint16_t address = 30;
    uint8_t data_length = 2;
    std::map<uint8_t, std::vector<uint8_t>> out_data = puerto.readMultiple(ids, address, data_length);
    if (!out_data.empty()) {
        if (out_data.empty()) {
            std::cerr << "Éxito pero ningún motor respondió" << std::endl;
        } else {
            for (const auto& [id, data] : out_data) {
                if (data.size() >= 2) {  // Verificar que tenemos suficientes datos
                    uint16_t position = (data[1] << 8) | data[0];
                    std::cout << "Motor " << static_cast<int>(id) 
                            << ": Posición = " << position << std::endl;
                } else {
                    std::cerr << "Datos insuficientes para el motor " 
                            << static_cast<int>(id) << std::endl;
                }
            }
        }
    } else {
        std::cerr << "Error en la comunicación con los motores" << std::endl;
    }
    //---------------------->> Operaciones de escritura <<----------------------//
        // unica escritura de un byte.
    std::cout<<puerto.write1byte(1, 24, 1)<<"\n";
        // unica escritura de dos byte.
    std::cout<<puerto.write2byte(1, 32, 100)<<"\n";
    std::cout<<puerto.write2byte(1, 30, 500)<<"\n";
        // escrituras multiples.
            // Preparación de los valores
    std::vector<std::vector<uint8_t>> values1 = {{1},{1},{1},{1},{1}}; // valores para activar el torque
    std::vector<std::vector<uint8_t>> values2 = {{100,0},{100,0},{100,0},{100,0},{100,0}}; // se establece una velocidad de 100
    std::vector<std::vector<uint8_t>> values3 = {{0,2},{0,2},{0,2},{0,2},{0,2}}; // se establece en la posicion 512 (0x0200)
            // Activacion del torque.
    std::cout<<puerto.writeMultiple(ids, 24, 1, values1)<<"\n";
            // Se modifica la velocidad de movimiento
    std::cout<<puerto.writeMultiple(ids, 32, 2, values2)<<"\n";
            // Se da la posicion objetivo
    std::cout<<puerto.writeMultiple(ids, 30, 2, values3)<<"\n";
    
    
    //fin del programa, se desconecta y libera el puerto
    puerto.disconnect();
    return 0;
}