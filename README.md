#  CPSC427 2019 Winter 1
![Image of logo](data/textures/logo.png)
# V.A.P.E. Nation (Vampire Astronaut Pirate Escape Nation)
## Team 6
- Andrea Park 45168151
- Andy (Dong Young) Kim 16785157
- Cody Newman 35833145
- Matteo Alps-Mocellin 32909146
- Sam Parhimchik 41037152
- Tanha Kabir 10850155

## Releases
- [Skeletal Game](https://github.students.cs.ubc.ca/CPSC427/vape_nation/releases/tag/skeletal-game)
- [Mimimal Playability](https://github.students.cs.ubc.ca/CPSC427/vape_nation/releases/tag/minimal-playability)
- [Playable Game](https://github.students.cs.ubc.ca/CPSC427/vape_nation/releases/tag/playable)

# Controls
- W, A, S, D  -> Movement
- Spacebar -> Shoot
- Enter ->  Vamp Mode / Skip Cutscenes
- Shift -> Continue (Dialogues)
- Escape -> Return to main menu (saves game to start of current level)
- [Debug] Shift+F to enter/exit debug mode
    - F -> Fill vamp mode
    - G -> Fill Health
    - V -> Invincibility
    - 1 -> At main menu only, go straight to level 1
    - 2 -> At main menu only, go straight to level 2
    - Tab -> At main menu only, go straight to the BetweenLevelState between level 1 and 2
    - 0 -> At main menu only, go straight to outro


# Content
The game consists of the main menu a tutorial, intro/outo, and two levels.

From the main menu, you can choose to continue a saved game (auto-saved at the start of each level / after you die. and cleared after beating the game), start a new game, play the tutorial, or exit the game.

Upon entering the tutorial, you will be walked through each of the controls and basic gameplay mechanics.

When you start a new game, you will be presented with an into cutscene before moving to the first level. Upon entering the first level the game will spawn waves of basic enemies for about a minute, before spawning the boss.
If you die, the level will restart and you will lose a life. Once all lives are lost you are returned to the main menu. Once the boss is defeated, you will be move to the next level.

Once you complete the last level you are presented with an outro and credits, before being returned to the main menu.


# Music Credits:
- Main Menu:      
    - Dynatron - Stars of the Night
- Intro
    - Hubrid & BMX Escape - Pursuit Force
- Tutorial BGM:   
    - M.O.O.N - Dust
- Level 1 BGM:    
    - Dynatron - Pulse Power
- Level 1 Boss:   
    - Perturbator - Diabolus Ex Machina
- Victory: 
    - Final Fantasy VII - Victory Fanfare
- Level 2 BGM:
    - Vulta - Crepuscule
- Level 2 Boss:
    - Pertrubator - Versus
- Outro:
    - Magic Sword - Awakening

# Video Credits:
- Intro/Outro:
    - Space Pirate Captain Harlock (2013)

# Dependencies
## Windows
- All dependencies are included
## Mac / Linux
- GLFW3
- SDL2
- SDL_Mixer
- FFmpeg
## Assets
As video assets cannot be stored on github, you will need to retrieve them yourself.

If the build process works as intended, cmake should automatically download the assets.

If you need to download a new version of the zip, just delete /data/data.7z and any videos and it will redownload next time you build. 
You may need to add an empty line to cmakelists.txt to force it to build.

Otherwise, you can always just comment out "download_assets()" in cmakelists.txt and download manually from the [google drive folder](https://drive.google.com/file/d/1a3yCBgBnr--UWfNw-07EkQegSBvIE2ON/view?usp=sharing)


# Building a release

- Build on the target platform with cmake, using release build type `-DCMAKE_BUILD_TYPE=Release`
- There should now be a `bin` folder containing the executable and any necessary dlls if on Windows.
- Zip up the `bin`, `data`, and `shaders` folders, along with the readme, and name appropriately indicating target platform.