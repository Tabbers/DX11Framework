#pragma once

const bool VSYNC = true;
const float DEG_TO_RAD = 0.0174532925F;
const int SHADOWMAP_WIDTH = 2048;
const int SHADOWMAP_HEIGHT = 2048;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const float SCREEN_DEPTHL = 5000.0f;
const float SCREEN_NEARL = 0.1f;
const float PI = 3.1415926535898f;

enum Modes 
{ 
	CAMERA, 
	LIGHT, 
};