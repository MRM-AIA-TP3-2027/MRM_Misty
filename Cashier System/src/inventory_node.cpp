#include "rclcpp/rclcpp.hpp"
#include "cashier_system/msg/bill.hpp"
#include "cashier_system/srv/get_status.hpp"
#include <map>

class InventoryNode : public rclcpp::Node {
public:
    InventoryNode() : Node("inventory_node") {
        sub_ = this->create_subscription<cashier_system::msg::Bill>(
            "bill_topic", 10,
            std::bind(&InventoryNode::callback, this, std::placeholders::_1));

        srv_ = this->create_service<cashier_system::srv::GetStatus>(
            "get_status",
            std::bind(&InventoryNode::status, this,
                      std::placeholders::_1, std::placeholders::_2));
    }

private:
    void callback(const cashier_system::msg::Bill::SharedPtr msg) {
        RCLCPP_INFO(this->get_logger(), "Received bill!");

        inventory_[msg->item_name] += msg->quantity;
        income_ += msg->quantity * msg->price_per_item;
    }

    void status(
        const std::shared_ptr<cashier_system::srv::GetStatus::Request>,
        std::shared_ptr<cashier_system::srv::GetStatus::Response> res) {

        std::string out = "Inventory:\n";

        for (auto &i : inventory_) {
            out += i.first + ": " + std::to_string(i.second) + "\n";
        }

        out += "Income: " + std::to_string(income_);

        res->status = out;
    }

    std::map<std::string, int> inventory_;
    double income_ = 0;

    rclcpp::Subscription<cashier_system::msg::Bill>::SharedPtr sub_;
    rclcpp::Service<cashier_system::srv::GetStatus>::SharedPtr srv_;
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<InventoryNode>());
    rclcpp::shutdown();
}
