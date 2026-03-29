#include "rclcpp/rclcpp.hpp"
#include "cashier_system/msg/bill.hpp"
#include <iostream>

class BillGenerator : public rclcpp::Node {
public:
    BillGenerator() : Node("bill_generator") {
        pub_ = this->create_publisher<cashier_system::msg::Bill>("bill_topic", 10);

        timer_ = this->create_wall_timer(
            std::chrono::seconds(3),
            std::bind(&BillGenerator::run, this));
    }

private:
    void run() {
        auto msg = cashier_system::msg::Bill();

        std::cout << "\nItem: ";
        std::cin >> msg.item_name;

        std::cout << "Quantity: ";
        std::cin >> msg.quantity;

        std::cout << "Price: ";
        std::cin >> msg.price_per_item;

        pub_->publish(msg);

        RCLCPP_INFO(this->get_logger(), "Bill sent!");
    }

    rclcpp::Publisher<cashier_system::msg::Bill>::SharedPtr pub_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<BillGenerator>());
    rclcpp::shutdown();
}
