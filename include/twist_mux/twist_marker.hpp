/*
 * twist_marker.hpp
 *
 *  Created on: Mar 2, 2025
 *      Author: Stephen Street (stephen@redrocketcomputing.com)
 */

#ifndef _TWIST_MUX_TWIST_MARKER_HPP_
#define _TWIST_MUX_TWIST_MARKER_HPP_

#include <string>

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <geometry_msgs/msg/twist_stamped.hpp>
#include <visualization_msgs/msg/marker.hpp>
#include <visualization_msgs/msg/marker_array.hpp>

namespace twist_mux {

class TwistMarker
{
public:
  TwistMarker(std::string & frame_id, double scale, double z)
  : frame_id_(frame_id), scale_(scale), z_(z)
  {
	// ID and type:
	marker_.id = 0;
	marker_.type = visualization_msgs::msg::Marker::ARROW;

	// Frame ID:
	marker_.header.frame_id = frame_id_;

	// Pre-allocate points for setting the arrow with the twist:
	marker_.points.resize(2);

	// Vertical position:
	marker_.pose.position.z = z_;

	// Scale:
	marker_.scale.x = 0.05 * scale_;
	marker_.scale.y = 2 * marker_.scale.x;

	// Color:
	marker_.color.a = 1.0;
	marker_.color.r = 0.0;
	marker_.color.g = 1.0;
	marker_.color.b = 0.0;

	// Error when all points are zero:
	marker_.points[1].z = 0.01;
  }

  void update(const geometry_msgs::msg::Twist & twist)
  {
	using std::abs;

	marker_.points[1].x = twist.linear.x;

	if (abs(twist.linear.y) > abs(twist.angular.z)) {
	  marker_.points[1].y = twist.linear.y;
	} else {
	  marker_.points[1].y = twist.angular.z;
	}
  }

  const visualization_msgs::msg::Marker & getMarker()
  {
	return marker_;
  }

private:
  visualization_msgs::msg::Marker marker_;

  std::string frame_id_;
  double scale_;
  double z_;
};

class TwistMarkerPublisher : public rclcpp::Node
{
public:
  TwistMarkerPublisher();
  explicit TwistMarkerPublisher(const rclcpp::NodeOptions& options);

  void callback(const geometry_msgs::msg::Twist::ConstSharedPtr twist)
  {
	marker_->update(*twist);

	pub_->publish(marker_->getMarker());
  }

  void callback_stamped(const geometry_msgs::msg::TwistStamped::ConstSharedPtr twist)
  {
	marker_->update(twist->twist);

	pub_->publish(marker_->getMarker());
  }

private:
  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_;
  rclcpp::Subscription<geometry_msgs::msg::TwistStamped>::SharedPtr sub_stamped_;
  rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr pub_;

  std::shared_ptr<TwistMarker> marker_ = nullptr;
};

}

#endif /* _TWIST_MUX_TWIST_MARKER_HPP_ */
