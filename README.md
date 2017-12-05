# Project 3 - LCD Game (Snake!) - CS3432 Fall 2017
--
*Steven Brooks, last updated December 5th, 2017*
## Introduction
This project implements the game of Snake on an MSP430 with LCD LaunchPad attachment. The snake's direction can be changed via the four buttons on the LaunchPad attachment. In order, they are: Left, Up, Down, Right. The game is lost when the snake collides with the fence of the play area, or it collides with its own body. The score is increased (and the snake is grown) when the snake's head collides with the apple. Reach a score of 25 to win!

## Setup & Running
In order to run this program:

1. Clone or download this repository.
2. Navigate to the root folder of the repository.
3. Run `$ make`.
4. Ensure the MSP430 is plugged into the computer.
5. Load the program onto the MSP430 with `$ make load`.

If you wish to clean up the object files created during compilation, simply run `$ make clean` in the root folder of the repository.

## Program Overview
The program makes use of several provided libraries that are not described here. A high level description is given of the follow "classes": Snake, Apple, and Speaker. Snake defines the functionality of the snake (how to draw, movement, etc.) Apple defines the functionality of the apple (how to draw, respawning.) Speaker defines the functionality of the speaker on the LaunchPad attachment (turning on, off, setting tone.)

### Snake
The following table describes each function in the Snake header and what it does.

| Name | Functionality  |
|---|---|
|  `snakeInit()` |  Initializes the snake to default values (size 0, center of screen, etc.)|
|  `snakeUpdate()` | Updates the position of the snake according to its current direction. |
|  `snakeChangeDirection(char dir)` | Updates the snake's direction according to the passed bit vector, corresponding to the bits of the switches on P2. |
|  `snakeIsEatingSelf()` | Returns **true** if the snake's head is on top of one of its body segments, **false** otherwise. |
|  `snakeIsEatingApple()` | Returns **true** if the snake's head is on top of the apple, **false** otherwise. |
|  `snakeIsOutOfBounds(Region* bounds)` | Returns **true** if the snake's head has collided with the passed bounds, **false** otherwise. |
|  `snakeGrow()` | Grows the snake by one segment. |
|  `snakeDraw()` | Draws the snake on the screen. |

#### Points of Interest
Efficiently drawing and managing the snake was an interesting exercise. Initially, the program tried to store an entire Layer object for each segment. This was highly inefficient usage of memory, as it stored a lot of redundant information: each segment was the same shape, color, etc. Furthermore, drawing the entire snake every cycle would be *very* slow for larger snakes.

To draw the snake efficiently, the program only draws the *new* position of the head and "erases" the previous position of the tail (using the tail layer.) This means that regardless of the snake's size, the program only has to draw two snake segments: one for the new head, and one for the old tail (matching the background color, i.e. "erasing.")

Since the program handles drawing this way, the only information that we need to store for each segment is a running history of the head's positions, and at what point in that history the tail should be at. An array of position vectors is used to store this information, much more memory efficient than a layer for each segment.

### Apple
The following table describes each function in the Snake header and what it does.

| Name | Functionality  |
|---|---|
|  `appleRespawn(Vec2* newPos)` |  Respawns and redraws the apple at the passed X,Y position vector. |

### Speaker
The Speaker header defines four methods. Speaker code was also provided by Dr. Freudenthal for a previous lab and refactored slightly, but mostly unchanged.

| Name | Functionality  |
|---|---|
|  `speakerInit()` |  Initializes the speaker for output. |
|  `speakerOn()` |  Enables output on the speaker. |
|  `speakerOff()` |  Disables output on the speaker. |
|  `speakerSetTone(char tone)` | Sets what tone the speaker should be playing according to the passed `short`. |

## Attributions
Several sources were used in the completion of this lab.

1. Dr. Eric Freudenthal's provided timer, shape, LCD utility, and switch libraries and and demo programs (and, of course, the Makefiles that accompanied them).
