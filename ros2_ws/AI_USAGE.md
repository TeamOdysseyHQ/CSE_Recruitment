# AI Usage Documentation

## Overview

During the development of this project, AI tools were used mainly as a learning assistant to understand how ROS2 and micro-ROS systems work.

Since the project involved technologies that were new to me, AI was used to clarify concepts such as ROS2 architecture, micro-ROS communication, node structure, and project organization.

Instead of directly generating the final solution, the AI was mainly used to guide the learning process and explain concepts step-by-step.

Below are examples of the prompts used and simplified explanations of the responses.

---

# AI Interaction Log

## 1. Understanding What the Project Was Asking

**Prompt**

I’m working on a robotics software task using ROS2 Humble and micro-ROS.  
The hardware will eventually be a Teensy microcontroller, but I don’t actually have the hardware right now.

How should I approach building the software side of the project?

---

**Prompt**

If the microcontroller isn’t available yet, can I still build the ROS2 side of the system?

---

**Prompt**

What would the overall architecture look like for something like this?

---

**AI Response  **

The AI explained that the system could still be developed without the hardware by creating the ROS2 components first.

It suggested designing the system with three main parts:

- a micro-ROS node on the microcontroller
- a micro-ROS agent running on the computer
- a ROS2 node that subscribes to the data

This way, the software structure would already be ready when the hardware is connected.

---

## 2. Understanding How micro-ROS Communicates with ROS2

**Prompt**

How does a microcontroller actually communicate with ROS2?

---

**Prompt**

What exactly does the micro-ROS agent do?

---

**Prompt**

Does the microcontroller talk directly to ROS2 or does the agent handle that?

---

**AI Response  **

The AI explained that microcontrollers cannot run the full ROS2 system because of limited resources.

Instead, they run a lightweight micro-ROS client that sends messages to a computer through the **micro-ROS agent**.

The agent acts as a bridge that converts micro-ROS communication into normal ROS2 messages.

---

## 3. Creating a Clean Project Structure

**Prompt**

What should the folder structure of a ROS2 project look like?

---

**Prompt**

Where should the ROS2 nodes go?

---

**Prompt**

Where are launch files normally stored in a ROS2 package?

---

**AI Response  **

The AI explained that ROS2 projects are typically organized as a workspace containing a `src` folder where packages are stored.

Each package contains the node code, configuration files, and launch files.

This structure allows ROS2 to build and run the project using tools like `colcon`.

---

## 4. Writing micro-ROS Publisher Code

**Prompt**

I need a simple example of micro-ROS code for a Teensy microcontroller.

---

**Prompt**

Can it just publish a counter value every second?

---

**Prompt**

Could you also explain what each part of the code is doing?

---

**AI Response  **

The AI provided an example program that creates a micro-ROS node and publishes a counter message repeatedly.

It also explained how the initialization works and how the loop function sends messages at regular intervals.

---

## 5. Writing a ROS2 Subscriber Node

**Prompt**

If the microcontroller publishes data, how do I receive it in ROS2?

---

**Prompt**

Could you show an example subscriber node in Python?

---

**Prompt**

It should just print the values that it receives.

---

**AI Response  **

The AI described how to create a ROS2 subscriber node using Python.

It showed how to subscribe to a topic and how the callback function prints the incoming messages to the terminal.

---

## 6. Launching the ROS2 Node

**Prompt**

What is the purpose of a launch file in ROS2?

---

**Prompt**

Do I need one if I only have one node?

---

**Prompt**

How would I write a simple launch file for my subscriber node?

---

**AI Response  **

The AI explained that launch files allow ROS2 nodes to be started automatically.

Even if a project only has one node, using launch files helps keep the system organized and makes it easier to start the application later.

---

## 7. Testing the System Later with Hardware

**Prompt**

Once the microcontroller is connected later, how would I test if everything is working?

---

**Prompt**

Are there ROS2 commands to check if topics are being published?

---

**Prompt**

How do you see messages being sent through a topic?

---

**AI Response  **

The AI explained that ROS2 has command-line tools to inspect the system.

These commands allow users to:

- list active topics
- check which nodes are running
- view messages being published

This helps verify that communication between nodes is functioning correctly.

---

# Conclusion

AI tools were mainly used to understand robotics software concepts and to guide the development process.

The AI acted as a learning assistant that helped explain how ROS2 and micro-ROS systems work, how nodes communicate, and how to structure the project properly.