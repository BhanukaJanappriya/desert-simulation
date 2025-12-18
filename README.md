# 3D Desert Exploration Engine 🌵☀️

A comprehensive 3D graphics project built with **C++** and **OpenGL (GLUT)**. This engine features a procedurally generated desert environment with dynamic weather systems, day/night cycles, interactive structures, and organic terrain generation.

## ✨ Key Features

### 🌍 **Procedural Environment**
* **Terrain Generation:** Heightmap-based terrain using sine/cosine interference patterns.
* **Fractal Forest:** Procedurally generated trees using recursive algorithms (fractals) to create unique organic structures.
* **Dynamic Water:** Animated lake surface simulating waves using vertex displacement.

### 💡 **Advanced Lighting & Atmosphere**
* **Day/Night Cycle:** Continuous orbital sun and moon system that creates realistic transitions.
* **Dynamic Lighting:**
    * **Directional Light:** Simulates the sun/moon.
    * **Spotlights:** Interactive flashlights (Lighthouse, Museum beams).
    * **Point Lights:** Campfires and torches with flickering animation.
* **Volumetric Fog:** Exponential fog that reacts to the time of day (appears at night, clears at day).

### 🌪️ **Particle Systems & Physics**
* **Dust Storms:** Volumetric billboarding technique to simulate sandstorms drifting across the dunes.
* **Sand Particles:** Floating particles rendering with alpha blending.
* **Animation:** Rotating windmills, floating hot air balloons, and rolling barrels.

### 🏠 **Interactive Architecture**
* **Complex Models:** Custom-built structures (House, Museum, Lighthouse) created from geometric primitives.
* **Interactivity:**
    * **Open/Close Door:** Animated hinge rotation using 'O'.
    * **Light Switch:** Toggle interior house lighting using 'G'.

---

## 🎮 Controls

| Key | Action |
| :--- | :--- |
| **W / S** | Move Camera Forward / Backward |
| **A / D** | Strafe Camera Left / Right |
| **E / Q** | Fly Up / Down |
| **Arrow Keys** | Look Around (Yaw / Pitch) |
| **G** | Toggle House Interior Spotlight |
| **O** | Open / Close House Door |
| **T** | Reset Camera View |

---

## 🛠️ Technical Stack

* **Language:** C++
* **Graphics Library:** OpenGL (Legacy/Fixed-Function Pipeline)
* **Window Management:** GLUT / FreeGLUT
* **Texture Loading:** SOIL2
* **IDE:** Visual Studio

---

## 📂 Project Structure

This project follows a modular architecture for maintainability:

* **`Main.cpp`**: Handles the main game loop, user input (Keyboard/Mouse), and window management.
* **`Init.cpp` / `Init.h`**: Manages initialization of OpenGL state, texture loading, and random position generation.
* **`Render.cpp` / `Render.h`**: Contains all rendering logic (drawing terrain, houses, lighting, particles).
* **`Global.cpp` / `Global.h`**: Central "Database" for shared variables, constants, and state management.

---

## 🚀 How to Run

1.  **Clone the Repository**
    ```bash
    git clone [https://github.com/your-username/your-repo-name.git](https://github.com/your-username/your-repo-name.git)
    ```
2.  **Open in Visual Studio**
    * Open the `.sln` file.
3.  **Dependencies**
    * Ensure **GLUT** and **SOIL2** libraries are linked correctly in your project settings (Linker > Input).
    * *Note: Textures are loaded from absolute paths in `Init.cpp`. Update the paths to match your local directory structure if textures do not load.*
4.  **Build & Run**
    * Select **Debug** or **Release** (x86/x64).
    * Press **F5** to compile and run.

---

## 📸 Screenshots

| Day Scene | Night Scene |
| :---: | :---: |
|<img width="1918" height="987" alt="Screenshot_3" src="https://github.com/user-attachments/assets/d3d10f4c-8058-4184-bd01-2cf392610b4a" /> |<img width="1918" height="990" alt="Screenshot_2" src="https://github.com/user-attachments/assets/1f7996a8-ba10-4619-b633-ca0a0e153543" />|


| Weather Effects |
| :---: |
|<img width="1918" height="983" alt="Screenshot_1" src="https://github.com/user-attachments/assets/f20590f0-ddd2-4c23-94f1-ec105f9e5de0" />|



## 👨‍💻 Author

**Bhanuka Janappriya**
* Project for CSC_3081
* Department of Statistics and Computer Science
* Faculty of Science
* University of Peradeniya

---

*This project was created for educational purposes to demonstrate the fundamentals of 3D computer graphics.*
