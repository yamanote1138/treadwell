# Treadwell

![silly little tank](https://raw.githubusercontent.com/yamanote1138/treadwell/main/resources/treadwell.jpg)
Simple tank robot with ESP8266 and WebSockets :)

## Introduction

I had an old [SRV-1 Surveyor Robot](http://www.robotcombat.com/products/IL-SRV1Q.html) laying around and decided I wanted to update it with more modern tech. The aluminum chassis, motors, tank treads, and 7.5v lipo were all in great shape and ripe for reuse. I opted for the Wemos D1 Mini as the base as they're low-power, resilient, and easy to use.

The sketch sets up web and websocket servers that allows you to control the robot via a tidy, mobile-friendly, web ui.

Motion control is very basic (forward, backward, rotate left, rotate right, stop), but could easily be expanded. Check out the robot in action [here](https://www.youtube.com/watch?v=0ZynWxu7_jY).

## Documentation

Detailed instructions for [hardware setup](https://github.com/yamanote1138/treadwell/wiki/Hardware-Setup), [software installation](https://github.com/yamanote1138/treadwell/wiki/Software-Installation), and [operation](https://github.com/yamanote1138/treadwell/wiki/Operation) are available in the [wiki](https://github.com/yamanote1138/treadwell/wiki/)
