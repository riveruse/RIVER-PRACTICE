import rclpy
from rclpy.node import Node
from status_interfaces.msg import SystemStatus
import psutil
import platform

class SysStatusPub(Node):
    def __init__(self,node_name) :
        super().__init__(node_name)
        self.status_publisher_=self.create_publisher(SystemStatus,'sys_status',10)
        self.timer=self.create_timer(1,self.timer_callback)
    
    def timer_callback(self) :
        cpu_percent=psutil.cpu_percent()
        memory_info=psutil.virtual_memory()
        net_io_counters=psutil.net_io_counters()

        msg=SystemStatus()
        msg.stamp=self.get_clock().now().to_msg()
        msg.host_name =platform.node()               #系统名称
        msg.cpu_percent=cpu_percent             #cpu使用率
        msg.memory_percent=memory_info.percent          #内存使用率
        msg.memory_total=memory_info.total /1024 / 1024            #内存总量
        msg.memory_availble=memory_info.available /1024 / 1024         #剩余有效内存
        msg.net_sent =net_io_counters.bytes_sent /1024 / 1024            #网络发送数据总量
        msg.net_recv= net_io_counters.bytes_recv /1024 / 1024

        self.get_logger().info(f'发布：{str(msg)}')
        self.status_publisher_.publish(msg)



def main() :
    rclpy.init()
    node=SysStatusPub('sys_status_pub')
    rclpy.spin(node)
    rclpy.shutdown()
