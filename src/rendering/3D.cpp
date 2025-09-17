// change name to .hpp once done testing

#include <iostream> //iostream only for testing purposes
#include <cmath>
using namespace std;

#define WIDTH 30    // arbitrary value - change later
#define HEIGHT 30   // arbitrary value - change later
#define FOV (M_PI / 2)  // FOV must be in radians

// the camera is at position (0,0,0) in 3D space. The view is a cone with a diameter of screen width.


// Position of all pixels on screen in 3D space can be found once distance of camera from screen is found (called Focal Length)
// Focal Length = height of cone (ie: adjacent side to FOV)
// therefore, Focal Length = (WIDTH / 2) / (tan [pi / 2])
float find_focalLength(int width, float fov)
{
    float focalLength = (width / 2) / (tan(fov));
    return focalLength;
}


// imagine screen is giant 4 sector graph, (0,0) is centre


// find (relative) vector to any given pixel on screen from camera
// R = (position vector = pixel position) + lambda(direction vector = vector to camera)