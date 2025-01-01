# **Megaman Game Prototype**  

This project is a 2D game prototype inspired by *Megaman*, developed in C with OpenGL. It includes a robust architecture with dynamic systems for animation, movement, input handling, and more.  

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
