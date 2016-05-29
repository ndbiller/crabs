
#include "stdafx.h"
#include "Manager.h"

//TODO: How to get rid of actors? / general clean up
//			-> get rid of pointers? (free memory)
//		Developer log or TextActor with variable output
//		for text map (zombie movement): p = ground, f = wall, door = obstacle
//		todo list in dropbox
//		optimize #includes
//		Message for CreatureMovementDone ? (player can move before creature movement is done..)
//		restrict framerate to reduce processor load! (e.g. 60fps)
//		some letters missing from text when using font size 8...

Manager::Manager(float cameraX, float cameraZ, int drawDist, int unusedWidth, int resolutionY, int scalingFactor, int tileSize)
{
	// Declare private member variables
	cameraOffset	= cameraX;
	cameraViewDist	= cameraZ;
	drawDistance	= drawDist;
	hudWidth		= unusedWidth;
	hudHeight		= resolutionY;
	pixelScale		= scalingFactor;
	tilePixelLength = tileSize * scalingFactor;

	turnCount		= 0;
	crabCount		= 1;
	turnView		= -1;
	toggleFog		= true;
	toggleMap		= true;
	firstUpdate		= true;
	readyForInput	= true;
	nextMove		= -1;
	animStartTime	= 0.0f;

	mapSize.X		= MAP_SIZE_X;
	mapSize.Y		= MAP_SIZE_Y;

	// Generate output to the in-game console (#)
	con = new ConsoleLog();
	con->Printf("cameraOffset: %f, cameraViewDist: %f, drawDistance: %i, hudWidth: %i, hudHeight: %i, pixelScale: %i, tilePixelLength: %i",
		cameraOffset, cameraViewDist, drawDistance, hudWidth, hudHeight, pixelScale, tilePixelLength);

	// Subscribe to messages
	theSwitchboard.SubscribeTo(this, "Wait");
	theSwitchboard.SubscribeTo(this, "MoveN");
	theSwitchboard.SubscribeTo(this, "MoveNE");
	theSwitchboard.SubscribeTo(this, "MoveE");
	theSwitchboard.SubscribeTo(this, "MoveSE");
	theSwitchboard.SubscribeTo(this, "MoveS");
	theSwitchboard.SubscribeTo(this, "MoveSW");
	theSwitchboard.SubscribeTo(this, "MoveW");
	theSwitchboard.SubscribeTo(this, "MoveNW");
	theSwitchboard.SubscribeTo(this, "MovementDone");
	theSwitchboard.SubscribeTo(this, "ReadyForInput");
	theSwitchboard.SubscribeTo(this, "ToggleMap");
	theSwitchboard.SubscribeTo(this, "Seppuku");
	theSwitchboard.SubscribeTo(this, "devReloadLevel");
	theSwitchboard.SubscribeTo(this, "TurnBack");
	theSwitchboard.SubscribeTo(this, "TurnForward");
	theSwitchboard.SubscribeTo(this, "Death");
	theSwitchboard.SubscribeTo(this, "Quit");
	theSwitchboard.SubscribeTo(this, "MouseDownLeft");
	theSwitchboard.SubscribeTo(this, "MouseDownMiddle");
	theSwitchboard.SubscribeTo(this, "MouseDownRight");

	theSwitchboard.SubscribeTo(this, "PlayTime");
	theSwitchboard.SubscribeTo(this, "MadeIt");
	// development functions
	theSwitchboard.SubscribeTo(this, "ToggleFog");

	// Name layers for Actor placement
	theWorld.NameLayer("background", -1);
	theWorld.NameLayer("tiles", 0);			// the surface of the world
	theWorld.NameLayer("ground", 1);		// for stuff lying on the ground (items etc.)
	theWorld.NameLayer("corpses", 2);		// dead characters
	theWorld.NameLayer("dying", 2);			// dying characters (between death and end of death animation)
	theWorld.NameLayer("characters", 3);	// all living characters (player, enemies, NPCs)
	theWorld.NameLayer("hud_bg", 4);		// background of the HUD
	theWorld.NameLayer("hud_items", 5);		// text and images in the HUD

	// create MouseListener (HUD.cpp)
	new Mouse();

	// set up level
	SetUpLevel();

	//start game after tiles faded in etc.
	theSwitchboard.DeferredBroadcast(new Message("PlayTime"), 2 * theTuning.GetFloat("tileFadeInDuration") + float(theTuning.GetInt("tileFadeInDelayMaxMS"))/1000.0f);
}

//////// Manager functions //////////////////////////////////////////////////////////////////////////////
void Manager::SetUpLevel()
{
	turnCount = 0;

	// seed rand (TODO: make it so that one seed creates the same level, creature positions, etc. reliably)
	levelSeed = 1;

	// create level
	level = new Level(drawDistance);
	level->SetUp();
	
	// load/create level
	if (theTuning.GetInt("loadLevel"))
		level->LoadLevel("Resources/Maps/test_map.txt");
	else
	{
		level->CreateLevel(mapSize);
		level->BitMaskLevel(mapSize);
		level->CreateTileActors(mapSize);
	}

	// create heads-up-display
	hud = new HUD(hudWidth, hudHeight);
	hud->SetUp();
	hud->WriteMapBMP(level->tilemap);

	//Temporary: add ants to the world
	if (true)
	{
		for (int i = 0; i <= 100; i++)
		{
			Vector2 pos;
			pos.X = rand() % (mapSize.X - 5) + 5;
			pos.Y = rand() % mapSize.Y;
			StringList targetTileName (2);
			targetTileName[0] = FloatToString(pos.X);
			targetTileName[1] = FloatToString(pos.Y);
			Tile *targetTile = (Tile*)Actor::GetNamed(JoinString(targetTileName, "_"));
			if(targetTile)
			{
				if (targetTile->IsTagged("occupied") == false && targetTile->IsTagged("walkable"))
				{
					Creature *ant = new Creature(pos);
					//Creature *zombie = new Creature(Vector2(1.0f,1.0f));
					//Creature *zombie = (Creature*)Actor::Create("zombie");	
					//Exception after Destroy() when using archetype...
					theWorld.Add(ant, "characters");
					ant->SetUp();
					ant->seedMod = i + 1;
				}
			}
		}
	}

	//This part copied over from RestartLevel() /////////////////////////////////////////////////

	// create protagonist
	startPos		= Vector2(2,mapSize.Y/2 + 2*crabCount);
	//TODO: implement level goal ?
	levelGoalPos	= Vector2(mapSize.X-2, rand()%(mapSize.Y-4)+2);
	dude			= new Dude(startPos);

	// update hud variables
	HUD::UpdateHP(dude->health);
	HUD::UpdateTurnCount(turnCount);
	HUD::UpdateCrabCount(crabCount);
	HUD::UpdateTurnView(turnView);

	// set goal for ai friend
	dude->goal = Vector2(mapSize.X+1,mapSize.Y/2+rand()%21-10);

	// reseed rng
	srand(levelSeed);

	// start tile fade-in
	level->FadeInTiles(dude->intPosition);

	// add tiles in view to the world
	level->AddVisibleTilesToWorld(dude->intPosition);

		//TODO: temporary. fix this...
	// Move dude once, otherwise nothing gets drawn. PROBLEM(TODO:) Move animation runs once at the beginning...
	//theCamera.SetPosition(startPos.X + cameraOffset, startPos.Y);
	MoveCamera(startPos);
	dude->Move(startPos, startPos, cameraOffset, 0);
	dude->moveHistory.pop_back();
	dude->isMoving = false;

	// this is so no movement input is taken until "PlayTime"
	dude->isMoving = true;
} // end SetUpLevel()

