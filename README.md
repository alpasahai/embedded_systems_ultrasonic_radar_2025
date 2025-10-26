# embedded_systems_ultrasonic_radar_2025
2303ENG - Embedded Systems Individual Project: Ultrasonic Radar
Embedded ultrasonic radar built on the TM4C1294 Tiva board using C in Keil!

DESCRIPTION:
I’m creating an ultrasonic radar that is able to detect multiple objects in one scan - an LCD is used to display the distance from the ultrasonic ranger (USR) and the angle at which it is positioned. A buzzer can be used to let the user know when an object is within the threshold of the USR (5.00 cm). There’s going to be two modes:​
  > Automatic Sweep - when the USR is moving on it automatically, and detecting objects - this is basically going to be coding the servo motor to go from 0 -180 degrees and then back​
  > Manual Sweep - when the USR is detecting an object based on where the user wants - this is done by utilising the joy-stick (ADD-ON)

EXTENDED FUNCTIONALITY: 
  > Using a joystick to manually control the angle at which the USR is facing – this is done by incrementing the angle within the servo-motor function, as this is what determines which way the      USR will be facing.​
  > Using an LCD Display – Having a really cool radar graphic on the LCD just for visual effects and better visualisation for where the object is.​
  > Using a buzzer to let the user know when the object has reached/is within threshold- this is 5.00 cm from the USR, and if the object is within/on threshold, the buzzer will start blaring.

TESTING EACH COMPONENT: 
  > Testing Servo Motor – Use of oscilloscope – forming a square wave that falls within the 20ms period, showcase the testing code (Tera Term)​
  > Testing Ultrasonic Radar - Showcase the testing code (Tera Term) - showing the Rising Edge and Falling Edge, printing to see if the register had changed​
  
  Testing Add-Ons:​
  > Joystick – Tera Term (printing out the ADC – sensing the potentiometer) + Test Code​
  > LCD – SSI Testing (LCD Demo Code), Visual from Screen​
  > Buzzer – Audio Testing - Messing with the beep length and pitch

IMPROVEMENTS:
  > Using the Y-axis on the joystick to alter the speed of the radar (increase/decrease the increments of the angle)​
  > Making sure all the components run smoother together - "code and noise interference" - adding in msDelay() functions. 

FUTURE FEATURES:
  > Making it move! – using a Tiva Board, car chassis (LINK)​
  > Allowing different functionalities for the buzzer – appropriate music when scanning, sound effect when the buzzer locates an object, etc.

FINAL THOUGHTS: 
This was a creative and challenging project that I enjoyed doing! There are components of the project that can be improved, but overall, the project has met its requirements and its add-ons.
