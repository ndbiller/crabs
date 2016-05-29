
#include <vector>

class Level : public Actor
{
public:
	Level(int drawDistance);
	
	void SetUp();

	CharMap tilemap;	// each char holds the tile-type at position [x][y]
	IntMap	bitmaskmap; // each int holds the bitmasking case for the tile at [x][y]
	BoolMap	exploredmap;// true for each tile of the level that has been explored (seen) by the player once
	int		bitmasktable[256];

	void CreateLevel(Vec2i size);
	void LoadLevel(String filename);
	void BitMaskLevel(Vec2i size);
	void CreateTileActors(Vec2i size);
	void UnloadLevel();

	void AddVisibleTilesToWorld(Vector2 dudePosition);
	void AddVisibleCreaturesToWorld(Vector2 dudePosition);
	void FadeInTiles(Vector2 dudePosition);
	void UpdateVisibleTiles(Vector2 dudePosition, int direction);
	void UpdateVisibleFriends(Vector2 dudePosition);
	void UpdateVisibleEnemies(Vector2 dudePosition);
	void RemoveAllTiles();

	void Fog(Vector2 dudeNewPos);
	void UnFog(Vector2 position);

		//TODO: RemoveAllTiles

	virtual void ReceiveMessage(Message* message);

private:
	void GenFillBlock(int x, int y, int size, char type);
	int  GenGetNeighbours(int x, int y, int distance, char type, Vec2i mapSize);
	bool GenRandomFill(int nFilledNeighbours);
	bool GenTreeTrunk(Vec2i position, Vec2i mapSize);

	int BitMaskTile(int x, int y, char one, char zero);

	void FogLine(int x0, int y0, int x1, int y1);

	int drawDist;
	int viewDist;	// player view distance in tiles [SET BY FUNCTION!]

	void	SetUpBitmasktable();
};

class Tile : public Actor
{
public:
	Tile(char type, Vector2 position);
	void SetUp();

	void AnimCallback(String animName);
	virtual void ReceiveMessage(Message* message);
	virtual void Update(float dt);
	virtual void Render();

	bool	visible;		// tracks if tile was visible once during Fog()
	int		fadeInAt;		// tile starts to fade in on game start at this tick-count (-1 while not in use)
	String occupantName;
private:
};