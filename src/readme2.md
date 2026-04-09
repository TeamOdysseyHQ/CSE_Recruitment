# Task 2

Honestly, this task alone took twice the amount of time as Tasks 1 and 3, and I will try to put out all of the workflow and problems encountered. 

An approximate flow looks something like this:
1. the ``` rover.xacro ``` file defines the physical properties of the tover.
    - A base joint connects the basee_footprint to the base_link with a lift of 0.15m. [i encountered issues where the simulation was merging with the ground level]
    - Assumed mass of the base_link - 628kg, and accordingly initialized values of inertia. 
    - The ``` meshes ``` directory is then referred to, scaled to meters using 0.001 0.001 ..
    - There is a mix of revolute joints for the steering and, furthermore, continuous joints for the wheels.

2. Configuration of the control
    - I've used a ``` diff_drive_controller ``` 
    - For the wheels, the 6 of them are divided into left_wheel and right_wheel linearly.
    - The distance between two wheels is 0.50m and the radius is 0.10 [trial and error]
        * Doesn't seem to be workign since it crashes within 5 secs from the start of the simulation

3. Launch
    - Use of Gazebo Classic
    - the nodes publish the ``` robot description ``` and handle the TF tree for the robot's internal joints.
    - now, a node publishes the rover into gazebo at coordinates of z=0.0 usinf the SPawn Entiity.
    - The race conditions are prevented using a timer action delaying the ``` joint_state_behaviour ``` and the rover drive controller.

4. Bonus
    - I haven't been able to achieve an ``` rviz ``` simulation, even though the initial gazebo sim runs

The errors: 

1. The first few were of script errors, mesh/controller initialization mismatches
2. Then came the beautiful few, that derailed my progress by almost a day which actually showed up because the body colors of silver/gold are unavailable. There are a quite a few warnings that show up even after everything works.
3. The publisher - subscriber model fail :
``` python 
[spawner-5] [WARN] [1775756657.214692222] [spawner_joint_state_broadcaster]: Could not contact service /controller_manager/list_controllers
[spawner-5] [INFO] [1775756657.215684113] [spawner_joint_state_broadcaster]: waiting for service /controller_manager/list_controllers to become available...
```
I can't quite understand where to troubleshoot for this. Although I did notice one of the logs mentioning that I will have to save a connection (not sure). (https://claude.ai/share/65147530-a22f-4bde-b820-28df28cb47b5)


