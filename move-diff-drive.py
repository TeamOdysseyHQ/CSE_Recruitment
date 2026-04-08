#!/usr/bin/env python3

import sys
import time

import rclpy
from geometry_msgs.msg import TwistStamped
from rclpy.node import Node


class DiffDriveMover(Node):
    def __init__(self) -> None:
        super().__init__("diff_drive_mover")
        self.pub = self.create_publisher(TwistStamped, "/diff_drive_controller/cmd_vel", 10)

    def publish_cmd(self, vx: float, wz: float) -> None:
        msg = TwistStamped()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.header.frame_id = "base_link"
        msg.twist.linear.x = float(vx)
        msg.twist.angular.z = float(wz)
        self.pub.publish(msg)

    def run_segment(self, vx: float, wz: float, seconds: float, rate_hz: float = 20.0) -> None:
        dt = 1.0 / rate_hz
        end_time = time.time() + seconds
        while time.time() < end_time:
            self.publish_cmd(vx, wz)
            rclpy.spin_once(self, timeout_sec=0.0)
            time.sleep(dt)


def main() -> int:
    rclpy.init()
    node = DiffDriveMover()

    try:
        # A short, obvious motion pattern:
        # 1) straight, 2) left arc, 3) right arc, 4) stop
        node.get_logger().info("Segment 1: straight")
        node.run_segment(vx=0.8, wz=0.0, seconds=2.5)

        node.get_logger().info("Segment 2: left arc")
        node.run_segment(vx=0.6, wz=0.8, seconds=2.0)

        node.get_logger().info("Segment 3: right arc")
        node.run_segment(vx=0.6, wz=-0.8, seconds=2.0)

        node.get_logger().info("Segment 4: stop")
        node.run_segment(vx=0.0, wz=0.0, seconds=1.0)

    except KeyboardInterrupt:
        node.get_logger().info("Interrupted, publishing stop")
        node.run_segment(vx=0.0, wz=0.0, seconds=0.5)
    finally:
        node.destroy_node()
        rclpy.shutdown()

    return 0


if __name__ == "__main__":
    raise SystemExit(main())

