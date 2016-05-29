
#include "stdafx.h"

#include "Manager.h"

int main(int argc, char* argv[])
{
	const int tileSize = 16;	// tile width and height in pixels
	int		resolutionWidth;
	int		resolutionHeight;
	int		scalingFactor = 1;	// zoom factor for higher resolutions
	float	cameraX;			// camera offset to the right in world units (due to HUD)
	float	cameraZ;
	int		drawDist;
	bool	fullscreen;

	if (argc > 2)
	{
		resolutionWidth  = StringToInt(argv[1]);
		resolutionHeight = StringToInt(argv[2]);
		if (argc > 3)
			fullscreen =  StringToBool(argv[3]);
		else
			fullscreen = false;
	}
	else	// default window size
	{
		resolutionWidth		= 640;
		resolutionHeight	= 360;
		fullscreen			= false;
	}

	if (resolutionHeight >= 600)
	{
		scalingFactor = 2;
		if (resolutionHeight >= 900)
		{
			scalingFactor = 3;
			if (resolutionHeight >= 1200)
			{	
				scalingFactor = 4;
				if (resolutionHeight >= 1500)
					scalingFactor = 5;
			}
		}
	}

	// arguments: windowWidth, windowHeight, windowName, antialiasing, fullscreen
	theWorld.Initialize(resolutionWidth, resolutionHeight, "rogue", false, fullscreen);

	// set world background to black
	theWorld.SetBackgroundColor(Color(0.0f, 0.0f, 0.0f));
	
	// Register font for the HUD
	RegisterFont("Resources/Fonts/PressStart2P.ttf",  4 * scalingFactor, "FontS");
	RegisterFont("Resources/Fonts/PressStart2P.ttf",  8 * scalingFactor, "FontM");
	RegisterFont("Resources/Fonts/PressStart2P.ttf", 16 * scalingFactor, "FontL");

	int unusedWidth = resolutionWidth - resolutionHeight;

	cameraX = (float(unusedWidth) / float(tileSize)) / (float(scalingFactor) * 2.0f);
	cameraZ = float(resolutionHeight) / float(tileSize * scalingFactor * 2);

	// number of tiles to be drawn in each direction from the player
	drawDist = MathUtil::RoundToInt(cameraZ) + 1;

	theCamera.SetPosition(Vector3(cameraX, 0.0f, cameraZ));
	theCamera.MoveTo(Vector3(cameraX, 0.0f, cameraZ), 0.0f, false);

	// set up and start game manager (Manager.cpp)
	theWorld.SetGameManager(new Manager(cameraX, cameraZ, drawDist, unusedWidth, resolutionHeight, scalingFactor, tileSize));
	
	// OpenGL lighting
/*	glEnable(GL_LIGHTING);
	GLfloat ambient[] = {1.0f, 0.0f, 0.0f, 0.2f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glEnable(GL_LIGHT0);
	GLfloat global_ambient[] = {1.0f, 0.0f, 0.0f, 0.2f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);	
*/

	// do all your setup first, because this function won't return until you're exiting
	theWorld.StartGame();
	
	// any cleanup can go here
	theWorld.Destroy();
	
	return 0;
}
