#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float32MultiArray.h>

  // setup for the message, publisher, subscriber and variables
  std_msgs::Bool stop_msg;

  ros::Publisher stop_pub;
  ros::Subscriber scan_sub;
  ros::Subscriber velosety_sub;

  bool stop;
  float collision_threshold;
  int scan_start = 180;
  int scan_stop = 361;
  
  float v = 0;
  float k11;
  float kr11;
  float t_delay;

  void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
  stop = false;
  //for loop to check every value in the ranges array from the LaserScan msg from the lidar node
  for(int i = scan_start; i < scan_stop; i = i + 1)
  {
  
    //if function to test the values relative to our collision threshold. As soon a value is found that is lower than 0.4m the loop breaks and the publish function is started
    if (0 < scan->ranges[i] && scan->ranges[i] < (collision_threshold + v * kr11 /pow(k11,2) + v * t_delay))
      {
        stop = true;
	break;
      }
  }
  stop_msg.data = stop;
  stop_pub.publish(stop_msg);

  

}

void vwCallback(const std_msgs::Float32MultiArray::ConstPtr& array)
{
  v = array->data[0];
}


int main(int argc, char** argv)
{
  
  ros::init(argc, argv, "coll_detect_node");
  
  ros::NodeHandle nh_;
  ros::NodeHandle n_("~");

  n_.param<float>("k11",k11,2.5);
  n_.param<float>("collision_threshold", collision_threshold, 0.4);
  n_.param<float>("kr11",kr11,2.5); 
  n_.param<float>("t_delay",t_delay,0.1);

  ROS_INFO("k11: %f, collision_threshold: %f, kr11: %f, t_delay: %f", kr11, collision_threshold, kr11, t_delay);

  stop_pub = nh_.advertise<std_msgs::Bool>("lidar_stop", 1);
  scan_sub = nh_.subscribe<sensor_msgs::LaserScan>("scan", 1, scanCallback);
  velosety_sub = nh_.subscribe<std_msgs::Float32MultiArray>("vw_estimate", 1, vwCallback);

  ros::spin();
}
