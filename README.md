# Space Ship Simulation

This is an OpenGL project written in C++ for Computer Graphics class.

![image1](https://user-images.githubusercontent.com/36446866/81590761-febe1f80-93c3-11ea-9291-acfae502048f.png)

![image2](https://user-images.githubusercontent.com/36446866/81590795-139ab300-93c4-11ea-883a-9f4b7ceb4032.png)

## Controls
### Playing
* You can turn the spaceship left/right using the **left**/**right** **arrow** keys.
* You can speed the spaceship up or slow it down using ’**a**’ and ’**d**’ keys. These controls do not apply in station view.
* Pressing ’**j**’ increases the station’s rotational speed by a constant amount. (As a result, the station starts rotating faster.) Similarly, hitting ’**k**’ decreases the station’s rotational speed by a constant amount.
* When the ’**p**’ is pressed, the simulation pauses. When ’**p**’ is pressed a second time the simulation resumes.
* When the **right mouse button** is pressed, the simulation enters paused state. By repeatedly hitting the **right mouse button** the program will perform consecutive single steps.
* When the ’**o**’ is pressed, autopilot mode is enabled so that the spaceship begins to move towards the station. When it reaches to the station, it stops in the station. You can cancel autopilot mode by pressing '**o**' again.

### Viewing
* If you press ‘**c**’ key, the viewer is switched to the control desk (in front of the tetrahedron) of the spacehip and looks in the direction that the spaceship moves.
* If you press ‘**s**’ key, the viewer is switched to the front of the station and looks in the direction of its front.
* If you press ‘**t**’ key, the viewer is positioned behind and slightly above the spacehip.
* If you press ‘**w**’ key, the viewer is positioned way up on z-axis so you can see all objects.
* If you press ‘**z**’ key, the viewer is positioned up of the spaceship and this mode allows you to zoom in and out with ‘**+**’ and ‘**-**’ keys correspondingly.

## Extra Features

* The torus of each planet rotates.
* There is an NPC space ship that moves in a certain region.
* Zoom view mode allows to zoom in or out to achive a decent view.
* Autopilot mode allows to travel to the station.