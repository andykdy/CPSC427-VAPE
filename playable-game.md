# Week: October 25

## Main Menu
Was already completed, but added an Exit button to allow for simple termination.

## Tutorial
Was already completed, but adjustments were made per Crossplay feedback to make things clearer and fix some stages.

## Additional enemy types
Turtles were refactored out into an enemy superclass, and all turtle interactions were adjusted to be Enemy interactions.
A new basic enemy, the Generic Shooter, was added. It still just follows a simple linear path, but also fires bullets.

## Basic item pickups
The initial framework for item pickups was put in place, building an abstract Pickups class, and implementing the
appropriate logic in the levelState to manage it.


## Run user studies on minimal viable game
Since we just ran the crossplay on it, we didn't do any additional testing during this week. We focused on making adjustments according to the crossplay feedback


# Week: October Nov 1

## Second Level + Boss
A new level, set with its own background and music, was added utilizing new enemy types.
The second boss was created, which utilizes set patterns of lasers that can fire from six specific points
of his ship. Upon hitting half health, he switches up to faster/more difficult patterns.


## Additional grpahical effects
Explosion particle effects were added, and are generated upon enemy deaths.
Particle Effects are used for the boss2 laser beams.
New vamp mode graphical effects were planned, but were unable to be implemented in time. 
Will be moved into final milestone.

## Bug Fixes
Fixed the bug in destruction code that was causing a segfault when attempting to the main menu. 
Continuous fixes as features were being merged.

## Additional enemy types
A new enemy type which comes into the screen, stops at a certain position, and then acts as a turret, 
aiming towards the player to shoot for a few seconds, before continuing its path.

# Week: Nov 8

## 3+ Levels
We are only at two levels currently. It took much more time than expected abstracting out bosses and enemies, and levels,
but now that those are done, it will be significantly simpler going forward to create new enemies/bosses and just plug them into levels.


## Additional item pickups
TODO - Moved to final milestone

## Basic sound effects
Was already done a long time ago. Additional music was added and some sound effects were adjusted.


# Implemented outside of schedule

## Crossplay Feedback

We focused a lot on adjusted existing mechanics according to feedback from the crossplay

- Tutorial adjusted to be clearer
- Large changes to vamp mode, adjusting the way you use your vamp charge, and the way it damages enemies (siphoning health instead of straight killing)
- UI changes, moving healtbar and vamp to a panel at the bottom of the screen, and making it clear what each bar actually is with icons
- Boss1 changes
    - Adding a boss healthbar
    - Adjusting behaviour to be less random
    - Fixing fire rate
    - Adding vamp mode interactions with boss

## Explosion Effects
Explosion particle effects were created and are generated upon enemy deaths.

## Memory Management
Focused on adjusting systems that were leaking memory. 
Biggest culprit was that the ECS was keeping Entities around after it was supposed to, which was fixed.

## Refactoring and abstraction
Took a decent chunk of time, so should be noted. 
Refactored more stuff into ECS entities. 
Refactored Bosses, Projectiles, and Enemies to have superclasses, making interaction/design of new enemies and bosses simpler.

## Level Design
Level design was improved, allowing further definition of a level beyond just the enemy timeline. 
Levels now declare what assets need to be used, and what the next level is. Points persist across levels.

## Intro
A simple intro state was added to the flow of the game. Currently is just a screen of dialogue, but we will hopefully
get some kind of scripted animation or video library added to it.


# Optional Components Implemented

## Reloadability
Likely doesn't count for the whole 10%, but score saving has been added, saving the player's score after 
dying or completing the game. Currently, this saves the score with a default name, and only the highscore 
is displayed (in the window bar, beside points).

## Physics-Based animation
This has been a thing in our engine since the start, using movement involving 
acceleration, velocity, and friction decay.
Our ECS MotionSystem runs off of this, and all of our ECS entities use it to some extent, 
though some entities just use a static velocity with zero decay. 
Player has his friction turned up high, since this isn't the kind of game you want to be drifting around in.


## Complex Geometry
Boss 2 contains both a sprite texture for animation, and an internal mesh for collisions.
Bullets and player collide with it using accurate collision detection. 
For efficiency, an axis-aligned bounding box check is performed first, using a slightly larger box than the texture.

And not sure if it also counts for complex collisions, but we use the Cohen-Sutherland algorithm (https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm)
to test two rays check for collisions between boss2's lasers and the player.