/**
 * @file DynamixelManager.h
 * @brief Control de motores dynamixel AX12-A.
 * @author Jose Manuel Plascencia Ramos
 * @date 2025-04-01
 * @version 1.0
 * @code 
  int main() {
    const int baudrate = 1000000;
    const std::string device = "/dev/ttyUSB0";
    const float protocol = 1.0;
    DynamixelManager puerto(device,baudrate,protocol);
    puerto.connect();

    DynamixelAXControl motor(&puerto, 1);

    motor.connect();
    std::cout<<motor.get_message();
    usleep(1000000);
    motor.setPosition(1);
    std::cout<<motor.get_message();
    usleep(2000000);

    //fin
    puerto.disconnect();
    return 0;
    }
 * @endcode 
 */

#ifndef DYNAMIXEL_CONTROL
#define DYNAMIXEL_CONTROL

#include <string>
#include "DynamixelManagerP1.h"


/**
 * @class DynamixelAXControl
 * @brief Controlador especializado para motores Dynamixel de la serie AX (AX-12A, AX-18F, etc.).
 * @details Proporciona una interfaz de alto nivel para manejar motores AX usando un DynamixelManager existente.
 * @warning Esta clase no gestiona la conexión serial (debe inyectarse un DynamixelManager válido).
 */
class DynamixelAXControl{
    
    public:
        // ------------------------- Constructores y Destructor -------------------------
        /**
         * @brief Constructor que asocia el controlador a un motor específico.
         * @param manager Puntero a un DynamixelManager ya inicializado (gestión de conexión).
         * @param idMotor ID del motor a controlar (1-253 para protocolo 1.0).
         * @note En caso de usar una ID negativa los angulos se invierten.
         * @throw std::invalid_argument Si el ID está fuera de rango o manager es nullptr.
         * @example
         *   DynamixelManager manager("/dev/ttyUSB0", 57600, 1.0);
         *   DynamixelAXControl ax_controller(&manager, 1); // Controla motor con ID=1
         */
        DynamixelAXControl(DynamixelManagerP1* manager, int idMotor);

        /**
         * @brief Destructor: Libera recursos (no cierra la conexión del manager).
         * @note El DynamixelManager debe vivir más que este objeto.
         */
        ~DynamixelAXControl();
        
        // ------------------------- Getters -------------------------
        /**
         * @brief Obtiene el último mensaje de estado (útil para depuración).
         * @return std::string Mensaje descriptivo del último error o acción ejecutada.
         * @example
         *   std::cout << ax_controller.get_message(); // Ej: "[ID: 1] set speed to 0.278974."
         */
        std::string get_message();

        /**
         * @brief Obtiene el ID del motor asociado.
         * @return int ID configurado en el constructor (1-253).
         */
        int get_id();
        
        // ------------------------- Configuración, conexion y evaluacion del motor -------------------------

        /**
         * @brief Establece conexión con el motor y opcionalmente lo mueve a posición "home".
         * @param home Si es true, ejecuta un movimiento a home después de conectar.
         * @return true Si la conexión y homing (si aplica) fueron exitosos.
         * @return false Si falla en alguna parte de la conexion.
         * @throw std::runtime_error Si el motor no responde.
         * @note Al terminar la operacion se asignan mensajes para los limites de movimiento y velocidad actual.
         * @note consultar el valor de dxl_comm_result del manager y get_message() para consultar el mensaje de error asignado.
         */
        bool connect(bool home = false);

        /**
         * @brief Configura el motor en modo "rueda" (rotación continua).
         * @return true Si el cambio de modo fue exitoso.
         * @warning Sobrescribe los límites de posición. Usar setJointMode() para volver a modo articulación.
         */
        bool setWheelMode();

