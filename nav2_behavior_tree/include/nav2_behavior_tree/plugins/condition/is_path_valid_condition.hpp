// Copyright (c) 2021 Joshua Wallace
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef NAV2_BEHAVIOR_TREE__PLUGINS__CONDITION__IS_PATH_VALID_CONDITION_HPP_
#define NAV2_BEHAVIOR_TREE__PLUGINS__CONDITION__IS_PATH_VALID_CONDITION_HPP_

#include <string>
#include <memory>

#include "nav2_ros_common/lifecycle_node.hpp"
#include "behaviortree_cpp/condition_node.h"
#include "behaviortree_cpp/json_export.h"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "nav2_msgs/srv/is_path_valid.hpp"
#include "nav2_ros_common/service_client.hpp"
#include "nav2_behavior_tree/bt_utils.hpp"
#include "nav2_behavior_tree/json_utils.hpp"


namespace nav2_behavior_tree
{

/**
 * @brief A BT::ConditionNode that returns SUCCESS when the IsPathValid
 * service returns true and FAILURE otherwise
 */
class IsPathValidCondition : public BT::ConditionNode
{
public:
  /**
   * @brief A constructor for nav2_behavior_tree::IsPathValidCondition
   * @param condition_name Name for the XML tag for this node
   * @param conf BT node configuration
   */
  IsPathValidCondition(
    const std::string & condition_name,
    const BT::NodeConfiguration & conf);

  IsPathValidCondition() = delete;

  /**
   * @brief The main override required by a BT action
   * @return BT::NodeStatus Status of tick execution
   */
  BT::NodeStatus tick() override;

  /**
   * @brief Function to read parameters and initialize class variables
   */
  void initialize();

  /**
   * @brief Creates list of BT ports
   * @return BT::PortsList Containing node-specific ports
   */
  static BT::PortsList providedPorts()
  {
    // Register JSON definitions for the types used in the ports
    BT::RegisterJsonDefinition<nav_msgs::msg::Path>();
    BT::RegisterJsonDefinition<std::chrono::milliseconds>();

    return {
      BT::InputPort<nav_msgs::msg::Path>("path", "Path to Check"),
      BT::InputPort<std::chrono::milliseconds>("server_timeout"),
      BT::InputPort<unsigned int>("max_cost", 253, "Maximum cost of the path"),
      BT::InputPort<bool>(
          "consider_unknown_as_obstacle", false,
          "Whether to consider unknown cost as obstacle")
    };
  }

private:
  nav2::LifecycleNode::SharedPtr node_;
  nav2::ServiceClient<nav2_msgs::srv::IsPathValid>::SharedPtr client_;
  // The timeout value while waiting for a response from the
  // is path valid service
  std::chrono::milliseconds server_timeout_;
  unsigned int max_cost_;
  bool consider_unknown_as_obstacle_;
};

}  // namespace nav2_behavior_tree

#endif  // NAV2_BEHAVIOR_TREE__PLUGINS__CONDITION__IS_PATH_VALID_CONDITION_HPP_
