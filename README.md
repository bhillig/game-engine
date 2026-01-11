# 3D Game Engine
A Custom OpenGL 3D game engine capable of loading complex models, rendering multiple scenes, and managing a dynamic set of objects

### Engine Architecture

The Engine is organized into several subsystems:

**Application Layer** (`Core` namespace)
- `Application` - Singleton managing window, layers, asset manager, and main loop
- `Layer` - Abstract base class for application layers (OnUpdate, OnRender, OnEvent)
- `Window` - GLFW window wrapper with event callbacks

**Entity Component System** (`ECS` namespace)
- `EntityManager` - Creates and manages entities, provides component access
- `Entity` - Lightweight handle with component operations (AddComponent, GetComponent, HasComponent, RemoveComponent)
- `EntityMemoryPool` - Data-oriented component storage using tuple of vectors
- `Component` - Base class; built-in types: `TransformComponent`, `MeshComponent`
- Uses C++20 concepts (`IsComponent`) to constrain component types

**Renderer** (`Renderer` namespace and classes)
- `Model` / `Mesh` - 3D model representation with mesh hierarchy
- `ModelLoader` - Assimp-based async model loading with threadsafe queue
- `Shader` - GLSL shader program wrapper
- `Texture` - Texture loading (stb_image) and binding
- OpenGL abstractions: `VertexArray`, `VertexBuffer`, `ElementBuffer`, `VertexBufferLayout`

**Events** (`Core` namespace)
- Type-safe event system with `Event` base class and `EventDispatcher`
- Event types: Mouse, Keyboard, Window (Closed, Resized)
- Uses `EVENT_CLASS_TYPE` macro for RTTI

**Asset Management**
- `AssetManager` - Async model loading via background thread and threadsafe queue

### App Architecture

- `GameSceneLayer` - Main game layer managing scene lifecycle and focus
- `GUILayer` - ImGui-based debug/editor UI
- `Scene` - Contains EntityManager, Camera, CameraController; handles simulation and rendering

## Other Projects:
Take a look at my other projects:

**Sorting Algorithms Visualizer:** https://github.com/bhillig/Sorting-Algorithms

**Neon Blade Impetus:** https://github.com/bhillig/Neon-Blade-Impetus

**CShell:** https://github.com/bhillig/CShell
