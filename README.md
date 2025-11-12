# EngineX

EngineX is a C++20 physics engine demo app built for Visual Studio 2019 using SFML for rendering.
Features:
- Real-time 2D physics (rigid bodies, circles, convex polygons)
- Broadphase, narrowphase (SAT), iterative impulse solver
- SFML-based renderer (simulation-style visuals)
- Mouse spawn, pause (Space), exit (Esc)

## Build steps

1. Download SFML 2.6.1 for Visual Studio 2019 (64-bit) from https://www.sfml-dev.org/download.php
2. Extract to `C:\SFML` (so `C:\SFML\include`, `C:\SFML\lib`, `C:\SFML\bin` exist)
3. Open `EngineX.sln` in Visual Studio 2019 (x64)
4. Set `Configuration` → `Debug` or `Release`, then Build (Ctrl+Shift+B)
5. Copy required SFML DLLs from `C:\SFML\bin` to `bin\Debug` or `bin\Release` as needed:
   - Debug: `sfml-graphics-d.dll`, `sfml-window-d.dll`, `sfml-system-d.dll`
   - Release: `sfml-graphics.dll`, `sfml-window.dll`, `sfml-system.dll`
6. Run the app (Ctrl+F5)

## Controls
- Left Click: spawn a circle
- Space: pause / resume
- Esc: exit

## License
MIT (see LICENSE)
