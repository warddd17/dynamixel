/**
 * @file DynamixelAXControl.cpp
 * @brief Implementación de la clase DynamixelAXControl (control de motores Dynamixel AX).
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
#include "DynamixelAXControl.h"

// Direcciones para los registros de servomotores AX-12A/AX-18A
   //Direcciones en EEPROM
#define ADDR_MODEL_NUMBER 0             //REGISTROS DE SOLO LECTURA
#define ADDR_FIRMWARE_VERSION 2         //REGISTROS DE SOLO LECTURA
#define ADDR_ID 3 // De 0 a 253 
#define ADDR_BAUDRATE 4 // De 0 a 254
#define ADDR_RETURN_DELAY_TIME 5 // De 0 a 254
#define ADDR_CW_ANGLE_LIMIT 6 // De 0 a 1023
#define ADDR_CCW_ANGLE_LIMIT 8 // De 0 a 1023
#define ADDR_MAX_TEMPERATURE 11 // De 0 a 1023
#define ADDR_MIN_VOLTAJE 12 // De 0 a 1023
#define ADDR_MAX_VOLTAJE 13 // De 0 a 1023
#define ADDR_MAX_TORQUE 14 // De 0 a 1023
#define ADDR_STATUS_RETURN_LIMIT 16 // De 0 a 2
#define ADDR_ALARM_LED 17 // De 0 a 255
#define ADDR_SHUTDOWN 18 
    //Direcciones en RAM
#define ADDR_TORQUE_ENABLE 24 
#define ADDR_LED 25 
#define ADDR_GOAL_POSITION 30 // De 0 a 1023
#define ADDR_MOVING_SPEED 32 // De 0 a 1023 en modo articulacion de 0 a 2047 en modo rueda
#define ADDR_TORQUE_LIMIT 34 // De 0 a 1023
#define ADDR_PRESENT_POSITION 36        //REGISTROS DE SOLO LECTURA
#define ADDR_PRESENT_SPEED 38           //REGISTROS DE SOLO LECTURA
#define ADDR_PRESENT_LOAD 40            //REGISTROS DE SOLO LECTURA
#define ADDR_PRESENT_VOLTAJE 42         //REGISTROS DE SOLO LECTURA
#define ADDR_PRESENT_TEMPERATURE 43     //REGISTROS DE SOLO LECTURA
#define ADDR_REGISTERED 44              //REGISTROS DE SOLO LECTURA
#define ADDR_MOVING 46                  //REGISTROS DE SOLO LECTURA
#define ADDR_LOCK 47 // De 0 o 1
#define ADDR_PUNCH 48 // De 0 a 1023
// Parametros de funcionamiento
#define MAX_ANGLE_LIMIT 1023
#define MIN_ANGLE_LIMIT 0
#define MIN_SPEED 0
#define MAX_JOINT_SPEED 1023
#define MAX_WHEEL_SPEED 2047
#define HEADER_MESSAGE "[ID: "+std::to_string(this->idMotor)+"] "

    // ------------------------- Constructores y Destructor -------------------------
    DynamixelAXControl::DynamixelAXControl(DynamixelManagerP1* manager, int idMotor):control(manager){
        bWheelMode = false;
        if(idMotor<0){
            bInvertido = true;
            this->idMotor = idMotor *-1;
        }else{
            bInvertido = false;
            this->idMotor = idMotor;
        }
    }
    
    DynamixelAXControl::~DynamixelAXControl(){}
    
    // ------------------------- Getters -------------------------
    std::string DynamixelAXControl::get_message(){
        return this -> sMessage;
    }
    
    int DynamixelAXControl::get_id(){
        return this->idMotor;
    }

    // ------------------------- Configuracion y conexion -------------------------
    bool DynamixelAXControl::connect(bool home){
        if(!control -> isConnect()){
            sMessage.append("Connection not available\n");
            return false;
        }
        if(!control -> pingServo(idMotor)){
            sMessage.assign("ERROR: " HEADER_MESSAGE + "not found!\n");
            return false;
        }
        nCwlimit = control->read2byte(idMotor, ADDR_CW_ANGLE_LIMIT);
        nCcwlimit = control->read2byte(idMotor, ADDR_CCW_ANGLE_LIMIT);
        if(!setSpeed(1))
            sMessage.assign(HEADER_MESSAGE + "Fail set initial Speed\n");
        if(home){
            if(!setHome())
                sMessage.assign(HEADER_MESSAGE + "Fail to set home position\n");
        }
        if(nCwlimit < 0 || nCcwlimit < 0){
            sMessage.assign(HEADER_MESSAGE + "Fail to evaluate mode.\n");   
            return false;
        }else if(nCcwlimit == 0 && nCwlimit == 0){
            bWheelMode = true;
            sMessage.assign(HEADER_MESSAGE + "in wheel mode.\n");
        }else{
            bWheelMode = false;
            sMessage.assign(HEADER_MESSAGE + "Joint mode to "+std::to_string(convertIntTOangle(nCwlimit))+" at "+std::to_string(convertIntTOangle(nCcwlimit))+".\n");
        }
        sMessage.append(HEADER_MESSAGE+ "Current speed: "+std::to_string(convertINTtoRADS(control->read2byte(idMotor, ADDR_MOVING_SPEED)))+".\n");
        return true;
    }

    bool DynamixelAXControl::setWheelMode(){
        if (!control -> write1byte(idMotor,ADDR_TORQUE_ENABLE, 0)) {
            sMessage.assign(HEADER_MESSAGE +"Failed to disable torque!\n");
            return false;
        }
        if(!setCWAngleLimit(0))
            return false;
        if(!setCCWAngleLimit(0))
            return false;
        if (!control -> write1byte(idMotor, ADDR_TORQUE_ENABLE, 1)) {
            sMessage.assign(HEADER_MESSAGE +"Failed to enable torque!\n");
            return false;
        }
        sMessage.assign(HEADER_MESSAGE +"Succeeded to set in wheel mode.\n");
        bWheelMode = true;
        return true;
    }
    
    bool DynamixelAXControl::setJointMode(float cwLimit, float ccwLimit, bool degrees){
        if (!control -> write1byte(idMotor, ADDR_TORQUE_ENABLE, 0)) {
            sMessage.assign(HEADER_MESSAGE +"Failed to disable torque!\n");
            return false;
        }
        if(!setCWAngleLimit(convertAngleTOint(cwLimit, degrees)))
            return false;
        if(!setCCWAngleLimit(convertAngleTOint(ccwLimit, degrees)))
            return false;
        if (!control -> write1byte(idMotor, ADDR_TORQUE_ENABLE, 1)) {
            sMessage.assign(HEADER_MESSAGE +"Failed to enable torque!\n");
            return false;
        }
        sMessage.assign(HEADER_MESSAGE +"Succeeded to set in joint mode.\n");
        bWheelMode = false;
        return true;
    }
    
    // ------------------------- Movimiento -------------------------
    bool DynamixelAXControl::setPosition(float position, bool degrees){
        if(this->isMoving()){
            sMessage.assign(HEADER_MESSAGE +"Present movement not complete.\n");
            return false;
        }
        if(bInvertido)
            position = -position;
        uint16_t goalPosition = convertAngleTOint(position, degrees);
        if(this -> bWheelMode){
            sMessage.assign(HEADER_MESSAGE +"Not posible to configure.\n");
            return false;
        }
        if(goalPosition > nCcwlimit || goalPosition < nCwlimit){
            sMessage.assign(HEADER_MESSAGE +"Value out of range!\n");
        }
        goalPosition = (uint16_t) clamp(goalPosition,nCwlimit,nCcwlimit);
        if (!control -> write2byte(idMotor,ADDR_GOAL_POSITION, goalPosition)) {
            sMessage.assign(HEADER_MESSAGE +"Failed to set position!\n");
            return false;
        }
        sMessage.assign(HEADER_MESSAGE +"set in position: "+ std::to_string(convertIntTOangle(goalPosition)) +".\n");
        return true;
    }
    
    bool DynamixelAXControl::setPositionInTime(float position, float time, bool degrees){
        if(this->isMoving()){
            sMessage.assign(HEADER_MESSAGE +"Present movement not complete.\n");
            return false;
        }
        if(bInvertido)
            position = -position;
        if(!calculateSpeed(position, time)){
            sMessage.assign(HEADER_MESSAGE +"Not posible to configure.\n");
            return false;
        }
        uint16_t goalPosition = convertAngleTOint(position, degrees);
        if(this -> bWheelMode){
            sMessage.assign(HEADER_MESSAGE +"Not posible to configure.\n");
            return false;
        }
        if(goalPosition > nCcwlimit || goalPosition < nCwlimit){
            sMessage.assign(HEADER_MESSAGE +"Value out of range!\n");
        }
        goalPosition = (uint16_t) clamp(goalPosition,nCwlimit,nCcwlimit);
        if (!control -> write2byte(idMotor,ADDR_GOAL_POSITION, goalPosition)) {
            sMessage.assign(HEADER_MESSAGE +"Failed to set position!\n");
            return false;
        }
        sMessage.assign(HEADER_MESSAGE +"set in position: "+ std::to_string(convertIntTOangle(goalPosition)) +".\n");
        return true;
        return true;
    }

    bool DynamixelAXControl::setSpeed(float speed){
        uint16_t goalSpeed = convertRADStoInt(speed);
        if(this -> bWheelMode){
            if(goalSpeed > MAX_WHEEL_SPEED || goalSpeed < MIN_SPEED){
                sMessage.assign(HEADER_MESSAGE +"Value out of range!\n");
            }
            goalSpeed = (uint16_t) clamp(goalSpeed,MIN_SPEED,MAX_WHEEL_SPEED);
        }else{
            if(goalSpeed > MAX_JOINT_SPEED || goalSpeed < MIN_SPEED){
                sMessage.assign(HEADER_MESSAGE +"Value out of range!\n");
            }
            goalSpeed = (uint16_t) clamp(goalSpeed,MIN_SPEED,MAX_JOINT_SPEED);
        }
        if (!control -> write2byte(idMotor,ADDR_MOVING_SPEED, goalSpeed)) {
            sMessage.assign(HEADER_MESSAGE +"Failed to set speed!\n");
            return false;
        }
        sMessage.assign(HEADER_MESSAGE +"set speed to "+ std::to_string(convertINTtoRADS(goalSpeed)) +".\n");
        return true;
    }
    
    bool DynamixelAXControl::setHome(){
        return setPosition(0);
    }

    // ------------------------- Lectura de estados -------------------------
    float DynamixelAXControl::getPosition(){
        float position = control -> read2byte(idMotor,ADDR_PRESENT_POSITION);
        if (position != -1) 
            sMessage.assign(HEADER_MESSAGE +"Current position: "+std::to_string(convertIntTOangle(position))+" \n");
         else 
            sMessage.assign(HEADER_MESSAGE +"Error getting present position.\n");
        return convertIntTOangle(position);
    }
    
    float DynamixelAXControl::getSpeed(){
        int speed = control -> read2byte(idMotor,ADDR_PRESENT_SPEED);
        if (speed != -1) 
            sMessage.assign(HEADER_MESSAGE +"Current speed: "+std::to_string(convertINTtoRADS(speed))+" rad/s.\n");
        else 
            sMessage.assign(HEADER_MESSAGE +"Error getting present speed.\n");
        return convertINTtoRADS(speed);
    }
    
    /// @warning no se han implementado funciones de conversion retorna valores del motor.
    int DynamixelAXControl::getVoltaje(){
        int voltaje = control -> read1byte(idMotor, ADDR_PRESENT_VOLTAJE);
        if (voltaje != -1) {
            sMessage.assign(HEADER_MESSAGE +"Present voltaje: "+std::to_string(voltaje)+" \n");
        } else {
            sMessage.assign(HEADER_MESSAGE +"Error getting present voltaje.\n");
        }
        return voltaje;
    }
    
    /// @warning no se han implementado funciones de conversion retorna valores del motor.
    int DynamixelAXControl::getTemperature(){
        int temperature = control -> read1byte(idMotor, ADDR_PRESENT_TEMPERATURE);
        if (temperature != -1) {
            sMessage.assign("[ID: "+std::to_string(this->idMotor)+"] Present temperature: "+std::to_string(temperature)+" \n");
        } else {
            sMessage.assign("[ID: "+std::to_string(this->idMotor)+"] Error getting present temperature.\n");
        }
        return temperature;
    }
    
    bool DynamixelAXControl::isMoving(){
        int moving = control -> read1byte(idMotor,ADDR_MOVING);
        if (moving == -1){
            sMessage.assign(HEADER_MESSAGE +"Error getting moving.\n");
        }else if(moving == 0){
            sMessage.assign(HEADER_MESSAGE +"is not moving.\n");
            return false;
        }
        sMessage.assign(HEADER_MESSAGE +"is moving.\n");
        return true;
    }

    // ------------------------- Validacion y limites -------------------------
    int DynamixelAXControl::clamp(int value, int minLimit, int maxLimit){
        return (value < minLimit) ? minLimit : (value > maxLimit) ? maxLimit : value;
    }
    
    bool DynamixelAXControl::calculateSpeed(float goal_angle, float time){
        float currentPosition = this->getPosition();
        float distance = goal_angle - currentPosition;
        float speed;
        if(distance < 0)
            distance = -(distance);
        speed = distance/time;
        if(!this->setSpeed(speed))
            return false;
        return true;
    }

    bool DynamixelAXControl::setCWAngleLimit(int limit){
        uint16_t cwLimit = 0;
        if(cwLimit > MAX_ANGLE_LIMIT || cwLimit < MIN_ANGLE_LIMIT){
            sMessage.assign(HEADER_MESSAGE +"Value out of range!\n");
        }
        cwLimit = (uint16_t) clamp(limit,MIN_ANGLE_LIMIT,MAX_ANGLE_LIMIT);
        if (!control -> write2byte(idMotor,ADDR_CW_ANGLE_LIMIT,cwLimit)){
            sMessage.assign(HEADER_MESSAGE +"Failed to set cw angle limit!\n");
            return false;
        }
        nCwlimit = cwLimit;
        return true;
    }

    bool DynamixelAXControl::setCCWAngleLimit(int limit){
        uint16_t ccwLimit = 0;
        if(ccwLimit > MAX_ANGLE_LIMIT || ccwLimit < MIN_ANGLE_LIMIT){
            sMessage.assign(HEADER_MESSAGE +"Value out of range!\n");
            return false;
        }
        ccwLimit = (uint16_t) clamp(limit,MIN_ANGLE_LIMIT,MAX_ANGLE_LIMIT);
        if (!control -> write2byte(idMotor, ADDR_CCW_ANGLE_LIMIT,ccwLimit)){
            sMessage.assign(HEADER_MESSAGE +"Failed to set cw angle limit!\n");
            return false;
        }
        nCcwlimit = ccwLimit;
        return true;
    }

    // ------------------------- Funciones de conversion -------------------------
    int DynamixelAXControl::convertAngleTOint(float angle, bool degrees){
        if(degrees)
            angle = angle * (180.0 / M_PI);
        if(angle == 0 )
            return 512;
        angle = angle/0.0051182676011730;
        return round(512 - angle);
    }

    float DynamixelAXControl::convertIntTOangle(int value){
        float correction = (512-value)*0.000004998;
        if(correction < 0){correction=-(correction);}
        return (((512-value) * 0.0051182676011730)-correction);
    }

    int DynamixelAXControl::convertRADStoInt(float radps){
         if(radps == 0)
            return 0;
        else if(radps < 0)
            return -((radps/0.01162392)+1023);
        return radps/0.01162392;
    }

    float DynamixelAXControl::convertINTtoRADS(int value){
        if(value > 1024)
            return -(value-1024)*0.01162392;
        return value*0.01162392;
    }

