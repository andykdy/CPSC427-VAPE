# Week: November 15

## Run user studies on game
Had another crossplay session. TODO talk about changes

## Opening backstory art
Added an intro state to the game, which displays a cutscene introducing the character and universe.

## Closing Epilogue
Added an outro state to the game, which displays a cutscene and credits.

## Implement Scoring
Basic scoring has been implemented since the start, and we've tracked leaderboard scores since last milestone.
Floating score indicator text was added which appears whenever you kill an enemy, indicating how many points you got for killing it.
Score was added to the UI panel.

# Week: November 22

## Bug Fixes
Various bug fixes done throughout, lots of issues to do with differing development environments / compilers. Also fixed a number of memory leaks.

## Implement Scoring Modifiers
Was not implemented.

## Finalized art assets
Done.

# Week: November 29

## Sound Effects and Music Finalized
Level 3 music added. Laser sound effects added. Player damage sound added, player death sound changed.

# Caried over from previous Milstone

## Additional enemy types
New enemy types were added
- Speedster enemy which charges up before speeding across the creen.
- Explosive Payload enemy, which will create a circular burst of bullets if you shoot it during its explosive phase.

## Additional item pickups
Item pickups were finally added. A new enemy type was added which will drop a random pickup on death.
Four pickups were created: 
- Health (heals player to full)
- Vamp Expansion (Increases vamp area size for the next/current use of vamp)
- Tri-Shot weapon (Gives the player the tri-shot weapon. Limited ammo, fires three bullets at 45/90/135 degrees)
- Machine Gun ewapon (Gives the player the machine gun weapon. Limited ammo, fires large number of bullets in rapid succession)

## 3+ Levels
Third level was added, integrating new enemy types.
Third boss was created, which has two phases.
- First phase: Charges at the player
- Secon phase: Spawns invincible clones which chase or shoot the player

# Implemented outside of schedule

## Improved Main Menu
More dynamic main menu, with actual buttons and a cursor, usable by keyboard or mouse.

## Player Lives
Track player lives, causing a game over after running out of lives.

## Save/Continue Playthrough
Save player data (lives, points, level) upon starting/restarting a level, allowing you to resume gameplay from the main menu.
Save data is cleared upon completing the game.

## Boss 2 Improvements
Laser beam textures, finish adding laser sweeping to patterns, and add enemy spawning and additional patterns.

## Vamp Mode Particle Effects
Advanced Vamp mode graphics were planned earlier, but they are now complete. Vamp mode now spawns particles when dealing damage, which travel towards the player and give him health.

## Video Playback
Integrated with ffmpeg to add in video playback for use in intro/ending

## Pause Menu
Added a pause menu, which displays a control diagram, and uses dynamic buttons like the main menu, for resuming/exiting.

## Between-level state
Was originally planned to show some interesting information, but currently just shows "Level n completed, press enter to continue" to add a break in-between levels

## Text Rendering
Truetype font rendering was added using stb_truetype.

## PhysFS
PhysFS integration was added to allow assets to be packaged into an archive to hide them from the user, but still be accessible at runtime.

# Milestone Components

## User Testing
Feedback from both crossplay sessions was taken into consideration and responded to. 
Multiple adjustments to the tutorial were made to make mechanics and controls more clear, and a very clear controls diagram was added after the intro.
UI was changed to be much clearer, changing health to green and adding a heart icon, and vamp to red and adding a fangs icon.
Boss healthbar was added, making it clear how far through the fight you are
Scaling was changed, so that everything is smaller. Easier to react and dodge enemies.
A state was added between levels to give a bit of a breather, instead of going straight into the action of the next level.


## External Integration
We integrated with FFmpeg/Libav to add video rendering. 
The VideoUtil class can demux/decode video files and read frames into pixel data, which is utilized by the Video Entity to create its texture each frame.

We also added text rendering using stb_truetype.

Also integrated PhysFS to load assets from an archive. 

## Advanced Graphics
Multiple particle systems: Explosions, Vamp mode particles, Boss2 laser priming/warning.
Does video rendering count?

## Advanced Gameplay
A* pathfinding was implemented, basically the same as I (Cody) had made for A2. 
The screen is divided up into a grid of configurable size, and entities are added to the grid, 
setting the value of the grid position to a certain type, depending on what they are (player, enemy, pickup, etc)
In debug mode (Shift+F) the grid can be seen, with squares colored based on what entity is inside them.
Furthermore, enemies that utilize pathfinding (only present in level 3) will have their path drawn as a set of dots.