void Manager::ResetLevel()
{
	turnCount = 0;

	// set all tiles as unoccupied (just in case something slipped through..)
/*	ActorSet allTiles = theTagList.GetObjectsTagged("tile");
	for (ActorSet::iterator it = allTiles.begin(); it != allTiles.end(); ++it)
	{
		Tile *tile = (Tile*)*it;
		Manager::UntagTile(position, "occupied");
		Manager::UntagTile(position, "occupied_by_friend");
		Manager::UntagTile(position, "occupied_by_enemy");
		Manager::SetTileOccName(position, "");
	}
*/
	// reset enemies
	ActorSet allEnemies = theTagList.GetObjectsTagged("enemy");
	for (ActorSet::iterator it = allEnemies.begin(); it != allEnemies.end(); ++it)
	{
		Creature *enemy		= (Creature*)*it;

		if (!enemy->IsTagged("dead") && !enemy->IsTagged("limbo"))
		{
			Vector2 position	= enemy->intPosition;
			Manager::UntagTile(position, "occupied");
			Manager::UntagTile(position, "occupied_by_enemy");
			Manager::SetTileOccName(position, "");
		}
		else
		{
			enemy->Untag("dead");
			enemy->Untag("limbo");
		}

		enemy->Reset();
	}

	// set up all friends (previous incarnations of the player)
	ActorSet allFriends = theTagList.GetObjectsTagged("friend");
	for (ActorSet::iterator it = allFriends.begin(); it != allFriends.end(); ++it)
	{
		Dude *crab		= (Dude*)*it;

		if (!crab->IsTagged("dead") && !crab->IsTagged("limbo"))
		{
			Vector2 position	= crab->intPosition;
			Manager::UntagTile(position, "occupied");
			Manager::UntagTile(position, "occupied_by_friend");
			Manager::SetTileOccName(position, "");
		}
		else
		{
			crab->Untag("dead");
			crab->Untag("limbo");
		}

		crab->Reset();
	}

	// remove all tiles
	level->RemoveAllTiles();

	// create protagonist
	startPos	= Vector2(2,mapSize.Y/2 + 2*crabCount);
	dude		= new Dude(startPos);

	// add protagonist to the world and run setup
	theWorld.Add(dude, "characters");
	dude->SetUp();

	// update HUD
	HUD::UpdateHP(dude->health);
	HUD::UpdateTurnCount(turnCount);
	HUD::UpdateCrabCount(crabCount);
	HUD::UpdateTurnView(turnView);

	// set goal for ai friend
	dude->goal = Vector2(mapSize.X+1,mapSize.Y/2+rand()%21-10);	

	// reseed rng
	srand(levelSeed);

	// add tiles in view to the world
	level->AddVisibleTilesToWorld(dude->intPosition);
	if (toggleFog)		
		level->Fog(dude->intPosition);

	// add creatures in view to the world
	level->AddVisibleCreaturesToWorld(dude->intPosition);

	//TODO: temporary. fix this...
	// Move dude once, otherwise nothing gets drawn. PROBLEM(TODO:) Move animation runs once at the beginning...
	//theCamera.SetPosition(startPos.X + cameraOffset, startPos.Y);
	MoveCamera(startPos);
	dude->Move(startPos, startPos, cameraOffset, 0);
	dude->moveHistory.pop_back();
	dude->isMoving = false;

	// update map
	hud->UpdateMap(dude->intPosition, level->tilemap, level->exploredmap);
} // end ResetLevel()

void Manager::RestartLevel()
{
	turnCount = 0;
	crabCount = 1;

	// set all tiles as unoccupied (just in case something slipped through..)
	ActorSet allTiles = theTagList.GetObjectsTagged("tile");
	for (ActorSet::iterator it = allTiles.begin(); it != allTiles.end(); ++it)
	{
		Tile *tile = (Tile*)*it;
		Vector2 position = tile->GetPosition();
		Manager::UntagTile(position, "occupied");
		Manager::UntagTile(position, "occupied_by_friend");
		Manager::UntagTile(position, "occupied_by_enemy");
		Manager::SetTileOccName(position, "");
	}

	// reset enemies
	ActorSet allEnemies = theTagList.GetObjectsTagged("enemy");
	for (ActorSet::iterator it = allEnemies.begin(); it != allEnemies.end(); ++it)
	{
		Creature *enemy		= (Creature*)*it;

		if (!enemy->IsTagged("dead") && !enemy->IsTagged("limbo"))
		{
			Vector2 position	= enemy->intPosition;
			Manager::UntagTile(position, "occupied");
			Manager::UntagTile(position, "occupied_by_enemy");
			Manager::SetTileOccName(position, "");
		}
		else
		{
			enemy->Untag("dead");
			enemy->Untag("limbo");
		}

		enemy->Reset();
	}

	// remove all friends (previous incarnations of the player)
	ActorSet allFriends = theTagList.GetObjectsTagged("friend");
	for (ActorSet::iterator it = allFriends.begin(); it != allFriends.end(); ++it)
	{
		Dude *crab		= (Dude*)*it;
		crab->Destroy();
	}

	// remove all tiles
	level->RemoveAllTiles();

	// reset dude
	startPos = Vector2(2,mapSize.Y/2 + 2*crabCount);
	dude->startPos = startPos;
	dude->Reset();

	// update HUD
	HUD::UpdateHP(dude->health);
	HUD::UpdateTurnCount(turnCount);
	HUD::UpdateCrabCount(crabCount);
	HUD::UpdateTurnView(turnView);

	// reseed rng
	srand(levelSeed);

	// add tiles in view to the world
	level->AddVisibleTilesToWorld(dude->intPosition);
	if (toggleFog)		
		level->Fog(dude->intPosition);

	// add creatures in view to the world
	level->AddVisibleCreaturesToWorld(dude->intPosition);

	//TODO: temporary. fix this...
	// Move dude once, otherwise nothing gets drawn. PROBLEM(TODO:) Move animation runs once at the beginning...
	//theCamera.SetPosition(startPos.X + cameraOffset, startPos.Y);
	MoveCamera(startPos);
	dude->Move(startPos, startPos, cameraOffset, 0);
	dude->moveHistory.pop_back();
	dude->isMoving = false;

	// update map
	hud->UpdateMap(dude->intPosition, level->tilemap, level->exploredmap);
} // end RestartLevel()

