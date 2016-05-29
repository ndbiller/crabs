
#include "stdafx.h"
//#include "Level.h"
//#include "Dude.h"
//#include "Creature.h"
#include "Manager.h"

Dude::Dude(Vector2 position)
{
	startPos		= position;
	intPosition		= position;
	movesTo			= position;
	isMoving		= false;
	goal			= NIX;
	orientation		= 0;
	health			= theTuning.GetInt("dudeHealth");
	viewDistance	= theTuning.GetInt("dudeViewDistance");
	SetPosition(position);
	SetDrawShape(ADS_Square);
	LoadSpriteFrames("Resources/Images/Sprites/dude/dude_001.png");
	SetSpriteFrame(0);	// 0 = default frame when no animation is playing (dude_001.png)
	//IdleAnim();
}

void Dude::SetUp()
{
	theSwitchboard.SubscribeTo(this, "MovementDone"); // seems to be redundant (only Manager processes this message)
	Tag("player, in_world");
	Vector2 position = GetPosition();
	Manager::TagTile(position, "occupied, occupied_by_friend");
	Manager::SetTileOccName(position, GetName());

	moveHistory.push_back(intPosition);
}

void Dude::Reset()
{
	MoveTo(startPos, 0.0f, false);
	Manager::TagTile(startPos, "occupied, occupied_by_friend");
	Manager::SetTileOccName(startPos, GetName());
	movesTo			= startPos;
	intPosition		= startPos;
	isMoving		= false;
	orientation		= 0;
	health			= theTuning.GetInt("dudeHealth");
	viewDistance	= theTuning.GetInt("dudeViewDistance");
	IdleAnim();
	Tag("in_world");
	SetLayer("characters");

	moveHistory.erase(moveHistory.begin(), moveHistory.end());
	moveHistory.push_back(intPosition);
}

void Dude::IdleAnim()
{
	float	moveDuration	= theTuning.GetFloat("idleDuration");
	int		firstFrame		= theTuning.GetInt("animDudeIdleFirst") - 1;
	int		lastFrame		= theTuning.GetInt("animDudeIdleLast") - 1;
	float	delay			= moveDuration / (lastFrame - firstFrame + 1);
	PlaySpriteAnimation(delay*3, SAT_Loop, firstFrame, lastFrame, "Idle");
} // end IdleAnim()

void Dude::Move(Vector2 targetPosition, Vector2 position, float cameraOffset, int direction)
{
	// TODO: cameraOffset redundant
	isMoving		= true;
	intPosition		= targetPosition;
	moveHistory.push_back(targetPosition);
	Vector2 camPosition		= theCamera.GetPosition();
	float	moveDuration	= theTuning.GetFloat("moveDuration");
	float	rotation;
	int		targetRotation	= 0;
	bool	smooth			= false;
	if (theTuning.GetInt("smoothMovement"))
		smooth = true;

	if (IsTagged("in_world"))
	{
		if (theTuning.GetInt("rotateCrab"))
		{
			rotation		= GetRotation();
			targetRotation	= ((45 * direction) - 45) * (-1) - 90;
			if (targetRotation - rotation < -180)
				targetRotation = targetRotation + 360;
			RotateTo(float(targetRotation), moveDuration / 4.0f);
		}

		MoveTo(Vector2(targetPosition.X, targetPosition.Y), moveDuration, smooth);
		MoveAnim();
	}
	else
	{
		if (theTuning.GetInt("rotateCrab"))
		{
			SetRotation(float(targetRotation));
		}
		SetPosition(targetPosition);
	}

	movesTo = targetPosition;
	Manager::UntagTile(position, "occupied");
	Manager::UntagTile(position, "occupied_by_friend");
	Manager::SetTileOccName(position, "");
	Manager::TagTile(targetPosition, "occupied, occupied_by_friend");
	Manager::SetTileOccName(targetPosition, GetName());
} // end Dude::Move()

void Dude::MoveAnim()
{
	float	moveDuration	= theTuning.GetFloat("moveDuration");
	int		firstFrame		= theTuning.GetInt("animDudeMoveFirst") - 1;
	int		lastFrame		= theTuning.GetInt("animDudeMoveLast") - 1;
	float	delay			= moveDuration / (float(lastFrame) - float(firstFrame) + 1.0f);
	PlaySpriteAnimation(delay, SAT_OneShot, firstFrame, lastFrame, "Move");
} // end MoveAnim()

void Dude::Fight(Vector2 targetPosition)
{
	moveHistory.push_back(intPosition);

	ActorSet allEnemies = theTagList.GetObjectsTagged("enemy");
	//TODO: get enemy by occupantName
	// cycle through all enemies to find the one located at targetPosition
	for (ActorSet::iterator it = allEnemies.begin(); it != allEnemies.end(); ++it)
	{
		Creature *enemy = (Creature*)*it;
		if (enemy->intPosition == targetPosition)
		{
			isMoving			= true;
			FightAnim();
			enemy->Damage(theTuning.GetInt("dudeDamage"));
		}
	}
} // end Fight()

void Dude::FightAnim()
{
	float	moveDuration	= theTuning.GetFloat("moveDuration");
	int		firstFrame		= theTuning.GetInt("animDudeFightFirst") - 1;
	int		lastFrame		= theTuning.GetInt("animDudeFightLast") - 1;
	float	delay			= moveDuration / (float(lastFrame) - float(firstFrame) + 1);
	PlaySpriteAnimation(delay, SAT_OneShot, firstFrame, lastFrame, "Fight");
} // end FightAnim()

void Dude::Damage(int damage)
{
	health = health - damage;
	if (IsTagged("friend"))
	{
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
				//SetSpriteFrame(31);
			}
		}
	}
	else if (IsTagged("player"))
	{	
		HUD::UpdateHP(health);
		if (health <= 0 && !IsTagged("dead"))
		{
			Tag("dead");
			if (IsTagged("in_world"))
				DeathAnim();
			//else
				//SetSpriteFrame(31);
		}
	}
	if (IsTagged("in_world"))
		HUD::DamageIndicator(intPosition, damage, Color(1.0f, 0.0f, 0.0f));
} // end Damage()

void Dude::DeathAnim()
{
	float	animDuration	= theTuning.GetFloat("deathAnimDuration");
	int		firstFrame		= theTuning.GetInt("animDudeDeathFirst") - 1;
	int		lastFrame		= theTuning.GetInt("animDudeDeathLast") - 1;
	float	delay			= animDuration / (float(lastFrame) - float(firstFrame) + 1);
	PlaySpriteAnimation(delay, SAT_OneShot, firstFrame, lastFrame, "Death");
} // end DeathAnim()

void Dude::ReceiveMessage(Message* message)
{
	String messageName = message->GetMessageName();
}

void Dude::AnimCallback(String animName)
{
	// Gets called when an animation (named animName) finishes
	if (animName == "Fight")
	{
		if (IsTagged("in_world"))
			IdleAnim();
	}
	else if (animName == "Move")
	{
		if (IsTagged("in_world"))
			IdleAnim();
	}
	else if (animName == "Death")
	{
		SetLayer("corpses");
	}
}

void Dude::Render()
{
	if(IsTagged("in_world"))
		Actor::Render();
}