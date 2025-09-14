# COIL 2025 — Flappy Bird (C + SDL2)

A minimal, fast Flappy Bird clone written in C with SDL2. Built for the COIL 2025 programming course. It favors clean modular code, clear constants.

---

## 🎮 How to Play

- **Flap / jump:** `Space`
- **Pause / Resume:** `P` or `esc` while playing

Fly through the gaps between columns. Each passed column adds to your score. Colliding with anything makes you lose a live, when you lose the three lives you have lost the game, so try it again, I know you can have a better score! 

---

## ✨ Features

- Movement with gravity and impulse flap
- Robust collision with columns and world bounds
- Procedural columns and gap placement
- HUD with current score and best score (saved locally)
- Start → gameplay → game-over flow with simple menu
- Keyboard input handling
- Centralized constants for easy tuning (speed, gap, tiles, number of columns)
- Multi-file structure with straightforward naming and comments

**Not included yet**
- Multi-platform builds (Windows pending)
- Resizable window with dynamic re-layout

> **Rubric coverage:** implements the full course rubric **except**:
> - ❌ **Multi-platform build** (Unix only)
> - ❌ **Resizable window**

---

## 📁 Project Layout
You can see our working tree and algorithm graphic in the game plan folder

---

## Requirments for playing our game

```bash
# first run:
sudo apt update

#Then if you dont have one of the following requirments:
### Cmake
sudo apt install build-essential cmake

### SDL2
sudo apt install libsdl2-dev

### SDL2_image
sudo apt install libsdl2-image-dev

### SDL2_TTF.H
sudo apt-get install libsdl2-ttf-dev
```
---

## ⚙️ Build & Run for Linux (Ubuntu)

We use **CMake** to generate a Makefile and build in a separate `build/` folder. Use the following comands from the build folder:

```bash

# Step 1: Configure (rerun only if CMakeLists.txt changed)
cmake ..

# Step 2: Compile
make

# Step 3: Run
./flappy_bird

```
