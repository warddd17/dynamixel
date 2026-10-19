/**
 * @file DynamixelXLControl.cpp
 * @brief Implementación de la clase DynamixelXLControl (control de motores Dynamixel XL430-W250-T).
 * @details Contiene la lógica de bajo nivel para comunicación, conversión de unidades y manejo de modos.
 * @author Jose Manuel Plascencia Ramos
 * @date 2025-04-01
 * @version 1.0 
 */

//Invocacion de librerias
#include <stdexcept>
#include <string>
#include <math.h>
#include <cmath>
#include "DynamixelXLControl.h"

// Direcciones para los registros de servomotor Dynamixel XL430-W250-T
   //Direcciones en EEPROM
#define ADDR_MODEL_NUMBER 0                         //REGISTROS DE SOLO LECTURA
#define ADDR_MODEL_INFORMATION 2                    //REGISTROS DE SOLO LECTURA
#define ADDR_FIRMWARE_VERSION 6                     //REGISTROS DE SOLO LECTURA1
#define ADDR_ID 7                                   // 0 ~ 252 
#define ADDR_BAUDRATE 8                             // 0 ~ 7
#define ADDR_RETURN_DELAY_TIME 9                    // 0 a 254
#define ADDR_DRIVE_MODE 10                          // 0 ~ 13
#define ADDR_OPERATING_MODE 11                      // 0 ~ 16
#define ADDR_SECONDARY_ID 12                        // 0 ~ 252
#define ADDR_PROTOCOL_TYPE 13                       // 1 O 2
#define ADDR_HOMING_OFFSET 20                       // -1,044,479 ~ 1,044,479
#define ADDR_MOVING_THRESHOLD 24                    // 0 ~ 1023
#define ADDR_TEMPERATURE_LIMIT 31                   // 0 ~ 100 
#define ADDR_MAX_VOLTAJE_LIMIT 32                   // 60 ~ 140
#define ADDR_MIN_VOLTAJE_LIMIT 34                   // 60 ~ 140
#define ADDR_PWM_LIMIT 36                           // 0 ~ 885
#define ADDR_VELOCITY_LIMIT 44                      // 0 ~ 1023
#define ADDR_MAX_POSITION_LIMIT 48                  // 0 ~ 4095
#define ADDR_MIN_POSITION_LIMIT 52                  // 0 ~ 4095
#define ADDR_STARTUP_CONFIGURATION 60               // 3
#define ADDR_SHUTDOWN 63                            //
    // Direcciones en RAM
#define ADDR_TORQUE_ENABLE 64                       // 0 ~ 1
#define ADDR_LED 65                                 // 0 ~ 1
#define ADDR_STATUS_RETURN_LEVEL 68                 // 0 ~ 2
#define ADDR_REGISTERED_INSTRUCTION 69              // REGISTROS DE SOLO LECTURA
#define ADDR_HARDWARE_ERROR_STATUS 70               // REGISTROS DE SOLO LECTURA
#define ADDR_VELOCITY_I_GAIN 76                     // 0 ~ 16,383
#define ADDR_VELOCITY_P_GAIN 78                     // 0 ~ 16,383
#define ADDR_POSITION_D_GAIN 80                     // 0 ~ 16,383
#define ADDR_POSITION_I_GAIN 82                     // 0 ~ 16,383
#define ADDR_POSITION_P_GAIN 84                     // 0 ~ 16,383
#define ADDR_FEEDFORWARD_2ND_GAIN 88                // 0 ~ 16,383
#define ADDR_FEEDFORWARD_1ST_GAIN 90                // 0 ~ 16,383
#define ADDR_BUS_WATCHDOG 98                        // 1 ~ 127 (20 msec)
#define ADDR_GOAL_PWM 100                           // -PWM_Limit ~ PWM_Limit (0.113%)
#define ADDR_GOAL_VELOCITY 104                      // -Velocity_Limit ~ Velocity_Limit (0.229 rev/min)
#define ADDR_PROFILE_ACCELERATION 108               // 0 ~ 32,767 (214.577 rev/min² o 1 ms)
#define ADDR_PROFILE_VELOCITY 112                   // 0 ~ 32,767 (0.229 rev/min)
#define ADDR_GOAL_POSITION 116                      // Min_Position_Limit ~ Max_Position_Limit (1 pulse)
#define ADDR_REALTIME_TICK 120                      // Solo lectura (1 msec)
#define ADDR_MOVING 122                             // Solo lectura (0 ~ 1)
#define ADDR_MOVING_STATUS 123                      // Solo lectura
#define ADDR_PRESENT_PWM 124                        // Solo lectura
#define ADDR_PRESENT_LOAD 126                       // Solo lectura (-1,000 ~ 1,000, 0.1%)
#define ADDR_PRESENT_VELOCITY 128                   // Solo lectura (0.229 rev/min)
#define ADDR_PRESENT_POSITION 132                   // Solo lectura (1 pulse)
#define ADDR_VELOCITY_TRAJECTORY 136                // Solo lectura (0.229 rev/min)
#define ADDR_POSITION_TRAJECTORY 140                // Solo lectura (1 pulse)
#define ADDR_PRESENT_INPUT_VOLTAGE 144              // Solo lectura (0.1 V)
#define ADDR_PRESENT_TEMPERATURE 146                // Solo lectura (1 °C)
#define ADDR_BACKUP_READY 147                       // Solo lectura (0 ~ 1)
    // Direcciones indirectas 
