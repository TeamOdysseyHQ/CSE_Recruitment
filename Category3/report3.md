reference - https://claude.ai/share/7f9a41db-b25f-4bb1-9f20-6351c50de0a2 (this link contains both category 1 and category 3)

was not able to execute category 3 due to errors in saving the service files 

according to my understanding in this task 
when the ground control computer boots up, two programs need to automatically a program that talks to the rover and a program that records rover data for which i have made two files (ROS 2 Bridge and Telemetry Logger  ) 

since we should not use cron, timers or wrappers i have used systemd (in linux it manages what program starts when our computer boots) so i have also made serivce files which help communicate with the systemd.

then we have to break or corrupt our files and show the errors

Delete or corrupt the scripts
Remove permissions
Break the config file 
Delete the service files entirely