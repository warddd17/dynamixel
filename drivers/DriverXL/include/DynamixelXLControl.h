#ifndef DYNAMIXEL_CONTROL
#define DYNAMIXEL_CONTROL

#include <string>
#include "DynamixelManagerP2.h"


class DynamixelXLControl{
    
    public:
        // ------------------------- Constructores y Destructor -------------------------
        
        DynamixelXLControl(DynamixelManagerP2* manager, int idMotor);
        ~DynamixelXLControl();
        
        // ------------------------- Getters -------------------------

        std::string get_message();
        int get_id();
        int get_shadowID();
        
        // ------------------------- Configuración, conexion y evaluacion del motor -------------------------

        bool connect(bool home);
        bool desactivate_shadowID();
        bool set_shadowID(int sid);
        bool set_driveMode(int mode);
        bool setWheelMode();
        bool setJointMode(float cwLimit, float ccwLimit);
        bool setPWMMode(float min, float max);
        
        // ------------------------- Control de Movimiento -------------------------

        bool setPosition();
        bool setPositionInTime();
        bool setSpeed();
        bool setPWD();
        bool setHome();

        // ------------------------- Lectura de Estados -------------------------

        float getPosition();
        float getSpeed();
        bool isMoving();

    private:
        // ------------------------- Funciones de conversion -------------------------

        int convertAngleTOint(float angle, bool degrees);
        float convertIntTOangle(int value);
        int convertRADStoInt(float radps);
        float convertINTtoRADS(int value);

        // ------------------------- Validacion y limites -------------------------

        int clamp(int value, int minLimit, int maxLimit);
        bool setCWAngleLimit(int nlimit);
        bool setCCWAngleLimit(int nlimit);
        
        // ------------------------- Variables privadas -------------------------

        DynamixelManagerP2* control;
        std::string sMessage;
        uint8_t idMotor;
        bool bInvertido = false;
        uint8_t shadow_Id = 0;
        short mode = 0;
        int nCwlimit = 0;
        int nCcwlimit = 0;
};
#endif