void Manager::MoveDude(int direction)
{
	Vector2 position		= dude->intPosition;
	Vector2 targetPosition	= TargetPosition(position, direction);
	// MOVE
	if (CanMoveTo(targetPosition))
	{
		turnCount++;
		HUD::UpdateTurnCount(turnCount);
		dude->Move(targetPosition, position, cameraOffset, direction);
		dude->playerPath.push_back(targetPosition);
		dude->playerFights.push_back(NIX);
		dude->orientation = direction;
		MoveFriends();
		MoveCreatures();
		MoveCamera(targetPosition);
		BringOutYourDead();
		level->UpdateVisibleTiles(position, direction);
		level->UpdateVisibleFriends(position);
		level->UpdateVisibleEnemies(position);
		if (toggleFog)
			level->Fog(targetPosition);
		if (toggleMap)
			hud->UpdateMap(targetPosition, level->tilemap, level->exploredmap);
	}
	// FIGHT
	else if (IsOccupiedByEnemy(targetPosition) && direction)
	{
		turnCount++;
		HUD::UpdateTurnCount(turnCount);
		dude->Fight(targetPosition);
		dude->playerPath.push_back(position);	// could also use targetPosition here...
		dude->playerFights.push_back(targetPosition);
		dude->orientation = direction;
		MoveFriends();
		MoveCreatures();
		BringOutYourDead();
		level->UpdateVisibleFriends(position);
		level->UpdateVisibleEnemies(position);
		if (toggleFog)
			level->Fog(position);
		if (toggleMap)
			hud->UpdateMap(position, level->tilemap, level->exploredmap);
	}
	// WAIT
	else if (direction == 0)
	{
		turnCount++;
		HUD::UpdateTurnCount(turnCount);
		dude->isMoving = true;	// this is for input tracking
		dude->moveHistory.push_back(position);
		dude->playerPath.push_back(position);
		dude->playerFights.push_back(NIX);
		MoveFriends();
		MoveCreatures();
		BringOutYourDead();
		level->UpdateVisibleFriends(position);
		level->UpdateVisibleEnemies(position);
		if (toggleFog)
			level->Fog(position);
		if (toggleMap)
			hud->UpdateMap(position, level->tilemap, level->exploredmap);
	}
	// check if level goal has been reached
	if (dude->intPosition.X == mapSize.X-1)	
	{
		theSwitchboard.DeferredBroadcast(new Message("MadeIt"), theTuning.GetFloat("moveDuration"));
	}
	else
	{
		// suspend taking input for half of moveDuration and suspend movement for moveDuration
		float moveDuration = theTuning.GetFloat("moveDuration");
		theSwitchboard.DeferredBroadcast(new Message("ReadyForInput"), moveDuration/2);
		theSwitchboard.DeferredBroadcast(new Message("MovementDone"), moveDuration);
	}
} // end MoveDude()

void Manager::MoveFriends()
{
	ActorSet allFriends = theTagList.GetObjectsTagged("friend");
	for (ActorSet::iterator it = allFriends.begin(); it != allFriends.end(); ++it)
	{
		Dude* crab				= (Dude*)*it;
		Vector2 position		= crab->intPosition;
		Vector2 targetPosition	= NIX;
		int		direction		= 0;

		// if the crab is alive
		if (!crab->IsTagged("limbo"))
		{
			// if there is a record in the player path for the current turn
			if (turnCount > 0 && turnCount <= crab->playerPath.size())
			{
				// if the next step in the player path is within reach
				if (TargetDirection(position, crab->playerPath[turnCount-1]) >= 0)
				{
					// if the next step requires a move
					if (crab->playerPath[turnCount-1] != position)
					{
						// if the tile is free or occupied by an enemy, set that as target position
						if (CanMoveTo(crab->playerPath[turnCount-1]) || IsOccupiedByEnemy(crab->playerPath[turnCount-1]))
							targetPosition	= crab->playerPath[turnCount-1];
						else
						{
							// look for enemies within reach biased towards the direction of the player fight and fight one of them
							if (targetPosition == NIX)
							{
								for (int turnRange = 1; turnRange <= 2; turnRange++) 
								{
									// Get random direction, biased towards the next step
									direction = GetRandomDirection(position, crab->seedMod, TargetDirection(position, crab->playerPath[turnCount-1]), turnRange, 3);
									if (direction == 0)
									{
										direction = GetRandomEnemy(position, crab->seedMod, TargetDirection(position, crab->playerFights[turnCount-1]), turnRange, 3);
									}
									if (direction > 0)
									{	targetPosition = TargetPosition(position, direction);
										break;	}
								}
							}
						}
					}
					// if the player fought in this turn
					else if (crab->playerFights[turnCount-1] != NIX)
					{
						// if there is an enemy on the same position, set that as target
						if (IsOccupiedByEnemy(crab->playerFights[turnCount-1]))
							targetPosition = crab->playerFights[turnCount-1];
					}
					// look for enemies within reach biased towards the direction of the player fight and fight one of them
					if (targetPosition == NIX)
					{
						for (int turnRange = 1; turnRange <= 4; turnRange++) //what happens when turnRange==4?
						{
							// Get random direction, biased towards the direction of the fight
							direction = GetRandomEnemy(position, crab->seedMod, TargetDirection(position, crab->playerFights[turnCount-1]), turnRange, 3);
							if (direction > 0)
							{	targetPosition = TargetPosition(position, direction);
								break;	}
						}
					}
					// if the player waited or no enemy was found, wait
					if (targetPosition == NIX)
						targetPosition = position;
				} 
				// if the next step in the player path is not within reach
				else
				{
					// set goal towards next step // TODO: Pack into function and use better algorithm
					direction = DirectionTowards(crab->playerPath[turnCount-1], position, crab->orientation);
					if (direction)
					{
						Vector2 positionTowards = TargetPosition(position, direction);
						if (!CanMoveTo(positionTowards) && !IsOccupiedByEnemy(positionTowards))
						{
							for (int turnRange = 1; turnRange <= 4; turnRange++) //what happens when turnRange==4?
							{
								// Get random direction, biased towards the next step
								direction = GetRandomDirection(position, crab->seedMod, TargetDirection(position, crab->playerPath[turnCount-1]), turnRange, 3);
								if (direction > 0)
								{	targetPosition = TargetPosition(position, direction);
									break;	}
							}
						}
						else
							targetPosition = positionTowards;
					}
				}
			}
			// if there is no record left in the player path
			else
			{
				// A.I. takes over here
				// set goal towards level edge/goal // TODO: Pack into function and use better algorithm
				direction = DirectionTowards(crab->goal, position, crab->orientation);
				if (direction)
				{
					Vector2 positionTowards = TargetPosition(position, direction);
					// if the crab can't move there and no enemy is there, get new direction
					if (!CanMoveTo(positionTowards) && !IsOccupiedByEnemy(positionTowards))
					{
						for (int turnRange = 1; turnRange <= 4; turnRange++) //what happens when turnRange==4?
						{
							// Get random direction, biased towards the next step
							direction = GetRandomDirection(position, crab->seedMod, TargetDirection(position, crab->goal), turnRange, 3);
							if (direction > 0)
							{	targetPosition = TargetPosition(position, direction);
								break;	}
						}
					}
					else
						targetPosition = positionTowards;
				}
			}
			
			if (targetPosition == NIX)
				targetPosition = position;

			if (targetPosition != position && targetPosition != NIX)
			{
				// move or fight
				if (CanMoveTo(targetPosition))
				{
					direction = TargetDirection(position, targetPosition);
					crab->Move(targetPosition, position, cameraOffset, direction);
					crab->orientation = direction;
				}
				else if (IsOccupiedByEnemy(targetPosition))
				{
					direction = TargetDirection(position, targetPosition);
					crab->Fight(targetPosition);
					crab->orientation = direction;
				}
			}
			else
			{
				crab->moveHistory.push_back(position);
			}

		} // end if(!crab->IsTagged("limbo"))
	}
} // end: MoveFriends()

