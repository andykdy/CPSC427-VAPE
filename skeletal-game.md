# Week: October 4
## Basic collision detection: 
Completed, mostly using the existing collision code. Collision with enemy triggers an “invulnerability” state for a few seconds to prevent multiple penalties in a small period of time.

## Preliminary game states: 
Completed, a simple state machine was implemented at the core of the engine, and the game can very simply change between states (Currently main menu and level)

## Player controls
Completed, WASD movement, Space to shoot, Enter to enter vamp mode, and F as a debug to charge vamp mode

## Grey box assets
Surpassed, Some assets (vamp mode effect primarily) are still simple assets, but most entities have also been given dedicated sprites.


# Implemented ahead of schedule
## Implement basic Vampire mode mechanics
Completed: destroying 10 enemies allows the player to press enter to go into Vamp mode - this lets the player steal health from enemies by destroying them in an area of effect around you as well as slows down time

## Flesh out Main Menu UI
Completed, starting screen now has a button that can be interacted with to start the game. Previously initiated by pressing “space”
Calculations on health power, ‘vampower,’ & points systems
Mostly completed. A health/power bar system has been created, and the vamp power system is implemented and usable but doesn’t have a UI element yet. Points exist and are shown in the window title, but also need a UI element.

## Basic Sprite assets
A first batch of sprites relevant to the current mechanics were added

## First level + boss
Partially completed. An initial version of the first boss has been created, and spawns after 30 seconds in the level.
