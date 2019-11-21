# Week: November 15

## Run user studies on game
Had another crossplay session. TODO talk about changes

## Opening backstory art
Added an intro state to the game, which displays a cutscene introducing the character and universe.

## Closing Epilogue
Added an outro state to the game, which displays a cutscene and credits.

## Implement Scoring
Basic scoring has been implemented since the start, and we've tracked leaderboard scores since last milestone.
TODO talk about any changes

# Week: November 22

## Bug Fixes
TODO

## Implement Scoring Modifiers
TODO

## Finalized art assets
TODO

# Week: November 29

## Sound Effects and Music Finalized
TODO


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
TODO

## TODO

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

We also added text rendering using stb_truetype

## Advanced Graphics
Multiple particle systems: Explosions, Vamp mode particles, Boss2 laser priming/warning

## Advanced Gameplay
TODO