void Manager::MoveCreatures()
{
	ActorSet allEnemies = theTagList.GetObjectsTagged("enemy");
	for (ActorSet::iterator it = allEnemies.begin(); it != allEnemies.end(); ++it)
	{
		Creature *enemy		= (Creature*)*it;
		if (!enemy->IsTagged("dead") && !enemy->IsTagged("limbo")) // enemy->IsDestroyed() == false && 
		{
			Vector2 position		= enemy->intPosition;
			Vector2 targetPosition	= NIX;
			int		direction		= 0;

			int minDistance			= 1000000;
			std::vector<String>		nearestCrabName;	// list of crabs in range with minimal distance from creature
			std::vector<Vector2>	line;

			// check if dude is in line of sight
			if (LineOfSight(dude->movesTo, position, enemy->viewDistance))
			{
				line = Helper::BresenhamLine(dude->movesTo.X, dude->movesTo.Y, position.X, position.Y);
				minDistance			= (int)line.size();
				nearestCrabName.push_back(dude->GetName());
			}
			
			// check all other crabs for line of sight and if any, select the closest one
			ActorSet allFriends = theTagList.GetObjectsTagged("friend");
			for (ActorSet::iterator it = allFriends.begin(); it != allFriends.end(); ++it)
			{
				Dude *crab = (Dude*)*it;
				if (!crab->IsTagged("limbo"))
				{
					if (LineOfSight(crab->movesTo, position, enemy->viewDistance))
					{
						line = Helper::BresenhamLine(crab->movesTo.X, crab->movesTo.Y, position.X, position.Y);
						// if theres no crab nearer than or as near as this one, empty nearest crab list and add this
						if ((int)line.size() < minDistance)
						{
							minDistance		= (int)line.size();					// update distance of closest crab
							nearestCrabName.erase(nearestCrabName.begin(), nearestCrabName.end());
							nearestCrabName.push_back(crab->GetName());
						}
						// if this is as near as crab(s) already on the list, add this one to them
						else if ((int)line.size() == minDistance)
						{
							nearestCrabName.push_back(crab->GetName());
						}
					}
				}
			}
			
			// if there is any crab in line of sight, set the position of a random one of them as goal
			if (!nearestCrabName.empty())
			{
				Dude *nearest;
				std::vector<String>::iterator result =  std::find(nearestCrabName.begin(), nearestCrabName.end(), enemy->lastTargetName);
				if (result == nearestCrabName.end())
					nearest = (Dude*)Actor::GetNamed(nearestCrabName[SeededRand(enemy->seedMod)%nearestCrabName.size()]);	//TODO: de-randomize
				else
					nearest = (Dude*)Actor::GetNamed(*result);
				enemy->goal				= nearest->movesTo;
				enemy->lastTargetName	= nearest->GetName();
			}

			// TODO:	- check if target crab (goal) is alive (otherwise the ant hangs around trying to reach the "dead goal"
			//			- maybe check in GetRandomDirection for other crabs (tag for friendly occupied vs. enemy occupied!)

			// if goal is reached, clear goal
			if (position == enemy->goal)
			{	enemy->goal = NIX;	}

			// if there's a goal, get a target position in direction of the goal
			if (enemy->goal != NIX)
			{	direction = DirectionTowards(enemy->goal, position, enemy->orientation);
				if (direction)
				{
					targetPosition = TargetPosition(position, direction);
					if (!CanMoveTo(targetPosition) && !IsOccupiedByFriend(targetPosition))
					{
						for (int turnRange = 1; turnRange <= 4; turnRange++) //what happens when turnRange==4?
						{
							// Get random direction, biased towards the orientation (last turn's direction)
							direction = GetRandomDirection(position, enemy->seedMod, enemy->orientation, turnRange, 3);
							if (direction > 0)
							{	targetPosition = TargetPosition(position, direction);
								break;	}
						}
					}	
				}
			}
			// if there's no goal, get a random target position biased towards current orientation
			else
			{
				direction = GetRandomDirection(position, enemy->seedMod, enemy->orientation, 2, 3);
				if (direction)
					targetPosition = TargetPosition(position, direction);
			}

			if (targetPosition == NIX)
			{
				if (direction == 0)
					direction = GetRandomDirection(position, enemy->seedMod);	// completely random direction (1-8)

				targetPosition		= TargetPosition(position, direction);
			}

			// move or fight
			if (CanMoveTo(targetPosition))
			{	enemy->Move(targetPosition, position, direction);
				enemy->orientation	= direction;	}
			else if (IsOccupiedByFriend(targetPosition))
			{	
				Dude* target = (Dude*)GetOccupant(targetPosition);
				if (target)
				{	enemy->Fight(target, direction);
					enemy->orientation	= direction;	}
				else
					con->Printf("Couldn't find occupant.");
			}
		}
	} // end for (ActorSet::iterator it = allEnemies.begin(); it != allEnemies.end(); ++it)
} // end MoveCreatures()

