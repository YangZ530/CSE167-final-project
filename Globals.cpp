#include "Globals.h"
#define DIRECTIONAL 1
#define POINT 2
#define SPOT 3

Camera Globals::camera = Camera();

//Cube Globals::cube = Cube(10.0);
//Cube Globals::room = Cube(100.0);

Light Globals::ptLight = Light(POINT);
Light Globals::spotL = Light(SPOT);
Light Globals::dirLight = Light(DIRECTIONAL);

DrawData Globals::drawData = DrawData();
UpdateData Globals::updateData = UpdateData();
