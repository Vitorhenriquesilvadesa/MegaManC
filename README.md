# **Megaman Game Prototype**  

This project is a 2D game prototype inspired by *Megaman*, developed in C with OpenGL. It includes a robust architecture with dynamic systems for animation, movement, input handling, and more. Currently, the game only supports Windows Operating System because the thread library used for AudioAPI is `<windows.h>`.

## **Implemented Features**  

### **Core Features**  
- **Game Loop**:  
  - Centralized update loop for managing game logic.  
  - Delta time calculation for smooth animations and physics.  

- **Entity System**:  
  - `Megaman` entity with position, size, and sprite rendering capabilities.  
  - Entity-specific update functions for custom behavior.  

### **Graphics System**  
- **Sprite Rendering**:  
  - Support for sprite animations using texture atlases (sprite sheets).  
  - `Idle` and `Walk` animations with seamless transitions.  
  - Mirroring of sprites based on movement direction.  

- **Orthographic Projection**:  
  - Resizable viewport while maintaining the aspect ratio.  
  - Dynamic handling of screen dimensions for proper rendering.  

- **Shaders**:  
  - Vertex and fragment shaders for 2D rendering, including texture sampling.  

### **Input System**  
- **Keyboard Input**:  
  - Key press detection using GLFW.  
  - Player movement mapped to directional keys (`Left` and `Right`).  

### **Audio System**  
- **Asynchronous Audio Playback**:  
  - Audio API supports both WAV and OGG formats with asynchronous playback.  
  - Uses OpenAL for audio management and `stb_vorbis` for decoding OGG files.  
  - Asynchronous loading prevents audio playback from blocking the game loop.  
  - Threading is handled using Windows' native `<windows.h>` library for managing background audio tasks.  

- **Audio API**:  
  - Custom API designed for loading, buffering, and playing sound effects asynchronously.  
  - Supports both WAV and OGG files with automatic thread management for non-blocking audio.  
  - Designed to minimize performance overhead during gameplay.

### **Event Trigger API**

The **Event Trigger API** is a system designed to handle events based on specific conditions within the game. It allows the game logic to trigger actions when certain conditions are met, such as player interactions, environmental events, or other game state changes. This system is key to implementing dynamic behavior, like triggering animations, sound effects, or other actions in response to game events, without blocking the main game loop.

#### **Key Features**
- **Conditions and Actions**:  
  - Events are made up of two components: a **condition** function that checks if an event should be triggered, and an **action** function that executes when the event is triggered.
  - The condition function takes a context as an argument and returns a boolean value. If the condition evaluates to `true`, the corresponding action is executed.
  - The action is executed immediately when the event is triggered.

- **Trigger Status**:  
  - Each trigger has a **status** indicating whether it has been triggered (`PERFORMED`) or is waiting to be triggered (`WAITING`).
  - When the condition is met, the status changes to `PERFORMED`, and the action is executed.
  - If the condition is no longer met, the trigger status is reset to `WAITING`, allowing it to be checked again in the future.

- **Event Management**:  
  - The API supports registering multiple triggers, each with a unique condition and action. Triggers are processed each frame to determine whether their condition is satisfied.
  - The system is designed to handle events dynamically, expanding internal storage as new triggers are registered.


### **Game Architecture**  
- **Service-Oriented Design**:  
  - Modular APIs for Graphics and Object Pool management.  
  - Easily extensible for adding new features.  

- **Object Pool API**:  
  - Efficient management of game entities and assets.  

### **Utilities**  
- **Dynamic Memory Management**:  
  - Custom allocation for various components.  
  - Reusable memory blocks for animations and textures.  
