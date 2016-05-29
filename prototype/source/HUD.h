
class HUD : public Actor
{
private:
	int				width;		// width of the HUD area in pixel space
	int				height;		// height of the window (also width of the gameplay area)
	static String	textHP;
	static String	textTurnCount;
	static String	textCrabCount;
	static String	textTurnView;
	Vector2			textHP_ext;
	Vector2			textTurnCount_ext;
	Vector2			textCrabCount_ext;
	Vector2			textTurnView_ext;

	Vector2			textFPS_ext;
	float			time_since_last_drop;
	float			last_drop;
	int				fps_print;
	float			fps;

	HUDActor* mapActor;

public:
	HUD(int hudWidth, int hudHeight);
	void SetUp();
	void WriteMapBMP(CharMap map);
	void DisplayMap(int scale, CharMap map);
	void UpdateMap(Vector2 dudePosition, CharMap map, BoolMap explored);
	void RemoveMap();
	void WinScreen(Vector2 position);

	virtual void ReceiveMessage(Message* message);
	virtual void Update(float dt);
	virtual void Render();

	static void		UpdateHP(int health);
	static void		UpdateTurnCount(int turnCount);
	static void		UpdateCrabCount(int crabCount);
	static void		UpdateTurnView(int crabCount);
	static void		DamageIndicator(Vector2 position, int damage, Color color);
};

//////////////////////////////////////////////////////////////////
// Damage indicator //////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

class DmgInd : public TextActor
{
private:
	float speed;

public:
	DmgInd(Vector2 position, int damage, Color color);

//	virtual void Update(float dt);
//	virtual void Render();
};

//////////////////////////////////////////////////////////////////
// Map tile //////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

class MapTile : public HUDActor
{
private:

public:
	MapTile(char type, Vector2 screenPosition, Vector2 mapPosition, float size);
	void SetUp();

	virtual void ReceiveMessage(Message* message);
};

//////////////////////////////////////////////////////////////////
// Mouse Listener ////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

class Mouse : public MouseListener
{
private:

public:
	Mouse();
	virtual void MouseDownEvent(Vec2i screenCoordinates, MouseButtonInput button);
};
