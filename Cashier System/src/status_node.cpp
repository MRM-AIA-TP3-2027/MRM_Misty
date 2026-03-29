#include "rclcpp/rclcpp.hpp"
#include "cashier_system/srv/get_status.hpp"

class StatusNode : public rclcpp::Node {
public:
    StatusNode() : Node("status_node") {
        client_ = this->create_client<cashier_system::srv::GetStatus>("get_status");

        timer_ = this->create_wall_timer(
            std::chrono::seconds(3),
            std::bind(&StatusNode::run, this));
    }

private:
    void run() {
        if (!client_->wait_for_service(std::chrono::seconds(1))) {
            RCLCPP_WARN(this->get_logger(), "Waiting for service...");
            return;
        }

        auto request = std::make_shared<cashier_system::srv::GetStatus::Request>();
        auto future = client_->async_send_request(request);

        if (future.wait_for(std::chrono::seconds(1)) == std::future_status::ready) {
            auto response = future.get();
            RCLCPP_INFO(this->get_logger(), "\n%s", response->status.c_str());
        }
    }

    rclcpp::Client<cashier_system::srv::GetStatus>::SharedPtr client_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<StatusNode>());
    rclcpp::shutdown();
}
