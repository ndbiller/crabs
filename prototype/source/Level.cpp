
#include "stdafx.h"
#include "Manager.h"

Level::Level(int drawDistance)
{
	drawDist = drawDistance;
}

void Level::SetUp()
{
	theSwitchboard.SubscribeTo(this, "devReloadLevel");

	viewDist = drawDist + 1;		// player view distance in tiles
	// fill tilemap with zeros
	for (int x = 0; x < tilemap.size(); x++)
	{	
		tilemap[x].fill('p');	
		bitmaskmap[x].fill(0);	
		exploredmap[x].fill(false);
	}
	SetUpBitmasktable();
}

void Level::SetUpBitmasktable()
{
	bitmasktable[0]		= 39;
	bitmasktable[1]		= 44;
	bitmasktable[2]		= 36;
	bitmasktable[3]		= 43;
	bitmasktable[4]		= 37;
	bitmasktable[5]		= 14;
	bitmasktable[6]		= 35;
	bitmasktable[7]		= 32;
	bitmasktable[8]		= 45;
	bitmasktable[9]		= 34;
	bitmasktable[10]	= 15;
	bitmasktable[11]	= 40;
	bitmasktable[12]	= 42;
	bitmasktable[13]	= 41;
	bitmasktable[14]	= 33;
	bitmasktable[15]	= 38;

	bitmasktable[16]	= 28;
	bitmasktable[17]	= 28;
	bitmasktable[18]	= 27;
	bitmasktable[19]	= 27;
	bitmasktable[20]	= 26;
	bitmasktable[21]	= 26;
	bitmasktable[22]	= 23;
	bitmasktable[23]	= 23;
	bitmasktable[24]	= 28;
	bitmasktable[25]	= 28;
	bitmasktable[26]	= 27;
	bitmasktable[27]	= 27;
	bitmasktable[28]	= 26;
	bitmasktable[29]	= 26;
	bitmasktable[30]	= 23;
	bitmasktable[31]	= 23;

	bitmasktable[32]	= 21;
	bitmasktable[33]	= 21;
	bitmasktable[34]	= 21;
	bitmasktable[35]	= 21;
	bitmasktable[36]	= 24;
	bitmasktable[37]	= 24;
	bitmasktable[38]	= 24;
	bitmasktable[39]	= 24;
	bitmasktable[40]	= 16;
	bitmasktable[41]	= 16;
	bitmasktable[42]	= 16;
	bitmasktable[43]	= 16;
	bitmasktable[44]	= 30;
	bitmasktable[45]	= 30;
	bitmasktable[46]	= 30;
	bitmasktable[47]	= 30;

	bitmasktable[48]	= 1;
	bitmasktable[49]	= 1;
	bitmasktable[50]	= 1;
	bitmasktable[51]	= 1;
	bitmasktable[52]	= 3;
	bitmasktable[53]	= 3;
	bitmasktable[54]	= 3;
	bitmasktable[55]	= 3;
	bitmasktable[56]	= 1;
	bitmasktable[57]	= 1;
	bitmasktable[58]	= 1;
	bitmasktable[59]	= 1;
	bitmasktable[60]	= 3;
	bitmasktable[61]	= 3;
	bitmasktable[62]	= 3;
	bitmasktable[63]	= 3;

	bitmasktable[64]	= 29;
	bitmasktable[65]	= 19;
	bitmasktable[66]	= 29;
	bitmasktable[67]	= 19;
	bitmasktable[68]	= 29;
	bitmasktable[69]	= 19;
	bitmasktable[70]	= 29;
	bitmasktable[71]	= 19;
	bitmasktable[72]	= 18;
	bitmasktable[73]	= 22;
	bitmasktable[74]	= 18;
	bitmasktable[75]	= 22;
	bitmasktable[76]	= 18;
	bitmasktable[77]	= 22;
	bitmasktable[78]	= 18;
	bitmasktable[79]	= 22;

	bitmasktable[80]	= 7;
	bitmasktable[81]	= 7;
	bitmasktable[82]	= 7;
	bitmasktable[83]	= 7;
	bitmasktable[84]	= 7;
	bitmasktable[85]	= 7;
	bitmasktable[86]	= 7;
	bitmasktable[87]	= 7;
	bitmasktable[88]	= 7;
	bitmasktable[89]	= 7;
	bitmasktable[90]	= 7;
	bitmasktable[91]	= 7;
	bitmasktable[92]	= 7;
	bitmasktable[93]	= 7;
	bitmasktable[94]	= 7;
	bitmasktable[95]	= 7;

	bitmasktable[96]	= 9;
	bitmasktable[97]	= 9;
	bitmasktable[98]	= 9;
	bitmasktable[99]	= 9;
	bitmasktable[100]	= 9;
	bitmasktable[101]	= 9;
	bitmasktable[102]	= 9;
	bitmasktable[103]	= 9;
	bitmasktable[104]	= 11;
	bitmasktable[105]	= 11;
	bitmasktable[106]	= 11;
	bitmasktable[107]	= 11;
	bitmasktable[108]	= 11;
	bitmasktable[109]	= 11;
	bitmasktable[110]	= 11;
	bitmasktable[111]	= 11;

	for (int i = 112; i < 128; i++)
		bitmasktable[i] = 13;

	bitmasktable[128]	= 20;
	bitmasktable[129]	= 17;
	bitmasktable[130]	= 25;
	bitmasktable[131]	= 31;
	bitmasktable[132]	= 20;
	bitmasktable[133]	= 17;
	bitmasktable[134]	= 25;
	bitmasktable[135]	= 31;
	bitmasktable[136]	= 20;
	bitmasktable[137]	= 17;
	bitmasktable[138]	= 25;
	bitmasktable[139]	= 31;
	bitmasktable[140]	= 20;
	bitmasktable[141]	= 17;
	bitmasktable[142]	= 25;
	bitmasktable[143]	= 31;

	bitmasktable[144]	= 0;
	bitmasktable[145]	= 0;
	bitmasktable[146]	= 2;
	bitmasktable[147]	= 2;
	bitmasktable[148]	= 0;
	bitmasktable[149]	= 0;
	bitmasktable[150]	= 2;
	bitmasktable[151]	= 2;
	bitmasktable[152]	= 0;
	bitmasktable[153]	= 0;
	bitmasktable[154]	= 2;
	bitmasktable[155]	= 2;
	bitmasktable[156]	= 0;
	bitmasktable[157]	= 0;
	bitmasktable[158]	= 2;
	bitmasktable[159]	= 2;

	for (int i = 160; i < 176; i++)
		bitmasktable[i] = 6;

	for (int i = 176; i < 192; i++)
		bitmasktable[i] = 5;

	bitmasktable[192]	= 8;
	bitmasktable[193]	= 10;
	bitmasktable[194]	= 8;
	bitmasktable[195]	= 10;
	bitmasktable[196]	= 8;
	bitmasktable[197]	= 10;
	bitmasktable[198]	= 8;
	bitmasktable[199]	= 10;
	bitmasktable[200]	= 8;
	bitmasktable[201]	= 10;
	bitmasktable[202]	= 8;
	bitmasktable[203]	= 10;
	bitmasktable[204]	= 8;
	bitmasktable[205]	= 10;
	bitmasktable[206]	= 8;
	bitmasktable[207]	= 10;

	for (int i = 208; i < 224; i++)
		bitmasktable[i] = 12;

	for (int i = 224; i < 240; i++)
		bitmasktable[i] = 4;

	for (int i = 240; i < 256; i++)
		bitmasktable[i] = 46;

} // end SetUpBitmasktable()

