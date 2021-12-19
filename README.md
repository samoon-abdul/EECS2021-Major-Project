# Major Project AVR programming - Regenerative Motor Braking

# Components list:
- ATmega328PU microcontroller
- Jumper Cables
- Standalone Button
- LED and Resistor
- Motors
- Seeedstudio Grove Board Button
- SNAP Debugger PG164100
- MPLAB X IDE
# Overview:
URL for project video and project showcasing is: https://youtu.be/mNzDOFX5dHM

Major Project for EECS2021. Aims to showcase the inner workings of the ATmega328PU via Pulse Width Modulation. Uses C, Icarus Verilog, Inline Assembly in combination with the ATmega328PU microcontroller with the MPLAB X IDE to communicate with the Microcontroller. The SNAP debugger is used to burn the program onto the controller while MPLAB X is used to write the program. 


# Function: 
A button is pressed which will be the power source for the first motor, after the first motor stops the second spins in the opposite direction to simulate regenerative generation whilst incrementing a battery value.
