/**
 * @file armAX.cpp
 * @brief Implementacion de la clase dynamixel armAX.
 * @author Jose Manuel Plascencia Ramos
 * @date 2025-04-01
 * @version 1.0
 */

#include "armAX.h"

#define MOTORS_N 5
    // ------------------------- Constructor y Destructor -------------------------
    armAX::armAX(DynamixelManager* puerto):manager(puerto){
        motors={
            { 1 , DynamixelAXControl( this->manager , 1 ) },
            { 2 , DynamixelAXControl( this->manager , 2 ) },
            { 3 , DynamixelAXControl( this->manager , 3 ) },
            { 4 , DynamixelAXControl( this->manager , 4 ) },
            { 5 , DynamixelAXControl( this->manager , 5 ) }
        };

    }

    armAX::~armAX(){}
    
    // ------------------------- Getters -------------------------
    std::string armAX::get_messages(){
        return messages;
    }
    
    // ------------------------- Conexion -------------------------
    bool armAX::connect(bool home){
        messages.clear();
        for(auto& p : motors){
            if(p.first == 1 || p.first == 2 || p.first == 3 || p.first == 4){
                if(!p.second.setJointMode(1.5707963268,-1.5707963268))
                    return false;
            }else if(p.first == 5){
                if(!p.second.setJointMode(2.617993878,0))
                    return false;
            }
        }
        for (auto& p : motors){
            if(!p.second.connect(home))
                return false;
            messages.append(p.second.get_message());
        }
        return true;
    }
    
    bool armAX::pingServo(int id){
        messages.clear();
        return manager->pingServo(id);
    }

    // ------------------------- Movimiento -------------------------
    bool armAX::setInHomePosition(float time){
        messages.clear();
        for(auto& p : motors){
            if(!p.second.setPositionInTime(0,time)){
                messages.append(p.second.get_message());   
                return false;
            }
            messages.append(p.second.get_message());
        }
        return true;
    }

    bool armAX::setOnePosition(int id, float position, float time){
        messages.clear();
        if(!motors.at(id).setPositionInTime(position, time)){
            messages.append(motors.at(id).get_message()); 
            return false;
        }
        messages.append(motors.at(id).get_message());   
        return true;
    }

    bool armAX::setAllPosition(std::vector<float> positions, std::vector<float> times){
        messages.clear();
        int i = 0;
        if(positions.size() > MOTORS_N || times.size() > MOTORS_N)
            return false;
        for(auto& m : motors){
            if(!m.second.setPositionInTime(positions[i], times[i])){
                messages.append(m.second.get_message());    
                return false;
            }
            messages.append(m.second.get_message());    
            i++;
        }
        return true;
    }

    bool armAX::setPositionArray(std::vector<int> ids, std::vector<float> positions, std::vector<float> times){
        messages.clear();
        int i = 0;
        if((ids.size() == positions.size()) && (times.size() == positions.size())){
            for(int m : ids){
                if(!motors.at(m).setPositionInTime(positions[i], times[i])){
                    messages.append(motors.at(m).get_message());    
                    return false;
                }
                messages.append(motors.at(m).get_message());    
                i++;
            }
            return true;
        }else 
            return false;
    }

    // ------------------------- Informacion -------------------------
    float armAX::getOnePosition(int id){
        messages.clear();
        messages.append(motors.at(id).get_message()); 
        return motors.at(id).getPosition();
    }

    std::vector<float> armAX::getAllPosition(){
        messages.clear();
        std::vector<float> positions;
        for(auto& p : motors){
            positions.push_back(p.second.getPosition());
            messages.append(p.second.get_message());
        }
        return positions;
    }

    std::vector<float> armAX::getPositionArray(std::vector<int> ids){
        messages.clear();
        std::vector<float> positions;
        for(int i: ids){
            positions.push_back(motors.at(i).getPosition());
            messages.append(motors.at(i).get_message());
        }
        return positions;
    }