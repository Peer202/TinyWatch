# Board
## Issues while Assembly and testing Board V1.0
- Battery Holder blocks the UPDI Connector
- The Footprint for Transistor was wrong ( GND and Trigger need to reversed)
- LEDS dont actually turn off entirely when Transistor disabled
### Adding Decoupling Capacitor 
![Decoupling](image.png)
### Add Oscillator
If there are problems with Clock accuracy, add this
![Oscillator](image-1.png)
# Software