void Manager::MoveCamera(Vector2 targetPosition)
{
	bool	smooth			= false;
	if (theTuning.GetInt("smoothMovement"))
		smooth = true;
	if (targetPosition.X < cameraViewDist - 0.5)
		targetPosition.X = cameraViewDist - 0.5;
	else if (targetPosition.X > mapSize.X - cameraViewDist - 0.5)
		targetPosition.X = mapSize.X - cameraViewDist - 0.5;
	if (targetPosition.Y < cameraViewDist - 0.5)
		targetPosition.Y = cameraViewDist - 0.5;
	else if (targetPosition.Y > mapSize.Y - cameraViewDist - 0.5)
		targetPosition.Y = mapSize.Y - cameraViewDist - 0.5;
	float moveDuration = theTuning.GetFloat("moveDuration");
	theCamera.Actor::MoveTo(Vector2(targetPosition.X + cameraOffset, targetPosition.Y), moveDuration, smooth);	
} // end MoveCamera()

int Manager::BringOutYourDead()
{
	int playerAlive = 1;
	// handles dead enemies friends and the player at the end of a turn.
	// returns 0 if the player is dead
	ActorSet markedDead = theTagList.GetObjectsTagged("dead");
	for (ActorSet::iterator it = markedDead.begin(); it != markedDead.end(); ++it)
	{
		Actor *deadGuy = *it;

		if (deadGuy->IsTagged("enemy"))
		{
			Creature *creature = (Creature*)*it;
			Vector2 position = creature->intPosition;
			Manager::UntagTile(position, "occupied");
			Manager::UntagTile(position, "occupied_by_enemy");
			Manager::SetTileOccName(position, "");
			
			creature->Tag("limbo");
			creature->Untag("dead");
			//deadGuy->Destroy();
		}
		else if (deadGuy->IsTagged("friend"))
		{
			Dude *crab = (Dude*)*it;
			Vector2 position = crab->intPosition;
			Manager::UntagTile(position, "occupied");
			Manager::UntagTile(position, "occupied_by_friend");
			Manager::SetTileOccName(position, "");

			crab->Tag("limbo");
			crab->Untag("dead");
		}
		else if (deadGuy->IsTagged("player"))
		{
			Dude *player = (Dude*)*it;
			Vector2 position = player->intPosition;
			if (toggleFog)
				level->UnFog(position);
			Manager::UntagTile(position, "occupied");
			Manager::UntagTile(position, "occupied_by_friend");
			Manager::SetTileOccName(position, "");
			player->Untag("player");
			player->Tag("friend");
			theSwitchboard.DeferredBroadcast(new Message("Death"), theTuning.GetFloat("deathAnimDuration"));
			player->isMoving = true;
			readyForInput = false;
			playerAlive = 0;
			player->seedMod = crabCount;
		}
	}
	return (playerAlive);
} // end BringOutYourDead()

int Manager::SeededRand(int seedModifier)
{
	srand(levelSeed + turnCount + seedModifier);
	return (rand());
} // end SeededRand()

String Manager::GetTileName(Vector2 position)
{
	StringList targetTileName (2);
	targetTileName[0] = FloatToString(position.X);
	targetTileName[1] = FloatToString(position.Y);
	return JoinString(targetTileName, "_");
} // end GetTileName()

Vector2 Manager::TargetPosition(Vector2 position, int direction)
{
	Vector2 target;
	if (direction == 0)			// Wait
		target = position;
	else if (direction == 1)	// North
	{	target.X = (position.X);
		target.Y = (position.Y + 1.0f);	}
	else if (direction == 2)	// North-East
	{	target.X = (position.X + 1.0f);
		target.Y = (position.Y + 1.0f);	}
	else if (direction == 3)	// East
	{	target.X = (position.X + 1.0f);
		target.Y = (position.Y);		}
	else if (direction == 4)	// South-East
	{	target.X = (position.X + 1.0f);
		target.Y = (position.Y - 1.0f);	}
	else if (direction == 5)	// South
	{	target.X = (position.X);
		target.Y = (position.Y - 1.0f);	}
	else if (direction == 6)	// South-West
	{	target.X = (position.X - 1.0f);
		target.Y = (position.Y - 1.0f);	}
	else if (direction == 7)	// West
	{	target.X = (position.X - 1.0f);
		target.Y = (position.Y);		}
	else if (direction == 8)	// North-West
	{	target.X = (position.X - 1.0f);
		target.Y = (position.Y + 1.0f);	}
	else
	{	con->Printf("TargetPosition(): argument direction = %i not valid", direction);
		return NIX;
	}
	return target;
} // end TargetPosition()

int Manager::TargetDirection(Vector2 position, Vector2 targetPosition)
{
	int direction = -1;	// negative value means target position is not within reach
	if (targetPosition.X == position.X - 1)				// WEST
	{
		if (targetPosition.Y == position.Y - 1)			// South-West
			direction = 6;
		else if (targetPosition.Y == position.Y)		// West
			direction = 7;
		else if (targetPosition.Y == position.Y + 1)	// North-West
			direction = 8;
	}
	else if (targetPosition.X == position.X)
	{
		if (targetPosition.Y == position.Y - 1)			// South
			direction = 5;
		else if (targetPosition.Y == position.Y)		// wait
			direction = 0;
		else if (targetPosition.Y == position.Y + 1)	// North
			direction = 1;
	}
	else if (targetPosition.X == position.X + 1)		// EAST
	{
		if (targetPosition.Y == position.Y - 1)			// South-East
			direction = 4;
		else if (targetPosition.Y == position.Y)			// East
			direction = 3;
		else if (targetPosition.Y == position.Y + 1)	// North-East
			direction = 2;
	}
	return direction;
} // end TargetDirection()

bool Manager::CanMoveTo(Vector2 targetPosition)
{
	Tile *targetTile = (Tile*)Actor::GetNamed(GetTileName(targetPosition));

	if(targetTile)
	{	
		if (targetTile->IsTagged("walkable") && !targetTile->IsTagged("occupied"))
		{	return true;	}
	}
	return false;
} // end CanMoveTo()

bool Manager::IsOccupied(Vector2 targetPosition)
{
	Tile *targetTile = (Tile*)Actor::GetNamed(GetTileName(targetPosition));
	
	if(targetTile)
	{	if (targetTile->IsTagged("occupied"))
			return true;
	}
	return false;
} // end IsOccupied()

bool Manager::IsOccupiedByFriend(Vector2 targetPosition)
{
	Tile *targetTile = (Tile*)Actor::GetNamed(GetTileName(targetPosition));
	
	if(targetTile)
	{	if (targetTile->IsTagged("occupied_by_friend"))
			return true;
	}
	return false;
} // end IsOccupiedByFriend()

bool Manager::IsOccupiedByEnemy(Vector2 targetPosition)
{
	Tile *targetTile = (Tile*)Actor::GetNamed(GetTileName(targetPosition));
	
	if(targetTile)
	{	if (targetTile->IsTagged("occupied_by_enemy"))
			return true;
	}
	return false;
} // end IsOccupiedByEnemy()

Actor* Manager::GetOccupant(Vector2 targetPosition)
{
	Tile *targetTile = (Tile*)Actor::GetNamed(GetTileName(targetPosition));
	
	if(targetTile)
	{
		return Actor::GetNamed(targetTile->occupantName);
	}
	return NULL;
} // end GetOccupant()