#define ADDR_INDIRECT_ADDRESS_RANGE_1 168-222       // 64 ~ 661
#define ADDR_INDIRECT_DATA_RANGE_1 224-251          // 0 ~ 255 
#define ADDR_INDIRECT_ADDRESS_RANGE_2 578-632       // 64 ~ 661
#define ADDR_INDIRECT_DATA_RANGE_2 632-661          // 0 ~ 255 
// Modos de operacion
#define VELOCITY_CONTROL_MODE 1
#define POSITION_CONTROL_MODE 3
#define EXTENDED_POSITION_CONTROL_MODE 4
#define PWD_CONTROL_MODE 16
// Opciones de configuracion
#define SECONDARY_ID_OFF 255

// Limites de operacion
#define MAX_POSITION_MODE3 4095
#define MIN_POSITION_MODE3 0
#define MAX_VELOCITY_MODE1 1023
#define MIN_VELOCITY_MODE1 -1023
// Mensaje de identificacion del motor
#define HEADER_MESSAGE "[ID: "+std::to_string(this->idMotor)+"] "

    // ------------------------- Constructores y Destructor -------------------------
    DynamixelXLControl::DynamixelXLControl(DynamixelManagerP2* manager, int idMotor):control(manager), idMotor(idMotor){}
    
    DynamixelXLControl::~DynamixelXLControl(){}

    // ------------------------- Getters -------------------------
    std::string DynamixelXLControl::get_message(){    return this->sMessage;    }

    int DynamixelXLControl::get_id(){    return this->idMotor;    }

    int DynamixelXLControl::get_shadowID(){    return this->shadow_Id;    }

    // ------------------------- Configuracion y conexion -------------------------
    
    // ------------------------- Movimiento -------------------------
    // ------------------------- Lectura de estados -------------------------
    // ------------------------- Funciones de conversion -------------------------
    int DynamixelXLControl::convertAngleTOint(float angle, bool degrees){

    }

    float DynamixelXLControl::convertIntTOangle(int value){

    }

    int DynamixelXLControl::convertRADStoInt(float radps){

    }

    float DynamixelXLControl::convertINTtoRADS(int value){

    }

    // ------------------------- Validacion y limites -------------------------
    int DynamixelXLControl::clamp(int value, int minLimit, int maxLimit){
        return (value < minLimit) ? minLimit : (value > maxLimit) ? maxLimit : value;
    }

    bool DynamixelXLControl::setCWAngleLimit(int nlimit){

    }

    bool DynamixelXLControl::setCCWAngleLimit(int nlimit){

    }

