#include <memory>
#include <cmath>
#include <iostream>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

class GlobalPlanner : public rclcpp::Node
{
public:
    GlobalPlanner() : Node("global_planner")
    {
        cmd_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);

        // GPS input
        std::cout << "Enter start latitude: ";
        std::cin >> start_lat_;

        std::cout << "Enter start longitude: ";
        std::cin >> start_lon_;

        std::cout << "Enter goal latitude: ";
        std::cin >> goal_lat_;

        std::cout << "Enter goal longitude: ";
        std::cin >> goal_lon_;

        RCLCPP_INFO(this->get_logger(),
                    "Start: (%f, %f) | Goal: (%f, %f)",
                    start_lat_, start_lon_, goal_lat_, goal_lon_);

        convertGPSToXY();

        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(100),
            std::bind(&GlobalPlanner::moveRobot, this));
    }

private:
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub_;
    rclcpp::TimerBase::SharedPtr timer_;

    double start_lat_, start_lon_;
    double goal_lat_, goal_lon_;

    double current_x_ = 0.0, current_y_ = 0.0;
    double current_theta_ = 0.0;

    double goal_x_, goal_y_;

    void convertGPSToXY()
    {
        double scale = 111139.0;

        goal_x_ = (goal_lat_ - start_lat_) * scale;
        goal_y_ = (goal_lon_ - start_lon_) * scale;

        RCLCPP_INFO(this->get_logger(),
                    "Goal X: %f, Goal Y: %f",
                    goal_x_, goal_y_);
    }

    void moveRobot()
    {
        geometry_msgs::msg::Twist cmd;

        double dx = goal_x_ - current_x_;
        double dy = goal_y_ - current_y_;
        double distance = sqrt(dx * dx + dy * dy);

        double angle_to_goal = atan2(dy, dx);
        double angle_error = angle_to_goal - current_theta_;

        // normalize angle
        while (angle_error > M_PI) angle_error -= 2 * M_PI;
        while (angle_error < -M_PI) angle_error += 2 * M_PI;

        if (distance > 0.3)
        {
            // MOVE + TURN together (curved path)
            cmd.linear.x = 0.6;
            cmd.angular.z = 0.5 + 1.5 * angle_error;

            // clamp angular velocity
            if (cmd.angular.z > 1.0) cmd.angular.z = 1.0;
            if (cmd.angular.z < -1.0) cmd.angular.z = -1.0;
        }
        else
        {
            cmd.linear.x = 0.0;
            cmd.angular.z = 0.0;
            RCLCPP_INFO(this->get_logger(), "Goal Reached!");
        }

        // Debug (optional but useful)
        RCLCPP_INFO(this->get_logger(),
            "Dist: %.2f | Angle_err: %.2f | Lin: %.2f | Ang: %.2f",
            distance, angle_error, cmd.linear.x, cmd.angular.z);
        
        cmd_pub_->publish(cmd);

        // simulate motion
        double dt = 0.1;
        current_x_ += cmd.linear.x * cos(current_theta_) * dt;
        current_y_ += cmd.linear.x * sin(current_theta_) * dt;
        current_theta_ += cmd.angular.z * dt;
    }
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<GlobalPlanner>());
    rclcpp::shutdown();
    return 0;
}
