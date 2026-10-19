/**
 * @file DynamixelManager.h
 * @brief Control de comunicacion con motores dynamixel.
 * @author Jose Manuel Plascencia Ramos
 * @date 2025-04-01
 * @version 1.0
 * @code 
  int main() {
    const int baudrate = 1000000;
    const std::string device = "/dev/ttyUSB0";
    DynamixelManager puerto(device,baudrate);
    puerto.connect();
    puerto.write1byte(1,32,10);
    std::cout<<puerto.read1byte()<<std::endl;
    puerto.disconnect();
    return 0;
    }
 * @endcode 
 */

#ifndef DYNAMIXEL_SDK_CONTROL
#define DYNAMIXEL_SDK_CONTROL

#include "dynamixel_sdk/dynamixel_sdk.h"
#include <string>
#include <memory>

/**
 * @class DynamixelManager
 * @brief Gestiona la comunicación con motores Dynamixel (protocolos 2.0).
 * @details Proporciona métodos para inicializar puertos, enviar comandos y verificar estados.
 */
class DynamixelManagerP2
{
    public:

        // ------------------------->> Constructor y Destructor <<-------------------------
        /**
         * @brief Constructor principal para inicializar la conexión con los motores.
         * @details Inicializa los manejadores de puerto y paquetes del SDK Dynamixel.
         *          - Configura el puerto serial pero NO lo abre (usar connect() posteriormente).
         *          - El protocolo (1.0/2.0) determina el tipo de paquete usado.
         * @param sPort Nombre del puerto serial (ej: "/dev/ttyUSB0" en Linux, "COM3" en Windows).
         * @param nBaudrate Velocidad de comunicación en baudios (ej: 57600, 115200).
         * @param protocol Versión del protocolo (1.0 o 2.0).
         * @throw std::runtime_error Si falla la apertura del puerto.
         */
        DynamixelManagerP2(const std::string& sPort, int nBaudrate);
        
        /**
         * @brief Destructor: Libera recursos y cierra la conexión.
         * @details Garantiza una desconexión segura:
         *  1. Cierra la conexión serial si está activa (llamando a disconnect()).
         *  2. Libera la memoria de los manejadores del SDK. 
         * @warning No lanza excepciones para evitar problemas en el flujo de destrucción.
         */
        ~DynamixelManagerP2();

        // ------------------------->> Getters (Accesores) <<-------------------------
        /**
         * @brief Obtiene el resultado de la última operación de comunicación.
         * @return int 
         *   - `COMM_SUCCESS` (0) si la operación fue exitosa.
         *   - Código de error específico de Dynamixel en caso de fallo.
         * @see dynamixel::CommErrorCode
         */
        int get_dxl_comm_result();

        /**
         * @brief Obtiene el último error reportado por un motor Dynamixel.
         * @return uint8_t Byte de error (consultar manual Dynamixel para bits de error).
         */
        uint8_t get_dxl_error();
        
        /**
         * @brief Verifica el estado de la conexión con el puerto serial.
         * @return true Si el puerto está abierto y operativo.
         * @return false Si no hay conexion o hay un error de conexión.
         */
        bool isConnect();
        
        /**
         * @brief Da el valor de baudrate con el que se configuro el objeto.
         * @return valor entero del baudrate.
         */
        int get_baudrate();
        
        /**
         * @brief Proporciona el nombre del puerto que se utilizado por el objeto
         * @return cadena con el dispositivo.
         */
        std::string get_port();

        /**
         * @brief Obtiene el manejador del puerto serial (para operaciones avanzadas).
         * @return Puntero a dynamixel::PortHandler.
         * @warning Modificar este objeto puede afectar la conexión.
         */
        dynamixel::PortHandler*getPortHandler();
        
        /**
         * @brief Obtiene el manejador de paquetes (para comunicación low-level).
         * @return Puntero a dynamixel::PacketHandler.
         */
        dynamixel::PacketHandler*getPacketHandler();

