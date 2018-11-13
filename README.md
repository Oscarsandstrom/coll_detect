# coll_detect
A node to handle data from Lidar, if the Permocar gets too close to an object it sends a stop signal.

## Subscribe
* Topic scan as sensor_mesgs::LaserScan containing lidar scan.
* Topic vw_estimate as std_msgs::Float32MulitArray containing [velocity, angular velocity].

## Publiching
* Topic lidar_stop as std_msgs::Bool true if collision threat is imminent.

## Param
Parameter that can be set.
* k11,kr11 -> default 1.5, timing variables for stop time (kr11/k11^2).
* collision_threshold -> default 0.5, distant to stop before collation in meters (messed from lidar).
* t_delay -> default 0.0, delay to compensate for system delays.
