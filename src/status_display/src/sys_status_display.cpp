#include <QApplication>
#include <QLabel>
#include <QString>
#include"rclcpp/rclcpp.hpp"
#include"status_interfaces/msg/system_status.hpp"

using SystemStatus = status_interfaces::msg::SystemStatus;
class SysStatusDisplay : public rclcpp::Node
{
public:
    SysStatusDisplay() : Node("sys_status_display") {
        subscription_ = this->create_subscription<SystemStatus>(
            "sys_status", 10, [&](const SystemStatus::SharedPtr msg) -> void
            { label_->setText(get_gstr_from_msg(msg)); });
        // 创建一个空的SystemStatus 对象，转化成QString 进行显示
        label_ = new QLabel(get_gstr_from_msg(std ::make_shared<SystemStatus>()));
        label_->show();
    }
QString get_gstr_from_msg (const SystemStatus::SharedPtr msg) {
//TODO：将msg中的内容提取出来并组装成字符串
std::stringstream show_str;
show_str
                                               <<"==================系统可视化显示工具====================\n"
                                               <<"数 据 时 间：\t" << msg->stamp.sec << "\ts\n"
                                               <<"用  户  名：\t" << msg->host_name << "\t\n"
                                               <<"CPU 使用率" << msg->cpu_percent << "\t%\n"
                                               <<"内存使用率：\t " << msg->memory_percent << "\t%\n "
                                               <<"内存总大小：\t " << msg->memory_total << "\tMB\n"
                                               <<"剩余有效内存：\t " << msg->memory_availble <<"\tMB\n "
                                               <<"网络发送量：\t " << msg->net_sent << "\tMB\n "
                                               <<"网络接收量：\t " << msg->net_recv << "\tMB\n "
                                               <<"====================================================";
return QString::fromStdString(show_str.str());

}
private:
    rclcpp::Subscription<SystemStatus>::SharedPtr subscription_;
    QLabel *label_;
    };

int main(int argc,char* argv[])
{
    rclcpp::init(argc,argv);
    QApplication app(argc, argv);
    auto node = std::make_shared<SysStatusDisplay>();
    std::thread spin_thread([&]() ->void{ rclcpp::spin(node); });
    spin_thread.detach();
    app.exec();
    rclcpp::shutdown();
    return 0;
}