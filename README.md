# ---Mehgine---

3D Engine made by Pol Carrera and Genís Bayó
 for the 3D engines subject during the 3rd year of Game Design and Game Development degree at CITM.

Project: https://github.com/PolCarCat/3D--Engine/

WebSite: https://polcarcat.github.io/3D--Engine/

Latest build: https://github.com/PolCarCat/3D--Engine/releases/tag/Assignment3.0

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
## Subsystems:
-Game Object Hierarcy: Game Objects can have childs which inheritate their transformations.

-Components: Each Game Object can have certain components such as transform, mesh, particle emitters...

-Own Binary Format: In order to speed up the document loading process, the engine converts .fbx files into its own format.

-Camera management: The camera has an invisible frustum that checks which objects are in the field of view of the camera so that the
engine can avoid drawing unnecessary gemoetry.

-QuadTree: QuadTree checks where static elements are placed and organizes them in a way we can know if they are irrelevant for some calculations, speeding up a lot of iteration processes.

-Scene Serialization: The engine converts the scene parameters into .json files.

-Mouse Picking: Mouse Picking allows the user to select objects in the scene.

-Time Manager: The time manager is used to check how logic would look in-game, and test it by increasing or decreasing the logical time.

-Resource Manager: Keeps track of all the used files and loads and unloads the data from the files. 

-Guizmo: Gizmo can be used to transform 3D elements (their position, scale and rotation).

## Particle System:
The particle system is a part of the engine that allows the creation of 2D elements in a 3D enviroment. This system its divided in two parts:

-Billboarding:
The billboards are plane components that always face the camera, these planes are used when the geometry is too complex and requires too much time to render.

-Particle & Particle Emitter: The particles are elements that manage the behaviour of a billboard, and the particle emitter is the compoenent that spawns them.

-Particle Customization: There is a set of parameters with which users are able to modify the behavior of the particles spawned by the emitter. They can modify the size, color, direction, speed...

----------------------------
## Video
https://www.youtube.com/watch?v=Nee9pyfw8is&t=10s

![gif1](https://github.com/PolCarCat/3D--Engine/blob/master/Game/Webpage/gif1.gif?raw=true)
![gif2](https://github.com/PolCarCat/3D--Engine/blob/master/Game/Webpage/gif2.gif?raw=true)
----------------------------
## Team:
![alt text](https://github.com/PolCarCat/3D--Engine/blob/master/Game/Webpage/Teamfoto.jpeg)

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
