#  Engine2D (PGM)
## 2D Engine made from scratch with SDL

An experimental 2D Engine made just for fun inspired, in big part, by the [Handmade Hero](https://https://handmadehero.org/) project.

# How it works
The system it's built in three parts:
* **Engine2D**: This is the core of the system. It's the module capable of handle the platform layer to interact with the user and it renders it's current state. This state will be mutated by the Application layer. This module will be also the one that builds the **executable** for an application.
* **libEngine2D**: This is a layer of services to manipulate the **Engine2D** state, to make things easier. But it's not mandatory. You could just work with the raw EngineState by yourself. It's purpose it's to be statically linked to the application.
* **Application**: This is the layer that it's provided by the consumer of the engine. The application should be compiled as a dynamic library that supply a set of entry points to the engine's core. Then, it will be loaded on start by the executable produced by the **Engine2D** core. This provide us an advantage in development and that's the power of **hot reloading our application code**. Optionally you could link **libEngine2D** into the application to make some things easier.

## Application API
The expected entry points for the application are:
```c++
    // This will be called when the core system armed itself and the dynamic lib is loaded
    void Engine2D_InitializeApplication(Engine2D::EngineState *state);
    // This will be called each frame with the new state of the engine and the user input
    void Engine2D_ApplicationUpdate(real32 deltaTime, real32 totalTime, Engine2D::EngineState *state, Engine2D::UserInput *input);
```
## Hot reload system
If you compile with the HOT_RELOAD=1 preprocessor macro, then the executable will load a copy of the dynamic library and will check in each frame for an updated version to reload.