        // -------------------------->>  Conectividad  <<--------------------------
        /**
         * @brief Establece conexión con el puerto serial configurado en el constructor.
         * @return int 
         *   - `0` (COMM_SUCCESS) si la conexión fue exitosa.
         *   - -1 si falla al abrir el puerto.
         *   - -2 si falla al establecer el baud rate de comunicacion.
         * @throw std::runtime_error Si el puerto no existe o está en uso.
         * @note Realiza un handshake inicial con los motores.
         * @warning No es thread-safe. Si se llama desde múltiples hilos, usar mutex.
         */
        int connect();

        /**
         * @brief Cierra la conexión serial y libera recursos.
         * @warning No se pueden enviar comandos después de llamar a este método.
         */
        void disconnect();

        /**
         * @brief Verifica si un servo Dynamixel específico está conectado y respondiendo.
         * @param idServo ID del servo (0-253 para protocolo 2.0, 1-254 para 1.0).
         * @return true Si el servo responde al ping.
         * @return false Si hay timeout o error de comunicación (consultar dxl_comm_result para mas detalle).
         */
        bool pingServo(int idServo);

        /**
         * @brief Realiza una busqueda de los servos dynamixel
         * @param ids vector vacio para almacenar las IDs encontradas
         * @return true Si al menos un Motor responde
         * @return false Si no hay respuesta o hay un fallo en la comunicacion.
         */
        bool broadcastPing(std::vector<uint8_t>& ids);

        // -------------------------->>  Operaciones de escritura  <<--------------------------
        /**
         * @brief Escribe 1 byte (8 bits) en la memoria del servo.
         * @param idServo ID del servo destino.
         * @param address Dirección de memoria (ej: 64 para "Torque Enable").
         * @param value Valor a escribir (0-255).
         * @return true Si la escritura fue confirmada por el servo.
         * @return false Si hubo error de comunicación o el servo rechazó el valor (consultar dxl_comm_result para mas detalle).
         * @see Control Table del servo (manual Dynamixel).
         */
        bool write1byte(int idServo, int address, int value);

         /**
         * @brief Escribe 2 bytes (16 bits) en la memoria del servo.
         * @param idServo ID del servo destino.
         * @param address Dirección de memoria (ej: 116 para "Goal Position" en AX-12A).
         * @param value Valor a escribir (0-65535).
         * @return bool Ver write1byte().
         */
        bool write2byte(int idServo, int address, int value);
        
        /**
         * @brief Escribe 4 bytes (32 bits) en la memoria del servo.
         * @param idServo ID del servo destino.
         * @param address Dirección de memoria (ej: 104 para "Velocity Limit" en XM430).
         * @param value Valor a escribir (0-4294967295).
         * @return bool Ver write1byte().
         */
        bool write4byte(int idServo, int address, int value);

        /**
         * @brief Escritura multiple a la misma direccion
         * @param ids IDs de los motores a los que se les esc
         * @param address Direcciones de memoria para cada motor
         * @param data_length longitud de los datos, protocolo 2 unicamente puede ser 1 ,2 o 4.
         * @param values Valores a escribir
         * @return bool Ver write1byte().
         */
        bool writeMultiple(const std::vector<uint8_t>& ids, uint16_t address, uint8_t data_length, std::vector<std::vector<uint8_t>>& values);
        
        /**
         * @brief escrituras a diferentes direcciones a diferentes motores
         * @param ids IDs de los motores a los que se les esc
         * @param addresses Direccion de memoria para cada motor del arreglo ids.
         * @param data_length longitud de los datos, protocolo 2 unicamente puede ser 1 ,2 o 4.
         * @param values Valores a escribir
         * @return bool Ver write1byte().
         */
        bool writeMultiple(const std::vector<uint8_t>& ids, std::vector<uint16_t> addresses, std::vector<uint8_t> data_length, std::vector<std::vector<uint8_t>>& values);

