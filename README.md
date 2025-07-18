# FSIS
The repository contains source files and resources for FSIS game.
## Game description
FSIS is a PVE game in which you become a dark magician capable of controlling any matter.
### Prepare for danger!
On the map "Secret Shrik" you are confronted with an endless stream of monsters of five matters.
### Survive!
Your task is to hold out in this hellish place for as long as possible! You have a special attack in your arsenal, a ball of dark matter that kills any monster.
### Fight back the creatures of matter!
In order to destroy monsters of different materials, you need to constantly move around the map and select the material that suits you from the corresponding surface, because these monsters are only vulnerable to the materials from which they themselves created. But the secret is that your dark magician does not just take into account matter, he creates a ball from it, inside of which a charge of energy is placed, which causes damage, and the shell is needed so that the monster does not reject the charge, but absorbs it due to the impossibility of reflecting it . native matter.
### Test your abilities!
I almost forgot, you have a couple of trump cards up your sleeve for this battle - a special ability and a special mode that work great together. The special ability allows you to link to a beacon where you can teleport, and the special mode gives you a high chance of evading enemy attacks. This combination gives you greater mobility when transporting by car and changing visible material.
## Build
The project uses CMake and a C++17 compiler. All dependencies are downloaded automatically.
### Prerequisites
- [CMake](https://cmake.org/) version 3.30 or newer
- C++ compiler with C++17 support (MSVC, GCC, or Clang)
- [Git](https://git-scm.com/)
### Build steps
1. **Clone the repository:**
```sh
git clone https://github.com/Daedma/fsis.git
cd fsis
```
2. **Create a build directory and enter it:**
```sh
mkdir build
cd build
```
3. **Generate the project files**
```sh
cmake ..
```
4. **Build the project:**
```sh
cmake --build . --config Release
```
All third-party libraries (SFML, TGUI, Boost, EASTL, cpr) are automatically downloaded and built by CMake.
#### Using CMake presets
You can also use [CMake presets](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html) for easier configuration and building. For example:
```sh
cmake --preset=default
cmake --build --preset=Release
```
You can modify compilator and generator in `CMakePresets.json` used by CMake on that you prefer.
## Mentions
### Sprites
This project used characters generated using [Universal LPC Spritesheet Generator](https://sanderfrenken.github.io/Universal-LPC-Spritesheet-Character-Generator/#?body=Body_color_light&head=Human_male_light):

Authors: bluecarrot16, Benjamin K. Smith (BenCreating), Evert, Eliza Wyatt (ElizaWy), TheraHedwig, MuffinElZangano, Durrani, Johannes Sjolund (wulax), Stephen Challener (Redshrike)

- body/bodies/male/universal/light.png: by bluecarrot16, Benjamin K. Smith (BenCreating), Evert, Eliza Wyatt (ElizaWy), TheraHedwig, MuffinElZangano, Durrani, Johannes Sjolund (wulax), Stephen Challener (Redshrike) . Licenses: CC-BY-SA 3.0, GPL 3.0.
      - https://opengameart.org/content/liberated-pixel-cup-lpc-base-assets-sprites-map-tiles
      - https://opengameart.org/content/lpc-medieval-fantasy-character-sprites
      - https://opengameart.org/content/lpc-male-jumping-animation-by-durrani
      - https://opengameart.org/content/lpc-runcycle-and-diagonal-walkcycle
      - https://opengameart.org/content/lpc-revised-character-basics

- head/heads/human_male/universal/light.png: by bluecarrot16, Benjamin K. Smith (BenCreating), Stephen Challener (Redshrike). License(s): OGA-BY 3.0, CC-BY-SA 3.0, GPL 3.0.
      - https://opengameart.org/content/liberated-pixel-cup-lpc-base-assets-sprites-map-tiles
      - https://opengameart.org/content/lpc-character-bases

[Fireball sprite](https://www.reddit.com/r/AnimatedPixelArt/comments/zpwz9y/i_made_a_fireball_thing_idk_why/)
### Libraries
- [SFML](https://www.sfml-dev.org/) 
- [C++ Requests](https://github.com/libcpr/cpr) 
- [Boost](https://boost.org)
- [Mathter](https://github.com/petiaccja/Mathter)