        /**
         * @brief Configura el motor en modo "articulación" (posición angular limitada).
         * @param cwLimit Límite horario (en radianes o grados, segun el valor de 'degrees').
         * @param ccwLimit Límite antihorario (en radianes o grados, segun el valor de 'degrees').
         * @param degrees Si true, los límites se interpretan en grados (150- -150). Si false, en Radianes.
         * @return true Si la configuración fue exitosa.
         * @note Los valores típicos en grados para la serie AX es de 150 a -150 en grados.
         */
        bool setJointMode(float cwLimit, float ccwLimit, bool degrees = false);
        
        // ------------------------- Control de Movimiento -------------------------

        /**
         * @brief Mueve el motor a una posición específica.
         * @param position Posición objetivo (en radianes o grados, segun el valor de 'degrees').
         * @param degrees Si true, los límites se interpretan en grados (150- -150). Si false, en Radianes.
         * @return true Si el comando se envió correctamente (no bloqueante; ver getMoving()).
         * @see getPosition(), setJointMode()
         */
        bool setPosition(float position, bool degrees = false);

        /**
         * @brief Mueve el motor a una posición específica.
         * @param position Posición objetivo (en radianes o grados, segun el valor de 'degrees').
         * @param time Tiempo en el que se alcanzara el objetivo (en segundos).
         * @param degrees Si true, los límites se interpretan en grados (150- -150). Si false, en Radianes.
         * @return true Si el comando se envió correctamente (no bloqueante; ver getMoving()).
         * @see getPosition(), setJointMode()
         */
        bool setPositionInTime(float position, float time, bool degrees = false);

        /**
         * @brief Establece la velocidad de movimiento (afecta a setPosition()).
         * @param speed Velocidad (rango depende del modo):
         *              - Modo articulación: 0 - 11.89127016 (0-1023 en valor del servo)
         *              - Modo rueda: 11.89127016 a -11.89127016 (0-11.89127016 CW, 0 a -11.89127016 CCW).
         * @return true Si el parámetro fue aceptado.
         * @note Los valores se encuentran en la unidad de rads/s
         */
        bool setSpeed(float speed);

        /**
         * @brief Coloca al motor en la posicion home 0 (512 en valor del motor)
         * @return true Si se completo el movimiento exitosamente.
         * @return false Sí falló al llevar el movimiento.
         */
        bool setHome();

        // ------------------------- Lectura de Estados -------------------------

        /**
         * @brief Obtiene la posición actual del motor.
         * @return devuelve el valor de la posicion acutal del servo en radianes
         */
        float getPosition();

        /**
         * @brief Obtiene la velocidad actual del motor.
         * @return float Velocidad (interpretación igual que setSpeed()).
         */
        float getSpeed();

        /**
         * @brief Lee el voltaje de entrada del motor.
         * @return Devuelve el voltaje según los valores especificados en la memoria del motor
         * @warning No se han implementado funciones de conversion a este metodo.
         * @note Consultar el manual del motor 
         */
        int getVoltaje();

        /**
         * @brief Lee la temperatura interna del motor.
         * @return Devuelve la temperatura según los valores especificados en la memoria del motor
         * @warning No se han implementado funciones de conversion a este metodo.
         * @note Consultar el manual del motor
         */
        int getTemperature();

        /**
         * @brief Verifica si el motor está en movimiento.
         * @return true Si el motor está ejecutando un movimiento (setPosition() pendiente).
         * @return false Sí el motor no está ejecutando un movimiento (setPosition() completado).
         */
        bool isMoving();

    private:
        // ------------------------- Funciones de conversion -------------------------

        /**
         * @brief Convierte un ángulo en grados o radianes a valor entero Dynamixel (0-1023).
         * @param angle Ángulo en grados (degrees=true) o radianes (degrees=false).
         * @param degrees Si es true, el ángulo se interpreta en grados (-150 - 150). Si es false, en radianes.
         * @return int Valor en unidades Dynamixel (0-1023).
         * @note El rango de salida corresponde al modelo AX-12A y AX-18A (ajustar para otros modelos).
         */
        int convertAngleTOint(float angle, bool degrees);