bool Manager::LineOfSight(Vector2 position, Vector2 targetPosition, int viewDistance)
{
	StringList tileName (2);
	std::vector<Vector2> line = Helper::BresenhamLine(position.X, position.Y, targetPosition.X, targetPosition.Y);
	int tileCountIndex = 0;
	
	if (int(line.size()) > viewDistance + 1)
	{	return false;	}

	if (line[0] != position)
	{	tileCountIndex = line.size() - 1;	}
	while (true)
	{
		tileName[0] = FloatToString(line[tileCountIndex].X);
		tileName[1] = FloatToString(line[tileCountIndex].Y);
		Tile* tile = (Tile*)Actor::GetNamed(JoinString(tileName, "_"));
		if (tile)
		{
			if (tile->IsTagged("opaque"))
			{	return false;	}
		}
		if (line[0] != position)
		{
			tileCountIndex--;
			if (tileCountIndex < 0)	break;
		}
		else
		{
			tileCountIndex++;
			if (tileCountIndex >= int(line.size()))	break;
		}
	}
	return true;
} // end LineOfSight()

int Manager::DirectionTowards(Vector2 goalPosition, Vector2 position, int orientation)
{
	//TODO: invisible at position (0,0) ...
	if (goalPosition != NIX && position != NIX)
	{	if (goalPosition == position)
			return NULL;	}
	else
	{	con->Printf("DirectionTowards called with NIX argument. goalPosition=(%f,%f), position=(%f,%f)",
					goalPosition.X, goalPosition.Y, position.X, position.Y);
		return NULL;
	}
	int dX		 = int(goalPosition.X - position.X);
	int dY		 = int(goalPosition.Y - position.Y);
	
	if (dX > 0)			// East
	{	if (dY > 0)					// North-East
		{	if (dX > dY)								// ENE
			{	if (!CanMoveTo(TargetPosition(position, 2)) || orientation == 6)
				{	if (!CanMoveTo(TargetPosition(position, 3)) || orientation == 7)
					{	if (!CanMoveTo(TargetPosition(position, 1)) || orientation == 5)
						{	return 4;	}
						return 1;	}
					return 3;	}
				return 2;
			}
			else if (dX < dY)							// NNE
			{	if (!CanMoveTo(TargetPosition(position, 2)) || orientation == 6)
				{	if (!CanMoveTo(TargetPosition(position, 1)) || orientation == 5)
					{	if (!CanMoveTo(TargetPosition(position, 3)) || orientation == 7)
						{	return 8;	}
						return 3;	}
					return 1;	}
				return 2;
			}
			else if (dX > 1)							// NE
			{	if (!CanMoveTo(TargetPosition(position, 2)) || orientation == 6)
				{	if (!CanMoveTo(TargetPosition(position, 1)) || orientation == 5)
					{	return 3;	}
					return 1;	}
				return 2;
			}
			return 2;									// NE, one step away from goal
		}
		else if (dY < 0)			// South-East
		{	if (dX > MathUtil::Abs(dY))					// ESE
			{	if (!CanMoveTo(TargetPosition(position, 4)) || orientation == 8)
				{	if (!CanMoveTo(TargetPosition(position, 3)) || orientation == 7)
					{	if (!CanMoveTo(TargetPosition(position, 5)) || orientation == 1)
						{	return 2;	}
						return 5;	}
					return 3;	}
				return 4;
			}
			else if (dX < MathUtil::Abs(dY))			// SSE 
			{	if (!CanMoveTo(TargetPosition(position, 4)) || orientation == 8)
				{	if (!CanMoveTo(TargetPosition(position, 5)) || orientation == 1)
					{	if (!CanMoveTo(TargetPosition(position, 3)) || orientation == 7)
						{	return 6;	}
						return 3;	}
					return 5;	}
				return 4;
			}
			else if (dX > 1)							// SE
			{	if (!CanMoveTo(TargetPosition(position, 4)) || orientation == 8)
				{	if (!CanMoveTo(TargetPosition(position, 3)) || orientation == 7)
					{	return 5;	}
					return 3;	}
				return 4;
			}
			return 4;									// SE, one step away from goal
		}
		else if (dX > 1)								// E
		{	if (!CanMoveTo(TargetPosition(position, 3)) || orientation == 7)
			{	if (!CanMoveTo(TargetPosition(position, 2)) || orientation == 6)
				{	return 4;	}
				return 2;	}
			return 3;
		}
		return 3;										// E, one step away from goal
	}
	else if (dX < 0)	// West
	{	if (dY > 0)					// North-West
		{	if (MathUtil::Abs(dX) > dY)					// WNW
			{	if (!CanMoveTo(TargetPosition(position, 8)) || orientation == 4)
				{	if (!CanMoveTo(TargetPosition(position, 7)) || orientation == 3)
					{	if (!CanMoveTo(TargetPosition(position, 1)) || orientation == 5)
						{	return 6;	}
						return 1;	}
					return 7;	}
				return 8;
			}
			else if (MathUtil::Abs(dX) < dY)			// NNW
			{	if (!CanMoveTo(TargetPosition(position, 8)) || orientation == 4)
				{	if (!CanMoveTo(TargetPosition(position, 1)) || orientation == 5)
					{	if (!CanMoveTo(TargetPosition(position, 7)) || orientation == 3)
						{	return 2;	}
						return 7;	}
					return 1;	}
				return 8;
			}
			else if (dX > 1)							// NW
			{	if (!CanMoveTo(TargetPosition(position, 8)) || orientation == 4)
				{	if (!CanMoveTo(TargetPosition(position, 7)) || orientation == 3)
					{	return 1;	}
					return 7;	}
				return 8;
			}
			return 8;									// NW, one step away from goal
		}
		else if (dY < 0)			// South-West
		{	if (MathUtil::Abs(dX) > MathUtil::Abs(dY))	// WSW
			{	if (!CanMoveTo(TargetPosition(position, 6)) || orientation == 2)
				{	if (!CanMoveTo(TargetPosition(position, 7)) || orientation == 3)
					{	if (!CanMoveTo(TargetPosition(position, 5)) || orientation == 1)
						{	return 8;	}
						return 5;	}
					return 7;	}
				return 6;
			}
			else if (MathUtil::Abs(dX) < MathUtil::Abs(dY)) // SSW
			{	if (!CanMoveTo(TargetPosition(position, 6)) || orientation == 2)
				{	if (!CanMoveTo(TargetPosition(position, 5)) || orientation == 1)
					{	if (!CanMoveTo(TargetPosition(position, 7)) || orientation == 3)
						{	return 4;	}
						return 7;	}
					return 5;	}
				return 6;
			}
			else if (dX > 1)							// SW
			{	if (!CanMoveTo(TargetPosition(position, 6)) || orientation == 2)
				{	if (!CanMoveTo(TargetPosition(position, 5)) || orientation == 1)
					{	return 7;	}
					return 5;	}
				return 6;
			}
			return 6;									// SW, one step away from goal
		}
		else if (dX > 1)				// West
		{	if (!CanMoveTo(TargetPosition(position, 7)) || orientation == 3)
			{	if (!CanMoveTo(TargetPosition(position, 6)) || orientation == 2)
				{	return 8;	}
				return 6;	}
			return 7;
		}
		return 7;						// West, one step away from goal
	}
	else	// dX == 0
	{	if (dY > 1)			// North
		{	if(!CanMoveTo(TargetPosition(position, 1)) || orientation == 5)
			{	if (!CanMoveTo(TargetPosition(position, 2)) || orientation == 6)
				{	return 8;	}
				return 2;	}
			return 1;	}
		else if (dY == 1)	// Nort, one step away from goal
		{	return 1;	}
		else if (dY < -1)	// South
		{	if (!CanMoveTo(TargetPosition(position, 5)) || orientation == 1)
			{	if (!CanMoveTo(TargetPosition(position, 6)) || orientation == 2)
				{	return 4;	}
				return 6;	}
			return 5;	}
		else if (dY == -1)	// South, one step away from goal
		{	return 5;	}
	}
	return NULL;	// something went wrong...
} // end DirectionTowards()

