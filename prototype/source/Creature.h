
class Creature : public Actor
{
public:
	Creature(Vector2 position);
	void SetUp();
	void Reset();

	void Move	(Vector2 targetPosition, Vector2 position, int direction);
	void Fight	(Dude *opponent, int direction);
	void Damage	(int damage);

	void IdleAnim	();
	void MoveAnim	();
	void FightAnim	();
	void DeathAnim	();

	void AnimCallback(String animName);
	virtual void Render();

	Vector2 startPos;
	Vector2	intPosition;	// position in integers (since actor position changes gradually during movement)
	int		orientation;	// direction of last movement (perhaps of last fight..?)
	Vector2	goal;
	String	lastTargetName;	// the name of the crab currently set as goal (if any)
	int		seedMod;

	// species-specific constants/variables
	int		viewDistance;

	std::vector<Vector2> moveHistory;	//TODO: remove this if it doesn'T become a feature

private:
	int		health;

	//virtual void ReceiveMessage(Message* message);
};