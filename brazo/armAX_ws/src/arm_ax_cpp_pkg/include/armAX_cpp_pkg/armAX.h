/**
 * @file armAX.h
 * @brief Controlador de brazo robótico con motores Dynamixel AX12-A.
 * @author Jose Manuel Plascencia Ramos
 * @date 2025-04-01
 * @version 1.0
 * @code
   int main(int argc, char const *argv[])
    {
        //Declaracion del objeto DynamixelManager
        const int baudrate = 1000000;
        const std::string device = "/dev/ttyUSB0";
        const float protocol = 1.0;
        DynamixelManager manager(device,baudrate,protocol);
        manager.connect();

        armAX brazo(&manager);
        brazo.connect();
        std::cout<<brazo.get_messages()<<std::endl;
        usleep(3000000);

        brazo.setPosition({6,1,2,3,4},{2.5,1.5,1,-1,-1},{2,4,3,3,3});
        usleep(4000000);

        for(float i : brazo.getPosition({0}) )
            std::cout<< i << " ";
        std::cout<<brazo.get_messages()<<std::endl;
        //fin
        manager.disconnect();
        return 0;
    }
 * @endcode
 */

#ifndef ARM_AX
#define ARM_AX

//declaracion de librerias.
#include "DynamixelAXControl.h"
#include "DynamixelManager.h"
#include <vector>

/**
 * @class armAX
 * @brief Controlador de brazo robótico con motores Dynamixel de la serie AX.
 * @details Gestiona multiples motores para el manejo de un brazo robotico.
 * @warning Requiere un DynamixelManager inicializado para funcionar.
 */
class armAX{
    
    public:
        // ------------------------- Constructores y Destructor -------------------------
        /**
         * @brief Constructor principal.
         * @param puerto Puntero a un DynamixelManager ya configurado (gestión de puerto serial).
         * @note inicializa los motores que componen el robot
         * @throw std::invalid_argument Si `puerto` es nullptr.
         */
        armAX(DynamixelManager*puerto);

         /**
         * @brief Destructor: Libera recursos pero no cierra el puerto serial (manejado por DynamixelManager).
         */
        ~armAX();
        
        // ------------------------- Getters -------------------------
        /**
         * @brief Obtiene los mensajes de los motores y en caso de fallo da el mensaje de error correspondiente.
         * @return std::string Mensaje formateado (ej: "Motor ID=1: Posición alcanzada").
         */
        std::string get_messages();
        
        // ------------------------- Funciones de conexion y movimiento -------------------------
        /**
         * @brief Conecta y evalua todos los motores del brazo.
         * @param home Si es true, mueve los motores a su posición "home".
         * @return true Si todos los motores respondieron correctamente.
         */
        bool connect(bool home = false);

        /**
         * @brief Realiza un ping a un servo
         * @param id es el id asignado al servo que al que se llamara.
         * @return true Si todos los motores respondieron correctamente.
         */
        bool pingServo(int id);

        /**
         * @brief Establece todo los motores en posición 0.
         * @param ids Vector con los IDs de los motores (ej: {1, 2, 3}).
         * @return true Si la configuración fue exitosa para todos los motores.
         */
        bool setInHomePosition(float time);
        
        /**
         * @brief Mueve un motor a una posicion objetivo en un tiempo determinado
         * @param id Identificador del  motor que se quiere mover
         * @param position Vector de posiciones (en radianes).
         * @param time Vector de tiempos en segundos para cada movimiento.
         * @return true Si el comando se envió correctamente al motor.
         * @throw std::length_error Si los vectores no tienen el mismo tamaño.
         * @example
         *   arm.setPosition(1, 1.1, 2); // Mueve ID=1 a 1.1 rads en 2s.
         */
        bool setOnePosition(int id, float position, float time);

        /**
         * @brief Mueve todos motores a posiciones objetivo con tiempos sincronizados.
         * @param positions Vector de posiciones (en radianes y debe contener 5 posiciones).
         * @param times Vector de tiempos en segundos para cada movimiento (debe contener 5 tiempos).
         * @return true Si el comando se envió correctamente a todos los motores.
         * @throw std::length_error Si los vectores no tienen el mismo tamaño.
         * @example
         *   arm.setPosition( {1,1,1,1,1}, {2,2,2,2,2}); // Mueve todos los motores a 1 radian en dos segundos
         */
        bool setAllPosition(std::vector<float> positions, std::vector<float> times);

        /**
         * @brief Mueve múltiples motores a posiciones objetivo con tiempos sincronizados.
         * @param ids Vector de IDs de motores (ej: {1, 2}).
         * @param positions Vector de posiciones (en radianes).
         * @param times Vector de tiempos en segundos para cada movimiento.
         * @return true Si el comando se envió correctamente a todos los motores.
         * @throw std::length_error Si los vectores no tienen el mismo tamaño.
         * @example
         *   arm.setPosition({1, 2}, {90.0f, 45.0f}, {2.0f, 2.5f}); // Mueve ID=1 a 90° en 2s e ID=2 a 45° en 2.5s
         */
        bool setPositionArray(std::vector<int> ids, std::vector<float> positions, std::vector<float> times);
        
        /**
         * @brief Obtiene las posiciones actuales de múltiples motores.
         * @param id identificador del motor a evaluar.
         * @return float posicion en radianes
         */
        float getOnePosition(int id);

        /**
         * @brief Obtiene las posiciones actuales de múltiples motores.
         * @return std::vector<float> Posiciones de todos los motores en orden de id (en radianes).
         */
        std::vector<float> getAllPosition();

        /**
         * @brief Obtiene las posiciones actuales de múltiples motores.
         * @param ids Vector de IDs de motores (ej: {1, 2}).
         * @return std::vector<float> Posiciones en el mismo orden que `ids` (en grados o unidades nativas).
         */
        std::vector<float> getPositionArray(std::vector<int> ids);

    private: 
        std::string messages;                      ///< Buffer de mensajes de estado/error.
        std::map<int, DynamixelAXControl> motors;  ///< Mapa de controladores de motores (ID -> Controlador).
        DynamixelManager* manager;                 ///< Manejador de comunicación (no es propietario).
};
#endif