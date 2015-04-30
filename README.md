# kart

Kart simulator is a usefull tool to review your go karting races times on a graphical maner. All you need is to get the track coordinates and the lap times for each pilot and you can achieve results as show bellow.

## See kart simulation working

  [http://youtu.be/zt8DS1JDZ7w](http://youtu.be/zt8DS1JDZ7w)

## General Requirements

To compile kart simulator you will need g++ (tested with version 4.8.1) and a few libraries. On ubuntu:

* sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev

To compile:

* make
 
The result is the `kart` app. When openning you will get a window like this:
![App example](http://s14.postimg.org/sgai09vy9/Screenshot_from_2015_04_30_08_41_07.png)

To start the race press "SPACE" key.
There are 3 available simulation speeds which can be controlled with 1 (normal), 2 (fast), 3 (super-fast) keys.

## Configuring kart simulator to your own needs.

### Changing the race background image

Change src/main.c line that reads:
  
  background = image_new("sanmarino12counter.png");
  
And point it to your background image.

### Changing pilot names

Change src/data.c define NRUNNERS to indicate the number os pilots and change names_ array to modify pilot names. You might need to change colors_ array in the same file to make it the size of NRUNNERS definition.

### Changing race coordinates

`kart` app uses bezier coordinates to create the track on top of a background image. You can generate your coordinates using an excelent online tool: 
[BezierTool](http://www.victoriakirst.com/beziertool/)
Set the points generated on the coordinates_ array on src/data.c file

### Changing pilot lap times

Change src/data.c laptimes_ array to set each pilot lap time (in milli seconds). 
