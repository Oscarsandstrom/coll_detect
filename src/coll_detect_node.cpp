#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <std_msgs/Bool.h>
  // setup for the message, publisher, subscriber and variables
  std_msgs::Bool stop_msg;

  ros::Publisher stop_pub;
  ros::Subscriber scan_sub;

  bool stop;
  float collision_threshold = 0.4;

  void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
  stop = false;
  //for loop to check every value in the ranges array from the LaserScan msg from the lidar node
  for(int i = 0; i < scan->ranges.size(); i = i + 1)
  {
  
    //if function to test the values relative to our collision threshold. As soon a value is found that is lower than 0.4m the loop breaks and the publish function is started
    if (scan->ranges[i] < collision_threshold)
      {
        stop = true;
	break;
      }
  }
  stop_msg.data = stop;
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