        /**
         * @brief Convierte un valor entero Dynamixel (0-1023) a ángulo en radianes.
         * @param value Valor leído del motor (ej: de getPosition() en modo raw).
         * @return Valor equivalente de la posicion del motor en radianes.
         */
        float convertIntTOangle(int value);

        /**
         * @brief Convierte radianes por segundo (rad/s) a valor de velocidad Dynamixel.
         * @param radps Velocidad angular en radianes/segundo.
         * @return int Valor en unidades Dynamixel (0-1023 para modo joint, 0-2047 para modo wheel).
         * @see setSpeed()
         */
        int convertRADStoInt(float radps);

        /**
         * @brief Convierte un valor de velocidad Dynamixel a radianes por segundo (rad/s).
         * @param value Valor leído del motor (ej: de getSpeed()).
         * @return float Velocidad en rad/s.
         */
        float convertINTtoRADS(int value);

        // ------------------------- Validacion y limites -------------------------
        /**
         * @brief Limita un valor a un rango [minLimit, maxLimit].
         * @param value Valor a validar.
         * @param minLimit Límite inferior (incluido).
         * @param maxLimit Límite superior (incluido).
         * @return int Valor dentro del rango.
         * @warning Usado internamente para proteger al motor de valores inválidos.
         */
        int clamp(int value, int minLimit, int maxLimit);

        /**
         * @brief Calcula el tiempo de movimiento para un motor individual para establecer la velocidad del motor (lógica interna).
         * @param goal_angle Ángulo objetivo (en radianes).
         * @param time Tiempo total deseado (en segundos).
         * @return true Si el cálculo fue válido y se establecio la velocidad del motor.
         */
        bool calculateSpeed(float goal_angle, float time);

        /**
         * @brief Configura el límite de posición en sentido horario (CW) del motor.
         * @param nlimit Valor en unidades Dynamixel (0-1023).
         * @return true Si el límite fue aceptado por el motor.
         * @note Sobrescribe la EEPROM del motor (usar con moderación).
         * @see setJointMode()
         */
        bool setCWAngleLimit(int nlimit);

        /**
         * @brief Configura el límite de posición en sentido antihorario (CCW) del motor.
         * @param nlimit Valor en unidades Dynamixel (0-1023).
         * @return bool Ver setCWAngleLimit().
         */
        bool setCCWAngleLimit(int nlimit);
        
        // ------------------------- Variables privadas -------------------------

        /**
         * @brief Puntero al gestor de comunicación Dynamixel (inyectado en el constructor).
         * @warning No debe ser nullptr. La vida útil de este objeto debe superar a la de DynamixelAXControl.
         * @see DynamixelManager
         */
        DynamixelManagerP1* control;

        /**
         * @brief Mensaje de estado o error más reciente (para depuración).
         * @details Formato: "[ID: 1] Descripción del estado" (ej: "[ID: 1] set in position: 0.577799.").
         */
        std::string sMessage;

        /**
         * @brief ID del motor Dynamixel asociado (1-253 para protocolo 1.0).
         * @note Compatible con modelos de la serie AX (AX-12A, AX-18F, etc.).
         */
        int idMotor;

        /**
         * @brief Flag que indica si el motor está en modo "rueda" (rotación continua).
         * @see setWheelMode(), setJointMode()
         */
        bool bWheelMode;

        /**
         * @brief Flag que invierte la dirección de movimiento (útil para configuraciones espejo).
         * @note Si es true, los comandos de posición/sentido se invertirán.
         */
        bool bInvertido;

        /**
         * @brief Límite de posición en sentido horario (Clockwise) en unidades Dynamixel (0-1023).
         * @details Valor por defecto: 0 (sin límite en modo rueda).
         * @warning Configurado via setJointMode() o setCWAngleLimit().
         */
        int nCwlimit = 0;

        /**
         * @brief Límite de posición en sentido antihorario (Counter-Clockwise) en unidades Dynamixel (0-1023).
         * @details Valor por defecto: 1023 (máximo rango en modo articulación).
         */
        int nCcwlimit = 0;
};
#endif