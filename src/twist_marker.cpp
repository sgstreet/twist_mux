// Copyright 2020 PAL Robotics S.L.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of the PAL Robotics S.L. nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include <twist_mux/twist_marker.hpp>

twist_mux::TwistMarkerPublisher::TwistMarkerPublisher()
: TwistMarkerPublisher(rclcpp::NodeOptions())
{
}

twist_mux::TwistMarkerPublisher::TwistMarkerPublisher(const rclcpp::NodeOptions& options)
: Node("twist_marker", options)
{
  std::string frame_id;
  double scale;
  bool use_stamped = true;
  double z;

  this->declare_parameter("frame_id", "base_footprint");
  this->declare_parameter("scale", 1.0);
  this->declare_parameter("use_stamped", true);
  this->declare_parameter("vertical_position", 2.0);

  this->get_parameter<std::string>("frame_id", frame_id);
  this->get_parameter<double>("scale", scale);
  this->get_parameter<bool>("use_stamped", use_stamped);
  this->get_parameter<double>("vertical_position", z);

  marker_ = std::make_shared<TwistMarker>(frame_id, scale, z);

  if (use_stamped)
  {
	sub_stamped_ = this->create_subscription<geometry_msgs::msg::TwistStamped>(
	  "twist", rclcpp::SystemDefaultsQoS(),
	  std::bind(&TwistMarkerPublisher::callback_stamped, this, std::placeholders::_1));
  }
  else
  {
	sub_ = this->create_subscription<geometry_msgs::msg::Twist>(
	  "twist", rclcpp::SystemDefaultsQoS(),
	  std::bind(&TwistMarkerPublisher::callback, this, std::placeholders::_1));
  }

  pub_ =
	this->create_publisher<visualization_msgs::msg::Marker>(
	"marker",
	rclcpp::QoS(rclcpp::KeepLast(1)));
}

#include <rclcpp_components/register_node_macro.hpp>
RCLCPP_COMPONENTS_REGISTER_NODE(twist_mux::TwistMarkerPublisher);
