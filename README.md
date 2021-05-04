# MiniVent Platform IO Build

### Rice University - Global Medical Innovation & Oshman Engineering Design Kitchen

###### Authors: Varsha Thomas, Thomas Herring

This repository contains all the working code for the MAIN, follower1 (F1), and follower2 (F2) Arduino UNO microcontrollers to build the MiniVent device, a low-cost pediatric bridge ventilator. Feel free to fork it for your own development, we encourage collaboration! All mechanical parts and assembly instructions to build the predicate device (ApolloBVM - the adult bridge ventilator) can be found [here](http://oedk.rice.edu/apollobvm), a bill of materials, and a slide deck can be found in the google drive supplied by the 2020-2021 BIOE 530 Design Team

## System Overview

The MiniVent system uses three coordinated microcontrollers in order to facilitate user input while guaranteeing uninterrupted airflow to the patient. The system has a MAIN controller, responsible for managing the display and user input for the control panel, the F1 controller that actually sends motor commands to the servos, and an F2 controller that interfaces with the patient monitoring system which is comprised of a pressure sensor, and RGB LCD display to display patient parameters to the screen. 

The MAIN controller operates separately from both F1 and F2 and ensures that the system is real-time safe. In order to give more clarity about the technical operation of this device, we will outline the operation of each controller separately.

In order to facilitate the various functions of the device, F1 controller operates in 5 distinct states: load, on, off, and 2 states for an inspiratory hold maneuver. While in the ‘load’ state, F1 controller will read ventilation parameters from MAIN over the I2C bus, generate a new trajectory with these parameters, and turn its state to ‘on’. While in the ‘on’ state, the device will follow the trajectory over and over for infinity. This means that the F1 controller will never stop moving the motors unless it receives a command from MAIN to change its state to ‘stop’. If the master controller were to do so, the device would finish its current cycle and stop moving. In the first state for the inspiratory hold maneuver, F1 will generate a new trajectory that pauses the motors at the setpoint for 0.5 seconds, changes state to implement the inspiratory hold trajectory which it will follow for one cycle then reload the previous trajectory. 

The MAIN controller is mainly tasked with running the user display and sending commands to F1. Internally, MAIN switches which panel is currently displaying and manages input from the encoder and buttons. Each panel that the master displays is programmed to follow a different behavior and is modular. This makes it easy to implement panels that behave completely differently and interact with the F1 controller on the fly.

The F2 controller is the primary system for running the alarm display, sending and recieving commands to/from F1 for implementing an inspiratory hold maneuver, and reading and processing input from external sensors. In the current iteration of the device, F2 interfaces with a gauge pressure sensor. Internally, F2 manages input from the inspiratory hold button, reads pressure sensor data, manages output to the patient LED alarms, and displays patient respiration information to the user. 

## Installation Instructions

1. Install PlatformIO using the instructions [here](https://platformio.org/install).
   1. On Linux, run `pip3 install -U platformio` to install PlatformIO.
2. Clone this repository onto your local machine.
   1. On Linux, run `git clone https://github.com/apollobvm/apollobvm_pio.git` to clone the repo.
3. Build the PlatformIO project with either the core toolchain or IDE plugins.
   1. On Linux, run `pio run` to build.
4. Choose which environment to upload - `main`, `f1`, and `f2`, and specify with the `-e` flag.
   1. On Linux, run `pio run -e desired_env -t upload` to upload.
