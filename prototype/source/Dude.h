
class Dude : public Actor
{
public:
	Dude(Vector2 position);
	void SetUp();
	void Reset();

	void Move(Vector2 targetPosition, Vector2 position, float cameraOffset, int direction);
	void Fight(Vector2 targetPosition);
	void Damage(int damage);

	void IdleAnim	();
	void MoveAnim	();
	void FightAnim	();
	void DeathAnim	();

	virtual void ReceiveMessage(Message* message);
	void AnimCallback(String animName);
	virtual void Render();

	bool	isMoving;		// is true after player input (direction or wait) for the length of moveDuration (tuning.lua)
	Vector2 startPos;
	Vector2 intPosition;
	Vector2	movesTo;		//might be redundant -> replace with intPosition
	Vector2 goal;
	int		orientation;
	int		health;
	int		seedMod;

	int		viewDistance;
	
	std::vector<Vector2> playerPath;
	std::vector<Vector2> playerFights;
	std::vector<Vector2> moveHistory;	//TODO: remove this if it doesn'T become a feature

private:
};