int Manager::GetRandomDirection(Vector2 position, int seedMod, int orientation, int turnRange, int straightness)
{
	// Returns randomly one of all directions free for movement [or occupied by the dude - removed]
	// If orientation is passed, possible directions consist of orientation +- turnRange (CW/CCW).
	// straightness increases the probability for stright movement (direction = orientation).
	int canmoveto[32];	// array for possible directions (can contain multiples for bias)
	int index = 0;		// index for canmoveto[]

	if (orientation)
	{	// loop through all directions orientation +- turnRange and add to array if free
		for (int dir = -turnRange; dir <= +turnRange; dir++)	
		{
			int direction = orientation + dir;
			if (direction < 1)		// correct directions out of bound (1-8)
				direction = direction + 8;
			if (direction > 8)
				direction = direction - 8;
			Vector2 tilePosition = TargetPosition(position, direction);
			if (CanMoveTo(tilePosition)) // || dude->movesTo == tilePosition)
			{	
				int repeat = 2^(turnRange - MathUtil::Abs(dir));
				if (direction == orientation)
					repeat = repeat + straightness*turnRange;
				for (int p = 1; p <= repeat; p++)
				{	canmoveto[index] = direction;	// add the direction to the array of free directions
					index++;				}
			}
		}
	} // end if (orientation)
	else
	{	// loop through all 8 possible directions and add them to the array if free
		for (int dir = 1; dir <= 8; dir++)	
		{
			Vector2 tilePosition = TargetPosition(position, dir);
			if (CanMoveTo(tilePosition)) // || dude->movesTo == tilePosition)
			{	canmoveto[index] = dir;	// add the direction to the array of free directions
				index++;				}
		}
	}
	if (index)
	{	// select one of the free directions randomly and return it
		return (canmoveto[SeededRand(seedMod)%index]);
	}
	else
		return (0);
} // end GetRandomDirection()

int Manager::GetRandomEnemy(Vector2 position, int seedMod, int orientation, int turnRange, int straightness)
{
	// Returns randomly one of all directions toward a tile occupied by an enemy
	// If orientation is passed, possible directions consist of orientation +- turnRange (CW/CCW).
	// straightness increases the probability for stright movement (direction = orientation).
	int canmoveto[32];	// array for possible directions (can contain multiples for bias)
	int index = 0;		// index for canmoveto[]

	if (orientation)
	{	// loop through all directions orientation +- turnRange and add to array if free
		for (int dir = -turnRange; dir <= +turnRange; dir++)	
		{
			int direction = orientation + dir;
			if (direction < 1)		// correct directions out of bound (1-8)
				direction = direction + 8;
			if (direction > 8)
				direction = direction - 8;
			Vector2 tilePosition = TargetPosition(position, direction);
			if (IsOccupiedByEnemy(tilePosition))
			{	
				int repeat = 2^(turnRange - MathUtil::Abs(dir));
				if (direction == orientation)
					repeat = repeat + straightness*turnRange;
				for (int p = 1; p <= repeat; p++)
				{	canmoveto[index] = direction;	// add the direction to the array of free directions
					index++;				}
			}
		}
	} // end if (orientation)
	else
	{	// loop through all 8 possible directions and add them to the array if free
		for (int dir = 1; dir <= 8; dir++)	
		{
			Vector2 tilePosition = TargetPosition(position, dir);
			if (IsOccupiedByEnemy(tilePosition))
			{	canmoveto[index] = dir;	// add the direction to the array of free directions
				index++;				}
		}
	}
	if (index)
	{	// select one of the free directions randomly and return it
		return (canmoveto[SeededRand(seedMod)%index]);
	}
	else
		return (0);
} // end GetRandomEnemy()

void Manager::GoToTurn(int turn)
{
	float	moveDuration	= theTuning.GetFloat("moveDuration");

	if (turn >= 0 && turn < dude->moveHistory.size())
	{
		Vector2 targetPosition	= dude->moveHistory[turn];
		dude->MoveTo(targetPosition, moveDuration, true);
		MoveCamera(targetPosition);
	}

	ActorSet allFriends = theTagList.GetObjectsTagged("friend");
	for (ActorSet::iterator it = allFriends.begin(); it != allFriends.end(); ++it)
	{
		Dude* crab				= (Dude*)*it;
		Vector2 position		= crab->intPosition;

		if (turn >= 0 && turn < crab->moveHistory.size())
		{
			Vector2 targetPosition	= crab->moveHistory[turn];
			crab->MoveTo(targetPosition, moveDuration, true);
		}
	}
	ActorSet allEnemies = theTagList.GetObjectsTagged("enemy");
	for (ActorSet::iterator it = allEnemies.begin(); it != allEnemies.end(); ++it)
	{
		Creature* enemy			= (Creature*)*it;
		Vector2 position		= enemy->intPosition;

		if (turn >= 0 && turn < enemy->moveHistory.size())
		{
			Vector2 targetPosition	= enemy->moveHistory[turn];
			enemy->MoveTo(targetPosition, moveDuration, true);
		}
	}
} // end GoToTurn()

void Manager::TagTile(Vector2 tilePosition, String tag)
{
	Tile *tile = (Tile*)Actor::GetNamed(GetTileName(tilePosition));
	tile->Tag(tag);
}

void Manager::UntagTile(Vector2 tilePosition, String tag)
{
	Tile *tile = (Tile*)Actor::GetNamed(GetTileName(tilePosition));
	tile->Untag(tag);
}

