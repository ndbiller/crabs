
//using namespace std;

// map size in pixels. Must be even!
#define MAP_SIZE_X 128
#define MAP_SIZE_Y 64

// negative vector (used for "empty" vector, because NULL is (0,0))
#define NIX Vector2(-1.0f, -1.0f)

// typedefs
#include <array>
typedef std::array<std::array<char, MAP_SIZE_Y>, MAP_SIZE_X> CharMap;
typedef std::array<std::array<int, MAP_SIZE_Y>, MAP_SIZE_X> IntMap;
typedef std::array<std::array<bool, MAP_SIZE_Y>, MAP_SIZE_X> BoolMap;

// includes
#include <iostream>
#include "Dude.h"
#include "Creature.h"
#include "Level.h"
#include "HUD.h"
#include "Helper.h"

#include "EasyBMP/EasyBMP.h"


class Manager : public GameManager
{
public:
	Manager(float cameraX, float cameraZ, int drawDist, int unusedWidth, int resolutionY, int scalingFactor, int tileSize);
	
	static String	GetTileName		(Vector2 position);
	static void		TagTile			(Vector2 tilePosition, String tag);
	static void		UntagTile		(Vector2 tilePosition, String tag);
	static void		SetTileOccName	(Vector2 tilePosition, String name);
	static Vector2	GetDudePosition ();

	virtual void ReceiveMessage(Message* message);
	virtual void Update(float dt);
	virtual void Render();

	ConsoleLog* con;	// For console output

private:
	Dude*	dude;
	Level*	level;
	HUD*	hud;

	// these depend on window resolution and should generally not be changed
	float		cameraOffset;
	float		cameraViewDist;
	int			drawDistance;
	int			hudWidth;
	int			hudHeight;			// equals height and width of the gameplay area
	int			pixelScale;			// scaling factor for different resolutions. One tile takes up (16 * this)^2 of screen space
	int			tilePixelLength;	// height and width of a single tile in screen pixels

	bool		firstUpdate;
	bool		readyForInput;
	int			nextMove;
	float		animStartTime;
	int			levelSeed;

	Vec2i		mapSize;		// map height and width in pixel
	Vector2		startPos;
	Vector2		levelGoalPos;
	int			turnCount;
	int			crabCount;
	int			turnView;		// development function: turn currently viewed. -1 if not active
	bool		toggleFog;
	bool		toggleMap;

	void		SetUpLevel			();
	void		ResetLevel			();
	void		RestartLevel		();
	void		MoveDude			(int direction);
	void		MoveFriends			();
	void		MoveCreatures		();
	void		MoveCamera			(Vector2 targetPosition);
	int 		BringOutYourDead	();
	int			SeededRand			(int seedModifier);
	Vector2		TargetPosition		(Vector2 position, int direction);
	int			TargetDirection		(Vector2 position, Vector2 targetPosition);
	bool		CanMoveTo			(Vector2 targetPosition);
	bool		IsOccupied			(Vector2 targetPosition);
	bool		IsOccupiedByFriend	(Vector2 targetPosition);
	bool		IsOccupiedByEnemy	(Vector2 targetPosition);
	Actor*		GetOccupant			(Vector2 targetPosition);
	bool		LineOfSight			(Vector2 position, Vector2 targetPosition, int viewDistance);
	int			DirectionTowards	(Vector2 goalPosition, Vector2 position, int orientation);
	int			GetRandomDirection	(Vector2 position, int seedMod = 1, int orientation = 0, 
									 int turnRange = 2, int straightness = 1);
	int			GetRandomEnemy		(Vector2 position, int seedMod = 1, int orientation = 0, 
									 int turnRange = 2, int straightness = 1);
	void		GoToTurn			(int turn);
};
