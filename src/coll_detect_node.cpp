#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <std_msgs/Bool.h>

  std_msgs::Bool stop_msg;

  ros::Publisher stop_pub;
  ros::Subscriber scan_sub;

  bool stop;
  float collision_threshold = 0.4;

  void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{

  for(int i = 0; i < scan->ranges.size(); i = i + 1)
  {
  stop = false;
    if (scan->ranges[i] < collision_threshold)//TODO look up values from lidar
      {
        stop = true;
        stop_msg.data = stop;
	break;
      }
  }
  stop_pub.publish(stop_msg);

  

}



int main(int argc, char** argv)
{
  
  ros::init(argc, argv, "coll_detect_node");
  
  ros::NodeHandle nh_;
  
  stop_pub = nh_.advertise<std_msgs::Bool>("lidar/stop", 1);
  scan_sub = nh_.subscribe<sensor_msgs::LaserScan>("scan", 1, scanCallback);

  ros::spin();
}