void Manager::SetTileOccName(Vector2 tilePosition, String name)
{
	Tile *tile = (Tile*)Actor::GetNamed(GetTileName(tilePosition));
	tile->occupantName = name;
}

void Manager::ReceiveMessage(Message* message)
{
	String messageName = message->GetMessageName();
	if		(messageName == "Wait")
	{	
		if (dude->isMoving == FALSE)
		{	MoveDude(0);
			readyForInput = false;
			nextMove = -1;	}
		else if (readyForInput)
			nextMove = 0;
	}
	else if (messageName == "MoveN")
	{	
		if (dude->isMoving == FALSE)
		{	MoveDude(1);
			readyForInput = false;
			nextMove = -1;	}
		else if (readyForInput)
			nextMove = 1;
	}
	else if (messageName == "MoveNE")
	{	
		if (dude->isMoving == FALSE)
		{	MoveDude(2);
			readyForInput = false;
			nextMove = -1;	}
		else if (readyForInput)
			nextMove = 2;
	}
	else if (messageName == "MoveE")
	{	
		if (dude->isMoving == FALSE)
		{	MoveDude(3);
			readyForInput = false;
			nextMove = -1;	}
		else if (readyForInput)
			nextMove = 3;
	}
	else if (messageName == "MoveSE")
	{	
		if (dude->isMoving == FALSE)
		{	MoveDude(4);
			readyForInput = false;
			nextMove = -1;	}
		else if (readyForInput)
			nextMove = 4;
	}
	else if (messageName == "MoveS")
	{	
		if (dude->isMoving == FALSE)
		{	MoveDude(5);
			readyForInput = false;
			nextMove = -1;	}
		else if (readyForInput)
			nextMove = 5;
	}
	else if (messageName == "MoveSW")
	{	
		if (dude->isMoving == FALSE)
		{	MoveDude(6);
			readyForInput = false;
			nextMove = -1;	}
		else if (readyForInput)
			nextMove = 6;
	}
	else if (messageName == "MoveW")
	{	
		if (dude->isMoving == FALSE)
		{	MoveDude(7);
			readyForInput = false;
			nextMove = -1;	}
		else if (readyForInput)
			nextMove = 7;
	}
	else if (messageName == "MoveNW")
	{	
		if (dude->isMoving == FALSE)
		{	MoveDude(8);
			readyForInput = false;
			nextMove = -1;	}
		else if (readyForInput)
			nextMove = 8;
	}
	else if (messageName == "MovementDone")
	{
		if (!dude->IsTagged("dead"))
		{
			dude->isMoving = false;
			if (nextMove >= 0)
			{	MoveDude(nextMove);
				readyForInput = false;
				nextMove = -1;
			}
		}
	}
	else if (messageName == "ReadyForInput")
		readyForInput = true;
	else if (messageName == "ToggleMap")
	{
		if (toggleMap)
		{	hud->RemoveMap();
			toggleMap = false;	}
		else
		{	hud->DisplayMap(pixelScale, level->tilemap);
			toggleMap = true;	}
	}
	else if (messageName == "Death")
	{
		dude->isMoving = false;
		readyForInput = true;
		crabCount++;
		ResetLevel();
	}
	else if (messageName == "Quit")
	{
//		level->UnloadLevel();
//		level->Destroy();
//		theWorld.UnloadAll();
		theWorld.StopGame();
	}
	else if (messageName == "MouseDownLeft")
	{
		// development function: teleport
		//TODO: remove
		if (toggleMap)
		{
			TypedMessage<Vec2i> *m = (TypedMessage<Vec2i>*)message;
			Vec2i pos = m->GetValue();
			pos.X = ceil(pos.X/pixelScale);
			pos.Y = mapSize.Y - ceil(pos.Y/pixelScale);
			if (pos.X <= mapSize.X && pos.X > 0 && pos.Y <= mapSize.Y && pos.Y > 0)
			{
				if (CanMoveTo(pos))
				{
					level->RemoveAllTiles();
					dude->Move(pos, dude->intPosition, cameraOffset, 1);
					dude->isMoving = false;
					MoveCamera(pos);
					level->AddVisibleTilesToWorld(pos);
					level->AddVisibleCreaturesToWorld(pos);
				}
			}
		}
	}
	else if (messageName == "MouseDownMiddle")
	{}
	else if (messageName == "MouseDownRight")
	{}
	// development functions
	else if (messageName == "ToggleFog")
	{	if (toggleFog == true)	
		{	toggleFog = false;
			level->UnFog(dude->intPosition);
		}
		else
		{	toggleFog = true;
			level->Fog(dude->intPosition);
		}
	}
	else if (messageName == "Seppuku")
	{
		if (dude->isMoving == FALSE)
		{
			dude->Damage(9001);
			BringOutYourDead();
		}
	}
	else if (messageName == "devReloadLevel")
	{
		RestartLevel();
	}
	else if (messageName == "TurnBack")
	{
		if (turnView > 0)
		{
			dude->isMoving = true;
			turnView--;
			GoToTurn(turnView);
		}
		else if (turnView == -1 && turnCount > 0)
		{
			dude->isMoving = true;
			turnView = turnCount - 1;
			GoToTurn(turnView);
		}
		HUD::UpdateTurnView(turnView);
	}
	else if (messageName == "TurnForward")
	{
		if (turnView >= 0 && turnView < turnCount)
		{
			dude->isMoving = true;
			turnView++;
			GoToTurn(turnView);

			if (turnView == turnCount)
			{
				dude->isMoving = false;
				turnView = -1;
			}
		}
		HUD::UpdateTurnView(turnView);
	}
	else if (messageName == "PlayTime")
	{
		theWorld.Add(dude, "characters");
		dude->SetUp();
		dude->isMoving = false;
		level->AddVisibleCreaturesToWorld(dude->intPosition);
		hud->DisplayMap(pixelScale, level->tilemap);	
		//hud->UpdateMap(dude->intPosition, level->tilemap, level->exploredmap);
		theWorld.Add(hud, "hud_items");
		if (toggleFog)		
			level->Fog(dude->intPosition);
	}
	else if (messageName == "MadeIt")
	{
		Vector2 position = theCamera.GetPosition();
		hud->WinScreen(Vector2(position.X, position.Y-1.0f));
		dude->isMoving = true;
	}
}

void Manager::Update(float dt)
{
	if (firstUpdate)
	{
		animStartTime = 0;
		theSwitchboard.Broadcast(new Message("StartTileAnimation"));
		firstUpdate = false;
	}
	
	animStartTime += dt;
	if (animStartTime > theTuning.GetFloat("tileAnimDuration"))
	{
		animStartTime = 0;
		theSwitchboard.Broadcast(new Message("StartTileAnimation"));
	}
} // end Update()

void Manager::Render()
{

} // end Render()