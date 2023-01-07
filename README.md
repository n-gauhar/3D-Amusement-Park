# 3D-Amusement-Park
This project was done for my computer graphics lab. Lighting, textures &amp; beizier curves are used in the project.

## Environment

### MacOS
The project was done using the followings:
IDE: Xcode 12.4 (MacOS)
Language: C++ 
Library: OpenGL, GLUT
Image format (for texture): SGI
The project should run as it is on the given environment on Mac.

### Windows
The code should also be able to run on Windows except for the texture mapping code is different for Windows. Change the texture mapping code according and it should run on Windows. To

**Bmploader.h file:** The code for Bmploader.h is different for MacOS. The code for Windows is given in the project and commented out. The lines 250-270 should be uncommented and all other lines should be commented out in Bmploader.h before running in Codeblocks.

**Bmploader.cpp file:** The Bmploader.cpp is not used for running the code in MacOS. The file is included and commented out in the project. The code in Bmploader.cpp needs to be uncommented before running it on Windows.

**Loadtexture() function:** The Loadtexture() function is also different for both the environments. Loadtexture() function for Windows is included and commented out in lines 68-80 in main.cpp. The LoadTexture2() block in lines 82-113 in main.cpp needs to be commented out before running it on Codeblocks as it is the function written for Xcode.

**Image format:** Windows uses BMP format for texture mapping. The respective BMP formats of the textures used are included in the project and commented out in the code in the main() function.

#### The keys used are -  

To move Eye point:
* w: up
* s: down
* a: left
* d: right
* i: zoom in
* o: zoom out
   
To move Camera point:
* j: up
* n: down
* b: left
* m: right
* l: move nearer
* k: move far
* q: move to default position

To control Rides
* 1: Orbiter
* 2: Ferris Wheel
* 3: Sky Drop
* 4: Complex Orbiter
* 5: Pirate Boat

To control lights:
* 6: Spotlight 1
* 7: Spotlight 2
* 8: Spotlight 3
* 9: Spotlight 4

Other controls:
* 0: Day/Night
* Z: To show control points
    
