#include "rclcpp/rclcpp.hpp"
#include "armAX_cpp_pkg/armAX.h"
#include "arm_ax_interfaces/msg/set_one_position.hpp"

class MotorController : public rclcpp::Node {
public:
    MotorController() : Node("motor_controller"),
        manager_("/dev/ttyUSB0", 1000000, 1.0),
        brazo_(&manager_)  // Inicialización en lista
    {
        // 1. Conexión Dynamixel
        const int connect_result = manager_.connect();
        if(connect_result == -1) {
            RCLCPP_ERROR(get_logger(), "Port not available or does not exist");
            return;
        } else if(connect_result == -2) {
            RCLCPP_ERROR(get_logger(), "Baudrate not valid");
            return;
        }

        // 2. Configuración brazo
        if(!brazo_.connect(true)) {
            RCLCPP_ERROR(get_logger(), "Error detecting motors: %s", brazo_.get_messages().c_str());
            return;
        }
        RCLCPP_INFO(get_logger(), "%s", brazo_.get_messages().c_str());

        // 3. Configuración QoS
        auto qos = rclcpp::QoS(
            rclcpp::KeepLast(10),
            rmw_qos_profile_sensor_data
        );

        // 4. Crear suscriptor con callback moderno
        subscription_ = create_subscription<arm_ax_interfaces::msg::SetOnePosition>(
            "/OneMotor_cmd",
            qos,
            [this](const arm_ax_interfaces::msg::SetOnePosition::SharedPtr msg) {
                this->command_callback(msg);
            }
        );
    }

private:
    void command_callback(const arm_ax_interfaces::msg::SetOnePosition::SharedPtr msg) {
        if(!brazo_.setOnePosition(msg->id, msg->position, msg->time)) {
            RCLCPP_ERROR(get_logger(), "Command failed: %s", brazo_.get_messages().c_str());
        }
    }

    // Miembros como variables de clase
    DynamixelManager manager_;
    armAX brazo_;
    rclcpp::Subscription<arm_ax_interfaces::msg::SetOnePosition>::SharedPtr subscription_;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<MotorController>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}