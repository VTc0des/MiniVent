# ApolloBVM Platform IO Build

### Rice University Oshman Engineering Design Kitchen

###### Author: Varsha Thomas, Thomas Herring

![Build Status](https://travis-ci.com/apollobvm/apollobvm_pio.svg?token=yNiqGT423d2fSsg9ELix&branch=master)

<!--[![Build Status](https://github.com/apollobvm/apollobvm/workflows/test/badge.svg)](https://github.com/arduino/arduino-cli-example/actions)-->

This repository contains all the working code for both the master and slave Arduino to build the ApolloBVM. Feel free to fork it for your own development, we encourage collaboration! All mechanical parts and assembly instructions can be found [here](http://oedk.rice.edu/apollobvm).

For those looking to just download and run the code, please refer to the [releases](https://github.com/apollobvm/apollobvm/releases) section and downloaded the latest firmware. Otherwise, please follow the instructions below on install

For any issues with the code, electrical setup, or mechanical components, please file a bug report [here](https://github.com/apollobvm/apollobvm/issues).

## System Overview

The ApolloBVM system uses two coordinated microcontrollers in order to facilitate user input while guaranteeing uninterrupted airflow. The system has a master controller, responsible for managing the display and user input, and a slave controller that actually sends motor commands to the servos. This allows the master controller to operate entirely separately from the slave and ensures that the system is real-time safe. In order to give more clarity about the technical operation of this device, we will outline the operation of each controller separately.

In order to facilitate the various functions of the device, the slave controller operates in 3 distinct states: load, on, and off. While in the ‘load’ state, the slave controller will read ventilation parameters from the master over the I2C bus, generate a new trajectory with these parameters, and turn its state to ‘on’. While in the ‘on’ state, the device will follow the trajectory over and over for infinity. This means that the slave controller will never stop moving the motors unless it receives a command from the master to change its state to ‘stop’. If the master controller were to do so, the device would finish its current cycle and stop moving.

The master controller is mainly tasked with running the user display and sending commands to the slave. Internally, the master switches which panel is currently displaying and manages input from the encoder and buttons. Each panel that the master displays is programmed to follow a different behavior and is modular. This makes it easy to implement panels that behave completely differently and interact with the slave controller on the fly.

## Installation Instructions

1. Install PlatformIO using the instructions [here](https://platformio.org/install).
   1. On Linux, run `pip3 install -U platformio` to install PlatformIO.
2. Clone this repository onto your local machine.
   1. On Linux, run `git clone https://github.com/apollobvm/apollobvm_pio.git` to clone the repo.
3. Build the PlatformIO project with either the core toolchain or IDE plugins.
   1. On Linux, run `pio run` to build.
4. Choose which environment to upload - `master`, `slave`, `samd_master`, and `samd_slave` - and specify with the `-e` flag.
   1. On Linux, run `pio run -e desired_env -t upload` to upload.
