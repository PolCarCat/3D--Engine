﻿# ---Mehgine---

3D Engine made by Pol Carrera and Genís Bayó
 for the 3D engines subject during the 3rd year of Game Design and Game Development degree at CITM.

Project: https://github.com/PolCarCat/3D--Engine/

WebSite: https://polcarcat.github.io/3D--Engine/

Pol Carrera: https://github.com/PolCarCat/

Genís Bayó: https://github.com/retsnom9/

----------------------------
## Instructions:

	-File:
		·Save:		Saves configuration.
		·Load:		Loads configuration.
		·Quit:		Quits application. Also works with Esc, Alt+F4 or clicking X.

	-Windows:
		·Configuration:	Framerate, window and renderer management. Hardware and memory management information.
				& Quadtree
		·Console:	Information about loading objects, textures... and errors.
		·Element:	Information about Selected Game Object.
		·Objects:	List of loaded objects.
		·Materials:	Edit the selected Material
		·Time Manager:	Enters in game mode.
		·Test Window:	Random Number Generator. Show/Hide primitives.

	-Help:
		·Go to our Wiki.
		·Latest release.
		·Report a but.
		·About:		Information about version, team, license and libraries used.

	-Controls:
		·WASD:		Move around (Hold SHIFT to double speed).
		·Mouse wheel:	Zoom in/out.
		·F:		Focus camera on geometry.
		·Right click:	Orbitate camera (Hold Alt to orbitate around object).
		·LCNTRL + S : 	Save Scene
		·LCNTRL + L :	Load Scene
		·LCNTRL + X :	Clear Scene
		·LCNTRL + D :	Duplicate Selected Game Object
		·DELETE	:	Delete Selected Game Object
		·1:		Create firework

----------------------------
## Particle System:
The particle system is a part of the engine that allows the creation of 2D elements in a 3D enviroment. This system its divided in two parts:

-Billboarding:
The billboards are components that are a plane that is always facing the camera, this planes are used when the geometry is too complex and requires too much time to render.

-Particle & Particle Emitter: The particles are elements that manages the behaviour of a billboard, and the particle emitter is the one that spawns the particles.

----------------------------
## Tasks:

### Billboarding
	-Face Camera: Genís 
	-Save & Load: Pol
### Particle
	-Management: Pol
	-Billboard in particle: Genís
### Particle Emitter
	-Management: Pol
	-Save & Load: Pol
### Firework
	-Management: Genís

----------------------------
## MIT License

Copyright (c) 2018 Genís Bayó, Pol Carrera

Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction, including without
limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit
persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission
notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NON
INFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
