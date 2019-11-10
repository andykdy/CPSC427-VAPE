# Week: October 25

## Main Menu
Was already completed, but added an Exit button to allow for simple termination.

## Tutorial
Was already completed, but adjustments were made per Crossplay feedback to make things clearer and fix some stages.

## Additional enemy types
TODO

## Basic item pickups
TODO

## Run user studies on minimal viable game
Since we just ran the crossplay on it, we didn't do any additional testing during this week. We focused on making adjustments according to the crossplay feedback


# Week: October Nov 1

## Second Level + Boss
TODO

## Additional grpahical effects
TODO - New vamp mode graphical effects, Explosions, Lasers

## Bug Fixes
Fixed the bug in destruction code that was causing a segfault when attempting to the main menu. Continuous fixes as features were being merged

## Additional enemy types
TODO

# Week: Nov 8

## 3+ Levels
WIP - Maybe won't get this done. 

## Additional item pickups
TODO

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
Refactored Bosses, Projectiles, and Enemies (TODO) to have superclasses, making interaction/design of new enemies and bosses simpler.

## Level Design
Level design was improved, allowing further definition of a level beyond just the enemy timeline. 
Levels now declare what assets need to be used, and what the next level is. Points persist across levels.