        /**
         * @brief Escrituras multiples a un solo motor
         * @param id ID del motor.
         * @param addresses Direccion de memoria para cada motor del arreglo ids.
         * @param data_length longitud de los datos, protocolo 2 unicamente puede ser 1 ,2 o 4.
         * @param values Valores a escribir
         * @return bool Ver write1byte().
         */
        bool writeMultiple(const uint8_t id, std::vector<uint16_t> addresses, std::vector<uint8_t> data_length, std::vector<std::vector<uint8_t>>& values);

        // -------------------------->>  Operaciones de lectura  <<--------------------------
        /**
         * @brief Lee 1 byte (8 bits) de la memoria del servo.
         * @param idServo ID del servo a consultar.
         * @param address Dirección de memoria (ej: 63 para "Present Load").
         * @return int Valor leído (0-255) o -1 si hubo error.
         * @warning El valor -1 puede indicar error y -2 si no hay conexion.
         */
        int read1byte(int idServo, int address);
        
        /**
         * @brief Lee 2 bytes (16 bits) de la memoria del servo.
         * @param idServo ID del servo a consultar.
         * @param address Dirección de memoria (ej: 126 para "Present Position").
         * @return int Valor leído (0-65535), -2 si no hay conexion y -1 si hubo error.
         */
        int read2byte(int idServo, int address);

        /**
         * @brief Lee 4 bytes (32 bits) de la memoria del servo.
         * @param idServo ID del servo a consultar.
         * @param address Dirección de memoria (ej: 128 para "Present Current" en X-series).
         * @return int Valor leído (rango según especificación), -2 si no hay conexion y -1 si hubo error.
         */
        int read4byte(int idServo, int address);

        /**
         * @brief Lectura multiple a la misma direccion
         * @param ids lista de ids a los cuales se leeran los datos.
         * @param address Dirección de memoria (ej: 116 para "Goal Position" en AX-12A).
         * @param data_length longitud de los datos numero entero en bytes, como es protocolo 2 unicamente puede ser 1 ,2 o 4.
         * @param out_data valores resultante de la lectura de registros de los motores.
         * @return map with the values, the key is the id
         */
        std::map<uint8_t, std::vector<uint8_t>> readMultiple(const std::vector<uint8_t>& ids, uint16_t address, uint8_t data_length);

        /**
         * @brief Lectura multiple a diferentes direcciones
         * @param ids lista de ids a los cuales se leeran los datos.
         * @param addresses Direccion de memoria para cada motor del arreglo ids.
         * @param data_length longitud de los datos numero entero en bytes, como es protocolo 2 unicamente puede ser 1 ,2 o 4.
         * @param out_data valores resultante de la lectura de registros de los motores.
         * @return map with the values, the key is the id
         */
        std::map<uint8_t, std::vector<uint8_t>> readMultiple(const std::vector<uint8_t>& ids, std::vector<uint16_t> address, std::vector<uint8_t> data_length);

        /**
         * @brief Lecturas multiples al mismo motor.
         * @param id id del motor a leer.
         * @param addresses Direccion de memoria para el motor.
         * @param data_length longitud de los datos numero entero en bytes, como es protocolo 2 unicamente puede ser 1 ,2 o 4.
         * @param out_data valores resultante de la lectura de registros de los motores.
         * @return bool Ver write1byte().
         */
        std::map<uint16_t, std::vector<uint8_t>> readMultiple(const uint8_t id, std::vector<uint16_t> address, std::vector<uint8_t> data_length);

    private:

        /// @name Manejadores Dynamixel (SDK interno)
        /// @{
        dynamixel::PortHandler* portHandler;   ///< Low-level: No modificar directamente.
        dynamixel::PacketHandler* packetHandler; ///< Versión según protocolo (1.0/2.0).
        /// @}
    
        bool bConnection;  ///< Estado actual de conexión (true=activa).
    
        uint8_t dxl_error = 0; ///< Bits de error del último comando (ver manual Dynamixel).
        int dxl_comm_result = 0; ///< Resultado de comunicación (0=éxito, <0=error).
        int baudrate = 0;  ///< Baud rate actual (solo modificable antes de connect()).
        std::string port = "";  ///< Establece cual puerto se usara.
};
#endif