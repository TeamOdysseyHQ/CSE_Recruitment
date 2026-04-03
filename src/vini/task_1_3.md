##
### Task 3: Encoder-Based RPM & Odometry

Here's a gist of what I gather from the task at hand

1. The OE-775 is a sensor attached to the shaft of the motor to count the rotation, giving the speed -RPM, and the position.
2. This data can later be used for tracking movement -Odometry, additionally the speed control.
3. The mechanism: The encoder produces pulses as the motor shaft spins, with more pulses indicating faster speed, and total pulses, the distance travelled.


What's the ISR-Interrupt Service Routine in this case?
- Encoder sends a pulse
- the hardware sets an interrupt flag
- the main codde is halted
- ISR runs as increments count
- return to running

This stuff is quite similar to arduino except that we have a few vars:
- allocator -- handles memory
- support  -- initializes micro ros
- node -- the ROS entity
- publisher -- sends/publishes the data
- executor -- runs the callbacks

 why adding a ``` volatile ``` to a simple ``` long pulseC = 0; ```
- the variable can change asynchronously.S
- the compiler automatically assumes that the ``` count ``` variable may never change, or cache a value in the register, leaving with a case of infinite loop.
