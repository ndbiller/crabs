#include "stdafx.h"
//#include "Creature.h"
//#include "Level.h"
#include "Manager.h"


Creature::Creature(Vector2 position)
{
	startPos	= position;
	intPosition = position;
	SetPosition(position);
	SetDrawShape(ADS_Square);
	LoadSpriteFrames("Resources/Images/Sprites/ant/ant_001.png");
	SetSpriteFrame(0);	// 0 = default frame when no animation is playing (ant_001.png)
	//IdleAnim();
}

void Creature::SetUp()
{
	Tag("enemy");
	health			= theTuning.GetInt("antHealth");
	viewDistance	= theTuning.GetInt("antViewDistance");
	orientation		= 0;
	goal			= NIX;
	lastTargetName	= "";
	
	// tag tile as occupied
	Vector2 position = GetPosition();
	Manager::TagTile(position, "occupied, occupied_by_enemy");
	Manager::SetTileOccName(position, GetName());

	moveHistory.push_back(intPosition);
}

void Creature::Reset()
{
	int		targetRotation	= ((45 * orientation) - 45) * (-1);
	RotateTo(float(targetRotation), 0.0f);
	MoveTo(startPos, 0.0f, false);
	intPosition		= startPos;
	orientation		= 0;
	health			= theTuning.GetInt("antHealth");
	viewDistance	= theTuning.GetInt("antViewDistance");
	goal			= NIX;
	lastTargetName	= "";
	SetLayer("characters");

	Manager::TagTile(startPos, "occupied, occupied_by_enemy");
	Manager::SetTileOccName(startPos, GetName());

	moveHistory.erase(moveHistory.begin(), moveHistory.end());
	moveHistory.push_back(intPosition);
} // end Reset()

void Creature::IdleAnim()
{
	float	moveDuration	= theTuning.GetFloat("idleDuration");
	int		firstFrame		= theTuning.GetInt("animAntIdleFirst") - 1;
	int		lastFrame		= theTuning.GetInt("animAntIdleLast") - 1;
	float	delay			= moveDuration / (lastFrame - firstFrame + 1);
	PlaySpriteAnimation(delay, SAT_Loop, firstFrame, lastFrame, "Idle");
} // end IdleAnim()

void Creature::Move(Vector2 targetPosition, Vector2 position, int direction)
{
	intPosition				= targetPosition;
	moveHistory.push_back(targetPosition);
	float	moveDuration	= theTuning.GetFloat("moveDuration");
	float	rotationDuration= theTuning.GetFloat("rotationDuration");
	float	rotation		= GetRotation();
	int		targetRotation	= ((45 * direction) - 45) * (-1);
	bool	smooth			= false;
	if (theTuning.GetInt("smoothMovement"))
		smooth = true;

	if (targetRotation - rotation < -180)
		targetRotation = targetRotation + 360;

	if (IsTagged("in_world"))
	{
		MoveAnim();
		RotateTo(float(targetRotation), rotationDuration);
		MoveTo(Vector2(targetPosition.X, targetPosition.Y), moveDuration, smooth);
	}
	else
	{	
		SetRotation(float(targetRotation));
		SetPosition(targetPosition);
	}
	Manager::UntagTile(position, "occupied");
	Manager::UntagTile(position, "occupied_by_enemy");
	Manager::SetTileOccName(position, "");
	Manager::TagTile(targetPosition, "occupied, occupied_by_enemy");
	Manager::SetTileOccName(targetPosition, GetName());
} // end Move()

void Creature::MoveAnim()
{
	float	moveDuration	= theTuning.GetFloat("moveDuration");
	int		firstFrame		= theTuning.GetInt("animAntMoveFirst") - 1;
	int		lastFrame		= theTuning.GetInt("animAntMoveLast") - 1;
	float	delay			= moveDuration / (lastFrame - firstFrame + 1);
	PlaySpriteAnimation(delay, SAT_OneShot, firstFrame, lastFrame, "Move");
} // end MoveAnim()

void Creature::Fight(Dude *opponent, int direction)
{
	moveHistory.push_back(intPosition);
	float	rotationDuration= theTuning.GetFloat("rotationDuration");
	float	rotation		= GetRotation();
	int		targetRotation	= ((45 * direction) - 45) * (-1);
	
	if (targetRotation - rotation < -180)
		targetRotation = targetRotation + 360;

	opponent->Damage(theTuning.GetInt("antDamage"));
	FightAnim();
	RotateTo(float(targetRotation), rotationDuration);
} // end Fight()

void Creature::FightAnim()
{
	float	moveDuration	= theTuning.GetFloat("moveDuration");
	int		firstFrame		= theTuning.GetInt("animAntFightFirst") - 1;
	int		lastFrame		= theTuning.GetInt("animAntFightLast") - 1;
	float	delay			= moveDuration / (lastFrame - firstFrame + 1);
	PlaySpriteAnimation(delay, SAT_OneShot, firstFrame, lastFrame, "Fight");
} // end FightAnim()

void Creature::Damage(int damage)
{
	health = health - damage;
	if (health <= 0 && !IsTagged("dead"))
	{
		Tag("dead");
		if (IsTagged("in_world"))
		{
			SetLayer("dying");
			DeathAnim();
		}
		else
		{
			SetLayer("corpses");
		}
	}
	if (IsTagged("in_world"))
		HUD::DamageIndicator(intPosition, damage, Color(1.0f, 1.0f, 0.0f));
}

void Creature::DeathAnim()
{
	float	animDuration	= theTuning.GetFloat("deathAnimDuration");
	int		firstFrame		= theTuning.GetInt("animAntDeathFirst") - 1;
	int		lastFrame		= theTuning.GetInt("animAntDeathLast") - 1;
	float	delay			= animDuration / (float(lastFrame) - float(firstFrame) + 1);
	PlaySpriteAnimation(delay, SAT_OneShot, firstFrame, lastFrame, "Death");
} // end DeathAnim()

void Creature::AnimCallback(String animName)
{
	// Gets called when an animation (named animName) finishes
	if (animName == "Move")
	{
		if (IsTagged("in_world"))
			IdleAnim();
		else
			SetSpriteFrame(0);
	}
	if (animName == "Fight")
	{
		if (IsTagged("in_world"))
			IdleAnim();
		else
			SetSpriteFrame(0);
	}
	else if (animName == "Death")
	{
		SetLayer("corpses");
	}
}

void Creature::Render()
{
	if(IsTagged("in_world"))
		Actor::Render();
}