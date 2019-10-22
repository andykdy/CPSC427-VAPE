# Week: October 11

## Implement basic Vampire mode mechanics: 
Completed, mostly using the existing collision code. Collision with enemy triggers an “invulnerability” state for a few seconds to prevent multiple penalties in a small period of time.

## Calculations on health power, ‘vampower,’ & points systems:
Calculations for all 3 were mostly complete during the skeletal game. 
Health and vamp power now both have UIs. Points are displayed in the window title for now.

## Basic Enemy types/AI:
Was previously mostly complete. General basic behaviour for the simplest enemy type is implemented.

## Game Over
Previously complete, Game handles death by resetting the current level, and returns to the main menu on completion.

## User Studies
We were too busy to run user studies for the skeletal game, so that was ignored. We will be focusing on testing the minimally playable game.


# Week: October 18

## Basic Art assets
Was already semi-complete. New background assets were created and added, as well as a new main menu. Infinitely scrolling background added.

## Basic Sprite assets
Was already mostly complete from skeletal. Sprite sheet for the player was added, as well as new vamp mode.

## Vampire mode graphical effects/shaders
More finalized versions of the vamp mode graphics are now in place.

## First Level + Boss (approximately 2 minutes of gameplay)
Level wave spawning was implemented, allowing for the creation of a spawn timeline where waves of enemies spawn and move in patterns.
This made it easy to create a relatively interesting level using only the basic enemy type. First boss was already complete in skeletal.


# Ahead of schedule
## Main Menu
Completed in skeletal. Art adjusted

## Tutorial
A basic tutorial level was added, introducing the player to the controls in a simple environment. Needs to be refactored a bit


# Implemented outside of schedule
## Entity Component System
ECS was implmented, though not all functionality has been refactored.

## Sprite animation
Basic spritesheet based animation was added as part of the ECS refactor

## Dialogue system
Simple image dialogue system created for tutorial + boss conversation.
