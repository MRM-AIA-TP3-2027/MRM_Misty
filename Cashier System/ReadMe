# Cashier System using ROS 2

## Description

This project implements a distributed cashier and inventory management system using ROS 2. It simulates a basic billing workflow where transactions are generated, processed, and reflected across multiple nodes in real time.

## Approach

The system is designed using three ROS 2 nodes:

* **Bill Generator Node**: Accepts user input (item name, quantity, and price) and publishes it as a message.
* **Inventory Node**: Subscribes to the bill topic, updates inventory and total income, and provides a service to retrieve the current system status.
* **Status Node**: Periodically requests system status via a service call and displays the inventory and income.

This modular design demonstrates communication between nodes using topics and services.

## ROS Topics

* `/bill_topic`
  Used to publish and subscribe to bill data between the bill generator and inventory node.

## Messages

### Bill.msg

```
string item_name
int32 quantity
float32 price_per_item
```

## Services

### GetStatus.srv

```
---
string status
```

## How to Run

### Build the workspace

```
cd ~/ros2_ws
colcon build
source install/setup.bash
```

### Run nodes (in separate terminals)

Terminal 1:

```
ros2 run cashier_system inventory_node
```

Terminal 2:

```
ros2 run cashier_system status_node
```

Terminal 3:

```
ros2 run cashier_system bill_generator
```

## Example Input

```
Item: apple
Quantity: 2
Price: 20
```

## Expected Output

```
Inventory:
apple: 2
Income: 40
```

## RQT Graph

To visualize node communication:

```
rqt_graph
```

## Project Structure

```
cashier_system/
├── src/
├── msg/
├── srv/
├── launch/
├── CMakeLists.txt
├── package.xml
└── README.md
```

## Future Improvements

* Add a graphical user interface
* Integrate persistent storage for inventory
* Extend system for multiple users or terminals
