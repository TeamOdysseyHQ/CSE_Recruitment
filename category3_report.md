# Category 3 – Automation & Dashboard

## My Understanding of Category 3

1. According to the task document, we need to simulate how a real rover system would automatically satrt when the system boots, given using ros service and telemetry service.
2. Since the instruction mentiooned void using wrappers, cron I used native tools like systemd.
3. the required software tools were systemd and ROS2 Humble


# Task 3.1

The requireents in this category
 * Services must start automatically on system boot
 * Services must start only after the network is available
 * There must be proper dependency ordering between services


# Findings - Errors that got solved while doing the task
 - First, I thought about storing the service scripts in the ROS workspace itself, but later understood that systemd runs independently from ROS workspaces. So in order to keep things simple, I creted a new folder in my home directory named ~/rover_services to store the scripts.

 - When the ROS service was created the termination occurred because the process was completed, as the command used was finite. This was altered by using a while loop to create an infinite loop.
 
 - To ensure services start automatically on system boot, I enabled them using systemctl enable. This made the service launch during startup itself without any manual steps.
 
 
The files include:
 * ros_service.service
 * telemetry_service.service
 * ros_service.sh
 * telemetry_service.sh
 * log.txt
 
 How the .sh and .service files work is:	systemd (.service) → runs → .sh scripts
 
Results & Findings:
 * Both services run continuously
 * Telemetry starts only after ROS
 * Stopping ROS stops telemetry
 * Thw workflow :	Network → ROS Service → Telemetry Service


# Task 3.2 

## Exp 1 – ROS Service Failure

What I changed: I Modified ROS service ExecStart path to invalid location. This change would cause a failure in the primary serivce due to unknown path

Result:
 * ROS failed with `203/EXEC`
 * Dependent telemetry service also affected as it depens on ROS service
 
* Service failed with `203/EXEC`
* Multiple restart attempts occurred
* `start-limit-hit` triggered


## Exp 2 – Telemtry Failure

What I changed: I Modified Telemetry service ExecStart path to invalid location. 

Result:
 * Service failed with `203/EXEC`
 * Multiple restart attempts occurred
 * start-limit-hit triggered
 * ROS service continued to run normally
 
 
## Exp 3 - Dependency Removal

What I changed: I removed the dependency location (`Requires=ros_service.service`) between telemetry and ROS service

Result:
 * Telemetry continued running even after ROS stopped
 * Dependency was no longer enforced
 * Telemtry runs independently
 

## Exp 4 - Crash Loop

What I changed: I modified the telemetry script to exit immediately.

Result:
 * Systemd repeatedly attempted to restart the service.
 * After multiple failures, restart attempts were stopped due to system limits
 * This limit showed how there is a solution against infinite restart loops
 

## Conclusion
I was able to build a proper setup and then intentionally break it in various ways to understand its behaviour.

Link to LLM used : https://chatgpt.com/share/69d7098e-69b8-8320-b5b0-7eed5a4cc5a8











