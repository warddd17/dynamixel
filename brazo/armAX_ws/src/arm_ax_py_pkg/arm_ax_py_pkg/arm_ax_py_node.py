#!/usr/bin/env python3
import sys

sys.path.append('/home/robousr/ROS2Dev/dynamixel-ctrl/brazo/armAX_ws/src/arm_ax_py_pkg/arm_ax_py_pkg/dynamixel_libs')

import rclpy
from rclpy.node import Node
import armAX
import DynamixelManager
import time
from rclpy.qos import QoSProfile, ReliabilityPolicy,  HistoryPolicy
from arm_ax_interfaces.msg import SetOnePosition

class OneMotorCtrl(Node):
    def __init__(self, node_name):
        super().__init__(node_name)
        self.__baudrate = 1000000
        self.__port = "/dev/ttyUSB0"
        self.__protocol = 1.0
        self.__manager = DynamixelManager.DynamixelManager(self.__port, self.__baudrate, self.__protocol)

        if self.__manager.connect() == -1 :
            self.get_logger().error(f" Port '{self.__port}' not available or not exists")
        elif self.__manager.connect() == -2 :
            self.get_logger().error(f" Baudrate '{self.__baudrate}' not valid")
        
        self.__brazo = armAX.armAX(self.__manager)
        if not self.__brazo.connect(True):
            self.get_logger().error(self.__brazo.get_messages())
        
        self.get_logger().info(self.__brazo.get_messages())
        self.__qos = QoSProfile(    
            reliability=ReliabilityPolicy.BEST_EFFORT,
            history=HistoryPolicy.KEEP_LAST,
            depth=5
        )
        self._oneMotorPos_sub = self.create_subscription(
            SetOnePosition,
            '/OneMotor_cmd',
            self.__on_oneMotor_cmd,
            self.__qos
        )

    def __on_oneMotor_cmd(self, msg:SetOnePosition):
        id = msg.id
        position = msg.position
        time = msg.time
        if not self.__brazo.setOnePosition(id,position,time):
            self.get_logger().error(self.__brazo.get_messages())
        self.get_logger().info(self.__brazo.get_messages())

def init_node(args = None):
    try:
        rclpy.init(args=args)
        oneMotor = OneMotorCtrl('one_motor_ctrl')
        rclpy.spin(oneMotor)
        oneMotor.destroy_node()
        rclpy.shutdown()
    except KeyboardInterrupt:
        print('Program finalized.')
    except Exception as e:
        print(e)

if __name__ == '__main__':
    init_node()