void Level::CreateLevel(Vec2i size)
{
	for (int x = 10; x < size.X - 10; x += 12)
	{
		for (int y = 10; y < size.Y - 10; y += 10)
		{
			int treeX = x + rand() % 7 - 3;
			int treeY = y + rand() % 7 - 3;

			GenTreeTrunk(Vec2i(treeX, treeY), size);
		}
	}

	// OLD ISLAND GENERATION PRECEDURE ///////////////////////////////////////////////////////////////////////////////////

	//int x, y  = 0;	// indices for single tiles
	//int xn,yn = 0;	// indices for neighbour tiles
	//int xs,ys = 0;	// indices for 16x16 blocks of tiles
	//int sixteenth	= size/16;
	//int nFilledNeighbours = 0;
	//
	//// fill center
	//int center_start = sixteenth*5;
	//int center_end   = sixteenth*11;
	//GenFillBlock(sixteenth*5, sixteenth*5, 6*sixteenth, 'p');
	//
	//// spiral around center, filling 16x16 blocks of tiles randomly, depending on number of filled neighbour blocks
	//int step = size/16;
	//for (int round = 1; round <= 2; round++)
	//{
	//	int rStep = (round-1)*step;
	//	ys = center_start - step - rStep;
	//	for (xs = center_start - rStep; xs <= center_end + rStep; xs += step)
	//	{
	//		nFilledNeighbours = GenGetNeighbours(xs, ys, step, 'p', size);
	//		if (GenRandomFill(nFilledNeighbours))
	//			GenFillBlock(xs, ys, step, 'p');
	//	}
	//
	//	xs = center_end + rStep;
	//	for (ys = center_start - rStep; ys <= center_end + rStep; ys += step)
	//	{
	//		nFilledNeighbours = GenGetNeighbours(xs, ys, step, 'p', size);
	//		if (GenRandomFill(nFilledNeighbours))
	//			GenFillBlock(xs, ys, step, 'p');
	//	}
	//
	//	ys = center_end + rStep;
	//	for (xs = center_end - step + rStep; xs >= center_start - step - rStep; xs -= step)
	//	{
	//		nFilledNeighbours = GenGetNeighbours(xs, ys, step, 'p', size);
	//		if (GenRandomFill(nFilledNeighbours))
	//			GenFillBlock(xs, ys, step, 'p');
	//	}
	//
	//	xs = center_start - step - rStep;
	//	for (ys = center_end - step + rStep; ys >= center_start - step - rStep; ys -= step)
	//	{
	//		nFilledNeighbours = GenGetNeighbours(xs, ys, step, 'p', size);
	//		if (GenRandomFill(nFilledNeighbours))
	//			GenFillBlock(xs, ys, step, 'p');
	//	}
	//}
	//
	////TODO: fill spots of water with filled neighbours in all 4 directions (NESW) and again after the final step.
	//
	//// cycle through increasingly small blocks and fill them randomly depending on number of filled neighbour blocks
	////TODO: optimize (only check the relevant squares)
	//bool reverse = false;
	//while (step > 1)
	//{
	//	step = step/2;
	//	if (reverse)
	//	{
	//		for (y = step; y < size; y += step)
	//		{
	//			for (x = step; x < size; x += step)
	//			{
	//				if (tilemap[x][y] != 'p')
	//				{
	//					nFilledNeighbours = GenGetNeighbours(x, y, step, 'p', size);
	//					if (nFilledNeighbours)
	//					{
	//						if (MathUtil::RandomInt(9) < 2*(nFilledNeighbours-1))
	//						{
	//							GenFillBlock(x, y, step, 'p');
	//						}
	//					}
	//				}
	//			}
	//		}
	//		reverse = false;
	//	}
	//	else
	//	{
	//		for (y = size - step; y > 0; y -= step)
	//		{
	//			for (x = size - step; x > 0; x -= step)
	//			{
	//				if (tilemap[x][y] != 'p')
	//				{
	//					nFilledNeighbours = GenGetNeighbours(x, y, step, 'p', size);
	//					if (nFilledNeighbours)
	//					{
	//						if (MathUtil::RandomInt(9) < 2*(nFilledNeighbours-1))
	//						{
	//							GenFillBlock(x, y, step, 'p');
	//						}
	//					}
	//				}
	//			}
	//		}
	//		reverse = true;
	//	}
	//}
	//
	//// fill tiny puddles of water and smooth coast
	//for (int i = 1; i <= 4; i++)
	//{
	//	for (x = 1; x < size; x++)
	//	{
	//		for (y = 1; y < size; y++)
	//		{
	//			if (tilemap[x][y] != 'p')
	//			{	if (GenGetNeighbours(x, y, 1, 'p', size) >= 5)
	//					tilemap[x][y] = 'p';
	//			}
	//		}
	//	}
	//}
	//
	//// mark ocean tiles bordering on plains as coast
	//for (x = 1; x < size; x++)
	//{
	//	for (y = 1; y < size; y++)
	//	{
	//		if (tilemap[x][y] == 'o' && GenGetNeighbours(x, y, 1, 'p', size) > 0)
	//			tilemap[x][y] = 'c';
	//	}
	//}
	//
	////tmp
	//for (int t = 10; t < 100; t += 10)
	//{
	//	GenTreeTrunk(Vec2i(size/2 + t, size/2 + t), size);
	//}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // end CreateLevel()

void Level::LoadLevel(String filename)
{
	// Create tile Actors and an ascii map file (argument filename)
	StringList asciiMap;
	bool success = GetLinesFromFile(filename, asciiMap);
	int mapSizeY = int(asciiMap.size());

	// quick and dirty. check if correct...
//	size.Y = mapSizeY;
//	size.X = int(asciiMap[0].size());
	
	for (int y = 0; y < mapSizeY; y++)
	{
		for (int x = 0; x < int(asciiMap[y].size()); x++)
		{
			tilemap[x][y] = asciiMap[y].substr(x,1).at(0);
			float flipY = float(mapSizeY - y - 1);
			Tile *tile = new Tile(asciiMap[y].substr(x,1).at(0), Vector2(float(x), flipY));
		}
	}
} // end LoadLevel()

void Level::BitMaskLevel(Vec2i size)
{
	for (int y = 1; y < size.Y - 1; y++)
	{
		for (int x = 1; x < size.X - 1; x++)
		{
			if (tilemap[x][y] == 'c')
				bitmaskmap[x][y] = BitMaskTile(x, y, 'p', NULL);
			else if (tilemap[x][y] == 't')
				bitmaskmap[x][y] = BitMaskTile(x, y, NULL, 't');
		}
	}
} // end BitMaskLevel()

void Level::CreateTileActors(Vec2i size)
{
	for (int x = 0; x < size.X; x++)
	{
		for (int y = 0; y < size.Y; y++)
		{
			float flipY = float(size.Y - y - 1);
			Tile *tile = new Tile(tilemap[x][y], Vector2(float(x), flipY));
			tile->SetUp();

			if (tilemap[x][y] == 'c')
			{
				StringList spriteName(3);
				spriteName[0] = "Resources/Images/Tiles/coast/";
				spriteName[1] = IntToString(bitmasktable[bitmaskmap[x][y]]);
				spriteName[2] = ".png";
				tile->SetSprite(JoinString(spriteName));
			}
			else if (tilemap[x][y] == 't')
			{
				StringList spriteName(3);
				spriteName[0] = "Resources/Images/Tiles/tree/";
				spriteName[1] = IntToString(bitmasktable[bitmaskmap[x][y]]);
				spriteName[2] = ".png";
				tile->SetSprite(JoinString(spriteName));
			}
		}
	}
} // end CreateTileActors()

void Level::UnloadLevel()
{

} // end UnloadLevel()

void Level::AddVisibleTilesToWorld(Vector2 dudePosition)
{
	// Add tiles visible to the player to the world so they will be drawn to the screen
	StringList tileName (2);
	for (int x = int(dudePosition.X) - drawDist; x <= int(dudePosition.X) + drawDist; x++)
	{
		for (int y = int(dudePosition.Y) - drawDist; y <= int(dudePosition.Y) + drawDist; y++)
		{
			tileName[0] = IntToString(x);
			tileName[1] = IntToString(y);
			Tile* tile = (Tile*)Actor::GetNamed(JoinString(tileName, "_"));
			if (tile)
			{	
				theWorld.Add(tile, "tiles");	
			}
		}
	}
} // end AddVisibleTilesToWorld()

void Level::AddVisibleCreaturesToWorld(Vector2 dudePosition)
{
	// Add creatures visible to the player to the world so they will be drawn to the screen
	ActorSet allEnemies = theTagList.GetObjectsTagged("enemy");
	for (ActorSet::iterator it = allEnemies.begin(); it != allEnemies.end(); ++it)
	{
		Creature *enemy = (Creature*)*it;
		Vector2 pos = enemy->intPosition;
		if (pos.X >= dudePosition.X - drawDist && pos.X <= dudePosition.X + drawDist &&
			pos.Y >= dudePosition.Y - drawDist && pos.Y <= dudePosition.Y + drawDist)
		{	enemy->Tag("in_world");	
			enemy->IdleAnim();		}
	}
} // end AddVisibleCreaturesToWorld()

void Level::FadeInTiles(Vector2 dudePosition)
{
	StringList	tileName (2);
	int			fadeInDelayMS	= theTuning.GetInt("tileFadeInDelayMaxMS");		// maximum fade-in-delay in milliseconds
	for (int x = int(dudePosition.X) - drawDist; x <= int(dudePosition.X) + drawDist; x++)
	{
		for (int y = int(dudePosition.Y) - drawDist; y <= int(dudePosition.Y) + drawDist; y++)
		{
			tileName[0] = IntToString(x);
			tileName[1] = IntToString(y);
			Tile* tile = (Tile*)Actor::GetNamed(JoinString(tileName, "_"));
			if (tile)
			{	
				tile->SetSize(0.0f);
				// this is to set a random delay, after which the tile actually changes size (see Tile::Update)
				tile->fadeInAt = GetTickCount() + rand()%fadeInDelayMS;
			}
		}
	}
} // end FadeInTiles()

void Level::UpdateVisibleTiles(Vector2 dudePosition, int direction)
{
	// Update the tiles visible to the player and add/remove the relevant ones after player movement
	//				812		\N/
	//	direction:	7.3 =	W.E
	//				654		/S\ 

	StringList tileName (2);
	int xShift	= 0;
	int yShift	= 0;

	// For diagonal movement the row/column of added tiles must be shifted by one unit
	// in the direction of the other directional component (for example: direction = 2 (NE)
	// -> the row of tiles to be added North of the player must be shifted one unit to the East)
	if (direction == 2 || direction == 4)
		xShift = +1;
	if (direction == 8 || direction == 6)
		xShift = -1;
	if (direction == 8 || direction == 2)
		yShift = +1;
	if (direction == 6 || direction == 4)
		yShift = -1;

	// Add a row/column of tiles in the direction of movement; remove one in the opposite direction
	if (direction == 1 || direction == 2 || direction == 8)		// Dude moves North
	{
		for (int x = int(dudePosition.X) - drawDist; x <= int(dudePosition.X) + drawDist; x++)
		{
			tileName[0] = IntToString(x);
			tileName[1] = IntToString(int(dudePosition.Y) - drawDist);
			Tile *tileOut = (Tile*)Actor::GetNamed(JoinString(tileName, "_"));
			if (tileOut)
			{	tileOut->SetAlpha(1.0f);
				theWorld.Remove(tileOut);	}	// Remove the south-most row of tiles (Will only get rendered if tagged "in_world")
			tileName[0] = IntToString(x + xShift);
			tileName[1] = IntToString(int(dudePosition.Y) + drawDist + 1);
			Tile *tileIn = (Tile*)Actor::GetNamed(JoinString(tileName, "_"));
			if (tileIn)
			{	theWorld.Remove(tileIn);	// Remove the tile in case it's already been added *
				theWorld.Add(tileIn, "tiles");	}	// Add a row of tiles to the north
		}		// * (happens at the corners during diagonal movement)
	}
	if (direction == 3 || direction == 2 || direction == 4)		// Dude moves East
	{
		for (int y = int(dudePosition.Y) - drawDist; y <= int(dudePosition.Y) + drawDist; y++)
		{
			tileName[0] = IntToString(int(dudePosition.X) - drawDist);
			tileName[1] = IntToString(y);
			Tile *tileOut = (Tile*)Actor::GetNamed(JoinString(tileName, "_"));
			if(tileOut)
			{	tileOut->SetAlpha(1.0f);
				theWorld.Remove(tileOut);	}
			tileName[0] = IntToString(int(dudePosition.X) + drawDist + 1);
			tileName[1] = IntToString(y + yShift);
			Tile *tileIn = (Tile*)Actor::GetNamed(JoinString(tileName, "_"));
			if (tileIn)
			{	theWorld.Remove(tileIn);
				theWorld.Add(tileIn, "tiles");	}
		}
	}
	if (direction == 5 || direction == 4 || direction == 6)		// Dude moves South
	{
		for (int x = int(dudePosition.X) - drawDist; x <= int(dudePosition.X) + drawDist; x++)
		{
			tileName[0] = IntToString(x);
			tileName[1] = IntToString(int(dudePosition.Y) + drawDist);
			Tile *tileOut = (Tile*)Actor::GetNamed(JoinString(tileName, "_"));
			if(tileOut)
			{	tileOut->SetAlpha(1.0f);
				theWorld.Remove(tileOut);	}
			tileName[0] = IntToString(x + xShift);
			tileName[1] = IntToString(int(dudePosition.Y) - drawDist - 1);
			Tile *tileIn = (Tile*)Actor::GetNamed(JoinString(tileName, "_"));
			if (tileIn)
			{	theWorld.Remove(tileIn);
				theWorld.Add(tileIn, "tiles");	}
		}
	}
	if (direction == 7 || direction == 6 || direction == 8)		// Dude moves West
	{
		for (int y = int(dudePosition.Y) - drawDist; y <= int(dudePosition.Y) + drawDist; y++)
		{
			tileName[0] = IntToString(int(dudePosition.X) + drawDist);
			tileName[1] = IntToString(y);
			Tile *tileOut = (Tile*)Actor::GetNamed(JoinString(tileName, "_"));
			if(tileOut)
			{	tileOut->SetAlpha(1.0f);
				theWorld.Remove(tileOut);	}
			tileName[0] = IntToString(int(dudePosition.X) - drawDist - 1);
			tileName[1] = IntToString(y + yShift);
			Tile *tileIn = (Tile*)Actor::GetNamed(JoinString(tileName, "_"));
			if (tileIn)
			{	theWorld.Remove(tileIn);
				theWorld.Add(tileIn, "tiles");	}
		}
	}
} // end UpdateVisibleTiles()

void Level::UpdateVisibleFriends(Vector2 dudePosition)
{
	ActorSet allFriends = theTagList.GetObjectsTagged("friend");
	for (ActorSet::iterator it = allFriends.begin(); it != allFriends.end(); ++it)
	{
		Dude *crab = (Dude*)*it;
		Vector2 pos = crab->intPosition;
		if (pos.X >= dudePosition.X - drawDist && pos.X <= dudePosition.X + drawDist &&
			pos.Y >= dudePosition.Y - drawDist && pos.Y <= dudePosition.Y + drawDist)
		{	
			if (!crab->IsTagged("in_world"))
			{	crab->Tag("in_world");
				if (!crab->IsTagged("limbo"))
					crab->IdleAnim();
				else
					crab->SetSpriteFrame(theTuning.GetInt("animDudeDeathLast") - 1);
			}
		}
		else
		{	
			if (crab->IsTagged("in_world"))
			{	crab->Untag("in_world");	
				crab->SetAlpha(1.0f);
				if (!crab->IsTagged("limbo"))
					crab->SetSpriteFrame(0);
			}
		}
	}
} // end UpdateVisibleFriends()

void Level::UpdateVisibleEnemies(Vector2 dudePosition)
{
	ActorSet allEnemies = theTagList.GetObjectsTagged("enemy");
	for (ActorSet::iterator it = allEnemies.begin(); it != allEnemies.end(); ++it)
	{
		Creature *enemy = (Creature*)*it;
		Vector2 pos = enemy->intPosition;
		if (pos.X >= dudePosition.X - drawDist && pos.X <= dudePosition.X + drawDist &&
			pos.Y >= dudePosition.Y - drawDist && pos.Y <= dudePosition.Y + drawDist)
		{	if (!enemy->IsTagged("in_world"))
			{	enemy->Tag("in_world");		
				if (!enemy->IsTagged("limbo"))
					enemy->IdleAnim();
				else
					enemy->SetSpriteFrame(theTuning.GetInt("animAntDeathLast") - 1);
			}
		}
		else
		{	if (enemy->IsTagged("in_world"))
			{	enemy->Untag("in_world");
				enemy->SetAlpha(1.0f);
				if (!enemy->IsTagged("limbo"))
					enemy->SetSpriteFrame(0);
			}
		}
	}
} // end UpdateVisibleEnemies()

void Level::RemoveAllTiles()
{
	ActorSet allTiles = theTagList.GetObjectsTagged("tile");
	for (ActorSet::iterator it = allTiles.begin(); it != allTiles.end(); ++it)
	{
		Tile *tile = (Tile*)*it;
		theWorld.Remove(tile);
	}
} // end RemoveAllTiles()

void Level::Fog(Vector2 dudeNewPos)
{
	for (int x = int(dudeNewPos.X) - drawDist; x <= int(dudeNewPos.X) + drawDist; x++)
	{
		int y = dudeNewPos.Y + drawDist;
		FogLine(dudeNewPos.X, dudeNewPos.Y, x, y);
		y = dudeNewPos.Y - drawDist;
		FogLine(dudeNewPos.X, dudeNewPos.Y, x, y);
	}
	for (int y = int(dudeNewPos.Y) - drawDist + 1; y <= int(dudeNewPos.Y) + drawDist - 1; y++)
	{
		int x = dudeNewPos.X + drawDist;
		FogLine(dudeNewPos.X, dudeNewPos.Y, x, y);
		x = dudeNewPos.X - drawDist;
		FogLine(dudeNewPos.X, dudeNewPos.Y, x, y);
	}
} // end Fog()

void Level::UnFog(Vector2 position)
{
	// Add tiles visible to the player to the world so they will be drawn to the screen
	StringList tileName (2);
	for (int x = int(position.X) - drawDist; x <= int(position.X) + drawDist; x++)
	{
		for (int y = int(position.Y) - drawDist; y <= int(position.Y) + drawDist; y++)
		{
			tileName[0] = IntToString(x);
			tileName[1] = IntToString(y);
			Tile* tile = (Tile*)Actor::GetNamed(JoinString(tileName, "_"));
			if (tile)
			{	tile->SetAlpha(1.0f);	}
		}
	}
	// Do the same for enemies
	ActorSet allEnemies = theTagList.GetObjectsTagged("enemy");
	for (ActorSet::iterator it = allEnemies.begin(); it != allEnemies.end(); ++it)
	{
		Creature *enemy = (Creature*)*it;
		Vector2 pos = enemy->intPosition;
		if (pos.X >= position.X - drawDist && pos.X <= position.X + drawDist &&
			pos.Y >= position.Y - drawDist && pos.Y <= position.Y + drawDist)
		{	enemy->SetAlpha(1.0f);	}
	}
} // end UnFog()

//////////////////////////////////////////////////////////////////
// private member functions //////////////////////////////////////

void Level::GenFillBlock(int x, int y, int size, char type)
{
	for (int yi = y; yi < y + size; yi++)
	{
		for (int xi = x; xi < x + size; xi++)
		{
			tilemap[xi][yi] = type;
		}
	}
} // end GenFillBlock()

int Level::GenGetNeighbours(int x, int y, int distance, char type, Vec2i mapSize)
{
	// Get the number of neighbours that are of the passed type (counts the tile at x,y as well!)
	int nFilledNeighbours = 0;
	for (int yi = y - distance; yi <= y + distance; yi += distance)
	{
		for (int xi = x - distance; xi <= x + distance; xi += distance)
		{
			if (xi < mapSize.X && xi >= 0 && yi < mapSize.Y && yi >= 0)
			{
				if (tilemap[xi][yi] == type)
					nFilledNeighbours++;
			}
		}
	}
	return nFilledNeighbours;
} // end GenGetNeighbours()

bool Level::GenRandomFill(int nFilledNeighbours)
{
	return(rand()%10 < 2*(nFilledNeighbours-1));
//	return(MathUtil::RandomInt(13) < 2*(nFilledNeighbours));
//	return(MathUtil::RandomInt(10) < 2*(nFilledNeighbours));
} // end GenRandomFill()

bool Level::GenTreeTrunk(Vec2i position, Vec2i mapSize)
{
	// TODO: check if position + size(?) is inside map
	int x,y = 0;
	int nFilledNeighbours = 0;
	GenFillBlock(position.X - 1, position.Y - 1, 3, 't');
	for (int round = 2; round <= 3; round++)
	{
		y = position.Y - round;
		for (x = position.X - round; x <= position.X + round; x++)
		{
			nFilledNeighbours = GenGetNeighbours(x, y, 1, 't', mapSize);
			if (GenRandomFill(nFilledNeighbours))
				tilemap[x][y] = 't';
		}

		x = position.X + round;
		for (y = position.Y - round; y <= position.Y + round; y++)
		{
			nFilledNeighbours = GenGetNeighbours(x, y, 1, 't', mapSize);
			if (GenRandomFill(nFilledNeighbours))
				tilemap[x][y] = 't';
		}

		y = position.Y + round;
		for (x = position.X + round; x >= position.X - round; x--)
		{
			nFilledNeighbours = GenGetNeighbours(x, y, 1, 't', mapSize);
			if (GenRandomFill(nFilledNeighbours))
				tilemap[x][y] = 't';
		}

		x = position.X - round;
		for (y = position.Y + round; y >= position.Y - round; y--)
		{
			nFilledNeighbours = GenGetNeighbours(x, y, 1, 't', mapSize);
			if (GenRandomFill(nFilledNeighbours))
				tilemap[x][y] = 't';
		}
	}
	return true;
} // end GenTreeTrunk

int Level::BitMaskTile(int x, int y, char one, char zero)
{
	// if 'x' gets passed as argument one and zero is NULL, neighbours of type 'x' count towards bitCase
	// if 'x' gets passed as argument zero and one is NULL, neighbours that are not of type 'x' count towards bitCase

	int bitCase = 0;

	if (one && !zero)
	{
		if (tilemap[x][y-1] == one)
			bitCase += 16;
		if (tilemap[x+1][y-1] == one)
			bitCase += 1;
		if (tilemap[x+1][y] == one)
			bitCase += 32;
		if (tilemap[x+1][y+1] == one)
			bitCase += 2;
		if (tilemap[x][y+1] == one)
			bitCase += 64;
		if (tilemap[x-1][y+1] == one)
			bitCase += 4;
		if (tilemap[x-1][y] == one)
			bitCase += 128;
		if (tilemap[x-1][y-1] == one)
			bitCase += 8;
	}
	else if (!one && zero)
	{
		if (tilemap[x][y-1] != zero)
			bitCase += 16;
		if (tilemap[x+1][y-1] != zero)
			bitCase += 1;
		if (tilemap[x+1][y] != zero)
			bitCase += 32;
		if (tilemap[x+1][y+1] != zero)
			bitCase += 2;
		if (tilemap[x][y+1] != zero)
			bitCase += 64;
		if (tilemap[x-1][y+1] != zero)
			bitCase += 4;
		if (tilemap[x-1][y] != zero)
			bitCase += 128;
		if (tilemap[x-1][y-1] != zero)
			bitCase += 8;
	}

	return bitCase;	
} // end BitMask()

void Level::FogLine(int x0, int y0, int x1, int y1)
{
	StringList tileName (2);
	bool sight_blocked = false;
	std::vector<Vector2> line = Helper::BresenhamLine(x0, y0, x1, y1);
	int tileCountIndex = 0;
	if (line[0] != Vector2(x0, y0))
	{	tileCountIndex = line.size() - 1;	}
	while (true)
	{
		tileName[0] = FloatToString(line[tileCountIndex].X);
		tileName[1] = FloatToString(line[tileCountIndex].Y);
		Tile* tile = (Tile*)Actor::GetNamed(JoinString(tileName, "_"));
		if (tile)
		{
			if (sight_blocked && !tile->visible)
			{	//tile->SetAlpha(0.5f);	
				tile->ChangeColorTo(Color(1.0f, 1.0f, 1.0f, 0.5f), theTuning.GetFloat("moveDuration"));	
				if (tile->IsTagged("occupied") && tile->occupantName != "")
				{	GetNamed(tile->occupantName)->ChangeColorTo(Color(1.0f, 1.0f, 1.0f, 0.0f), theTuning.GetFloat("moveDuration"));	}	// TODO: untag in_world (when message sent after completion of color change)
			}
			else
			{	tile->visible = true;
				//tile->SetAlpha(1.0f);	
				tile->ChangeColorTo(Color(1.0f, 1.0f, 1.0f, 1.0f), theTuning.GetFloat("moveDuration"));
				if (tile->IsTagged("occupied") && tile->occupantName != "")
				{	GetNamed(tile->occupantName)->ChangeColorTo(Color(1.0f, 1.0f, 1.0f, 1.0f), theTuning.GetFloat("moveDuration"));	}	// TODO: tag in_world (right away)
			}
			if (tile->IsTagged("opaque"))
			{	sight_blocked = true;	}
		}
		if (line[0] != Vector2(x0, y0))
		{
			tileCountIndex--;
			if (tileCountIndex < 0)	break;
			if (tileCountIndex < int(line.size()) - viewDist)	
				sight_blocked = true;
		}
		else
		{
			tileCountIndex++;
			if (tileCountIndex >= int(line.size()))	break;
			if (tileCountIndex > viewDist)	
				sight_blocked = true;
		}
	}
} // end FogLine()

//////////////////////////////////////////////////////////////////
// angel2d functions /////////////////////////////////////////////

void Level::ReceiveMessage(Message* message)
{
	String messageName = message->GetMessageName();
	if (messageName == "devReloadLevel")
	{
		//theWorld.PauseSimulation();
		//theWorld.UnloadAll();
		//// create protagonist
		//Dude* dude = new Dude();
		//// add protagonist to the world and run setup
		//theWorld.Add(dude, "characters");
		//dude->SetUp();
		//theWorld.Add(new GridActor(), -1);
		//Level* level = new Level();
		//level->SetUp();
		//// load level from file
		//level->LoadLevel("Resources/Maps/test_map.txt");
		//theWorld.ResumeSimulation();
	}
}

//////////////////////////////////////////////////////////////////
// Tile class ////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

Tile::Tile(char type, Vector2 position)
{
	visible			= false;	// set visibility tracker for Fog()
	occupantName	= "";
	fadeInAt		= -1;

	StringList tileName (2);
	tileName[0] = FloatToString(position.X);
	tileName[1] = FloatToString(position.Y);
	SetName(JoinString(tileName, "_"));
	SetPosition(position);
	SetDrawShape(ADS_Square);
	Tag("tile");
	
	if		(type == 'p')
	{	SetSprite("Resources/Images/Tiles/plains.png");		
		Tag("walkable");								}
	else if (type == 'f')
	{	SetSprite("Resources/Images/Tiles/forest.png");
		Tag("opaque");									}
	else if (type == 'o')
	{	LoadSpriteFrames("Resources/Images/Tiles/water_001.png");
		Tag("ocean");									}
	else if (type == 'c')
	{	}
	else if (type == 't')
	{
		Tag("opaque");
	}
}

void Tile::SetUp()
{
	theSwitchboard.SubscribeTo(this, "StartTileAnimation");
} // end SetUp()

void Tile::ReceiveMessage(Message* message)
{
	String messageName = message->GetMessageName();
	if (messageName == "StartTileAnimation")
	{
		if (IsTagged("ocean"))
		{
			float	animDuration	= theTuning.GetFloat("tileAnimDuration");
			int		firstFrame		= theTuning.GetInt("animOceanFirst") - 1;
			int		lastFrame		= theTuning.GetInt("animOceanLast") - 1;
			float	delay			= animDuration / ((lastFrame - firstFrame + 1) * 2 - 2);
			PlaySpriteAnimation(delay, SAT_PingPong, firstFrame, lastFrame, "Ocean");
		}
	}
} // end ReceiveMessage()

void Tile::AnimCallback(String animName)
{
	// Gets called when an animation (named animName) finishes
}

void Tile::Update(float dt)
{
	visible = false;	// reset visibility tracker for Fog()
	
	// fade in tile at game start
	if (fadeInAt > 0)
	{
		if (fadeInAt <= GetTickCount())
		{
			ChangeSizeTo(1.0f, theTuning.GetFloat("tileFadeInDuration"), false);
			fadeInAt = -1;
		}
	}

	Actor::Update(dt);
}

void Tile::Render()
{
	Actor::Render();
}