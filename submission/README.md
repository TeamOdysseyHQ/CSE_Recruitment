# CSE Task Documentation

> [!NOTE]
> Due to back to back quiz, assignments, lab evals and academic projects over the past few weeks, I was unable to finish **all** the tasks with the given time without excessive and unfair use of LLMs. This pull request contains category 1 task 1 and the core task of category 2. 

## Category 1 Task 1

There was ZERO use of any LLM tools for this task. Everything I learnt was with the old school way using documentations, example code from repositories and forums.

### GPS Parsing
#### Message Type
* I did some searching and found out about ros2's [common interfaces](https://github.com/ros2/common_interfaces), where I compared different message types for GPS data.
* I looked into geographic_msgs/msg/GeoPoint, geometry_msgs/msg/Point and sensor_msgs/msg/NavSatFix. But I had to reject the first two because GeoPoint does not have a covariance matrix, and apparently a rover is useless without knowing the accuracy. Point was an obvious rejection because it did not output lat/long, but the relative X,Y,Z coordinates.
* This led me to choose NavSatFix, which also was my initial choice before research, it appeared to be used most commonly. It also included lat, long, alt and the covariance matrix.
* The code was pretty straightforward, I just had to refer to TinyGPU [example files](https://github.com/mikalhart/TinyGPSPlus/blob/master/examples/DeviceExample/DeviceExample.ino)

### Parsing
* I used TinyGPSPlus to parse the NMEA coordinates. First I had Serial.print() statements for latitude, longitude altitude and timestamp, but later during MicroROS integration, I removed it because I read that `set_microros_transports()` takes over of the serial port, and I cannot send plain text in the binary stream.

### MicroROS Integration
* For this as well, I mainly refered to [example files of MicroROS](https://github.com/mirs240x/micro_ros_arduino_mirs240x/blob/humble/examples/micro-ros_publisher_wifi/micro-ros_publisher_wifi.ino}) for boilerplate setup
* One of the main changes I did was with the publisher type. I chose `rclc_publisher_init_best_effort` rather than `init_default` as in the example files because it made more sense. The teensy shouldn't waste clock cycles to retry failed coordinates, and just send the next coordinate over instead.

## Category 2 Core Task

* I started by figuring out how ROS and Gazebo works. I had to step into the rabbit hole for a good while
* Once I had a decent idea, I ran `colcon build --symlink-install` which obviously did not run.
* I manually authored CMakeLists.txt and package.xml based on [this page from ROS2 Docs](https://docs.ros.org/en/humble/Tutorials/Beginner-Client-Libraries/Creating-Your-First-ROS2-Package.html)
* For the launch file I refered to [this doc](https://docs.ros.org/en/humble/Tutorials/Intermediate/URDF/Using-URDF-with-Robot-State-Publisher-cpp.html), but that wasn't really sufficient so I had to take some help from Gemini and look into other forums.
* There was a random symlink that I had to break my head over for a sad amount of time. I might just be slow. Maybe it wouldn't have caused such an issue if I had named my user "muthibazz".
* When I now tried building I got a `package 'mini_model_control' not found` error. Upon investigation, I found that the URDF was hardcoded `$(find mini_model_control)/config/rover_controllers.yaml` which wasn't in the repo. I then created this missing package with `ros2 pkg create mini_model_control`.
* Now to config and run the differential drive, I looked for joint names (I needed the exact names), but I found a bunch of rather.. subpar naming conventions. I instead looked for joints with `type="continuous"` and found that there were 6 wheels.
* Three wheels had a Positive Z offset (Revolute 29, 30, 31), Three wheels had a Negative Z offset (Revolute 36, 37, 38)
* Finally, when launching I got a parameter parsing error. Throwing this into Gemini, it told me to use a str type casting because the description was being passed as XML.
* This worked, and I got Skanda spawned on my Gazebo!!