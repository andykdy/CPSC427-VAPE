#  CPSC427 2019 Winter 1
![Image of logo](data/textures/logo.png)
# V.A.P.E. Nation (Vampire Astronaut Pirate Escape Nation)

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
- Enter ->  Vamp Mode
- Shift -> Continue (Dialogues)
- [Debug]
    - F -> Fill vamp mode
    - G -> Fill Health
    - 1 -> At main menu only, go straight to level 1
    - 2 -> At main menu only, go straight to level 2


# Content
The game consists of the main menu a tutorial, and two levels.

Upon entering the tutorial, you will be walked through each of the controls and basic gameplay mechanics.

Upon entering the first level the game will spawn waves of basic enemies for about a minute, before spawning the boss.
If you die, the level will restart. Once the boss is defeated, you will be move to the next level.

Once you complete the last level, you are returned to the main menu


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

# Video Credits:
- Intro:
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