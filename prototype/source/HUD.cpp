
#include "stdafx.h"
#include "Manager.h"

// initialize static member variables
String HUD::textHP = "_";
String HUD::textTurnCount = "_";
String HUD::textCrabCount = "_";
String HUD::textTurnView = "_";

HUD::HUD(int hudWidth, int hudHeight)
{
	height	= hudHeight;
	width	= hudWidth;
	HUDActor *hudBG = new HUDActor();
	hudBG->SetPosition(float(height + (width / 2)), float(height/2));
	hudBG->SetSize(float(width), float(height));
	hudBG->SetColor(0.0f, 0.0f, 0.0f);
	theWorld.Add(hudBG, "hud_bg");
}

void HUD::SetUp()
{
	theSwitchboard.SubscribeTo(this, "DamageIndMovementDone");

	fps_print = 0;
	time_since_last_drop = 0.0f;
}

void HUD::WriteMapBMP(CharMap map)
{
	BMP	bmp;

	bmp.SetSize(map.size(), map[0].size());
	bmp.SetBitDepth(24);
	
	for (int x = 0; x < map.size(); x++)
	{
		for (int y = 0; y < map[x].size(); y++)
		{
			if (map[x][y] == 'p')
			{	bmp(x,y)->Red =  11; bmp(x,y)->Green = 230; bmp(x,y)->Blue =  11;	}		//(0.2f, 1.0f, 0.2f)
			else if (map[x][y] == 'f')
			{	bmp(x,y)->Red =   0; bmp(x,y)->Green = 128; bmp(x,y)->Blue =   0;	}		//(0.0f, 0.5f, 0.0f);
			else if (map[x][y] == 'o')
			{	bmp(x,y)->Red =  77; bmp(x,y)->Green = 179; bmp(x,y)->Blue = 255;	}		//(0.3f, 0.7f, 1.0f);
			else if (map[x][y] == 't')
			{	bmp(x,y)->Red = 128; bmp(x,y)->Green =  51; bmp(x,y)->Blue =   0;	}		//(0.5f, 0.2f, 0.0f);
			else
			{	bmp(x,y)->Red = 255; bmp(x,y)->Green = 255; bmp(x,y)->Blue = 255;	}		//(1.0f, 1.0f, 1.0f);				
		}
	}
	bmp.WriteToFile("Resources/Images/map.bmp");
} // end WriteMapBMP()

void HUD::DisplayMap(int scale, CharMap map)
{
	mapActor	= new HUDActor();
	float xPos	= height + width/2;
	float yPos	= height - height/20 - map[0].size()*scale;

	mapActor->SetSprite("Resources/Images/map.bmp");
	mapActor->SetPosition(xPos, yPos);
	mapActor->SetSize(map.size()*scale*2, map[0].size()*scale*2);
	theWorld.Add(mapActor, "hud_items");

	/* OLD MAP GENERATION
	// set up map background
	mapBG = new HUDActor();
	mapBG->SetPosition(float(height/2), float(height/2));
	mapBG->SetSize(float(height));
	mapBG->SetColor(0.0f, 0.0f, 0.0f, 0.5f);
	theWorld.Add(mapBG, "hud_bg");

	//tmp
	//if (scale % 2)
	//	scale = scale + 1;

	float oddScaleOffset = 0;
	if (scale % 2)
		oddScaleOffset = 0.5f;

	for (int x = 0; x < map.size(); x++)
	{
		for (int y = 0; y < map[x].size(); y++)
		{
			MapTile *mt = new MapTile(map[x][y], Vector2(x*scale+oddScaleOffset,y*scale+oddScaleOffset), Vector2(x,y), scale);
		}
	}

	// dude
	StringList mapTileName (2);
	mapTileName[0] = FloatToString(dudePosition.X);
	mapTileName[1] = FloatToString(map[0].size() - dudePosition.Y - 1);
	HUDActor* mapTileDude = (HUDActor*)GetNamed(JoinString(mapTileName, "m"));
	if (mapTileDude)
		mapTileDude->SetColor(0.9f, 0.0f, 0.0f);	
*/
} // end DisplayMap()

void HUD::UpdateMap(Vector2 dudePosition, CharMap map, BoolMap explored)
{
	BMP	bmp;

	bmp.ReadFromFile("Resources/Images/map.bmp");

	// draw tiles
	for (int x = 0; x < map.size(); x++)
	{
		for (int y = 0; y < map[x].size(); y++)
		{
			if (map[x][y] == 'p')
			{	bmp(x,y)->Red =  11; bmp(x,y)->Green = 230; bmp(x,y)->Blue =  11;	}		//(0.2f, 1.0f, 0.2f)
			else if (map[x][y] == 'f')
			{	bmp(x,y)->Red =   0; bmp(x,y)->Green = 128; bmp(x,y)->Blue =   0;	}		//(0.0f, 0.5f, 0.0f);
			else if (map[x][y] == 'o')
			{	bmp(x,y)->Red =  77; bmp(x,y)->Green = 179; bmp(x,y)->Blue = 255;	}		//(0.3f, 0.7f, 1.0f);
			else if (map[x][y] == 't')
			{	bmp(x,y)->Red = 128; bmp(x,y)->Green =  51; bmp(x,y)->Blue =   0;	}		//(0.5f, 0.2f, 0.0f);
			else
			{	bmp(x,y)->Red = 255; bmp(x,y)->Green = 255; bmp(x,y)->Blue = 255;	}		//(1.0f, 1.0f, 1.0f);

			if (explored[x][y])
				bmp(x,y)->Alpha = 255;
			else
				bmp(x,y)->Alpha = 128;
		}
	}

	// draw player
	bmp(dudePosition.X,map[0].size()-dudePosition.Y-1)->Red		= 0;
	bmp(dudePosition.X,map[0].size()-dudePosition.Y-1)->Green	= 0;
	bmp(dudePosition.X,map[0].size()-dudePosition.Y-1)->Blue	= 255;
	//bmp(dudePosition.X,map[0].size()-dudePosition.Y-1)->Alpha	= 255;

	//TODO: draw only seen friends & enemies (maybe new tag seen or friend_seen, or just in_world)
	// draw friends
	ActorSet allFriends = theTagList.GetObjectsTagged("friend");
	for (ActorSet::iterator it = allFriends.begin(); it != allFriends.end(); ++it)
	{
		Dude* crab			= (Dude*)*it;
		if (!crab->IsTagged("limbo"))
		{
			Vector2 position	= crab->intPosition;
			position.Y			= map[0].size() - position.Y - 1;
			bmp(position.X,position.Y)->Red		= 230;
			bmp(position.X,position.Y)->Green	= 0;
			bmp(position.X,position.Y)->Blue	= 0;
		}
	}

	// draw enemies
	ActorSet allEnemies = theTagList.GetObjectsTagged("enemy");
	for (ActorSet::iterator it = allEnemies.begin(); it != allEnemies.end(); ++it)
	{
		Creature* enemy			= (Creature*)*it;
		if (!enemy->IsTagged("limbo"))
		{
			Vector2 position		= enemy->intPosition;
			position.Y				= map[0].size() - position.Y - 1;
			bmp(position.X,position.Y)->Red		= 230;
			bmp(position.X,position.Y)->Green	= 230;
			bmp(position.X,position.Y)->Blue	= 0;
		}
	}

	bmp.WriteToFile("Resources/Images/map.bmp");

	// remove last turn's map from cache
	PurgeTexture("Resources/Images/map.bmp");
	mapActor->SetSprite("Resources/Images/map.bmp");
} // end UpdateMap()

void HUD::RemoveMap()
{
	mapActor->Destroy();
	//theSwitchboard.Broadcast(new Message("RemoveMap"));
} // end RemoveMap()

void HUD::WinScreen(Vector2 position)
{
	//temporary
	TextActor* textWin = new TextActor("FontL", "You did it!", TXT_Center);
	textWin->SetLayer("hud_items");
	textWin->SetColor(1.0f, 0.0f, 0.0f);
	textWin->SetPosition(position);
	theWorld.Add(textWin);
} // end WinScreen()

void HUD::UpdateHP(int health)
{
	StringList hp (2);
	hp[0] = "Health: ";
	hp[1] = IntToString(health);
	textHP = JoinString(hp);
}

void HUD::UpdateTurnCount(int turnCount)
{
	StringList tc (2);
	tc[0] = "Turn: ";
	tc[1] = IntToString(turnCount);
	textTurnCount = JoinString(tc);
}

void HUD::UpdateCrabCount(int crabCount)
{
	StringList cc (2);
	cc[0] = "Crabs: ";
	cc[1] = IntToString(crabCount);
	textCrabCount = JoinString(cc);
}

void HUD::UpdateTurnView(int turnView)
{
	StringList tc (2);
	tc[0] = "Turn viewed: ";
	tc[1] = IntToString(turnView);
	textTurnView = JoinString(tc);
}

void HUD::DamageIndicator(Vector2 position, int damage, Color color)
{
	TextActor *damageInd = new TextActor("FontS", IntToString(damage));
	theWorld.Add(damageInd, "hud_items");
	damageInd->SetColor(color);
	damageInd->SetPosition(position);
	damageInd->MoveTo(Vector2(position.X, position.Y + 1.0f), 0.5f, false, "DamageIndMovementDone");

} // end DamageIndicator()

void HUD::ReceiveMessage(Message* message)
{
	String messageName = message->GetMessageName();
	if (messageName == "DamageIndMovementDone")
	{
		TextActor *damageInd =  (TextActor*)message->GetSender();
		damageInd->Destroy();
	}
}

void HUD::Update(float dt)
{
	if (ceil(1.0f/dt) < fps)
		fps = ceil(1.0f/dt);
	if (fps_print == 0)
	{
		fps_print = 10;
		fps = ceil(1.0f/dt);
	}
	fps_print--;
	textHP_ext = GetTextExtents(textHP, "FontM");
	textTurnCount_ext = GetTextExtents(textTurnCount, "FontM");
	textCrabCount_ext = GetTextExtents(textCrabCount, "FontM");
	textTurnView_ext  = GetTextExtents(textTurnView, "FontM");
	textFPS_ext = GetTextExtents(FloatToString(fps), "FontM");
}

void HUD::Render()
{
	glColor3f(1.0f, 1.0f, 1.0f);
	if (fps < 30 && glfwGetTime() - last_drop > 0.1f)
	{	
		time_since_last_drop = glfwGetTime() - last_drop;
		last_drop = glfwGetTime();
	}
	DrawGameText(FloatToString(fps),	"FontM", float(height + (width/2) - (textFPS_ext.X / 2)),		height/20);
	if (time_since_last_drop > 0.1f)
		DrawGameText(FloatToString(time_since_last_drop), "FontM", float(height + (width / 2)),			height/20 * 2);
	DrawGameText(textHP,				"FontM", float(height + (width/2) - (textHP_ext.X / 2)),		height/20 * 3);
	DrawGameText(textTurnCount,			"FontM", float(height + (width/2) - (textTurnCount_ext.X / 2)), height/20 * 4);
	DrawGameText(textCrabCount,			"FontM", float(height + (width/2) - (textCrabCount_ext.X / 2)), height/20 * 5);
	DrawGameText(textTurnView,			"FontM", float(height + (width/2) - (textTurnView_ext.X / 2)),	height/20 * 6);
}

//////////////////////////////////////////////////////////////////
// DmgInd class //////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

DmgInd::DmgInd(Vector2 position, int damage, Color color)
{
	SetFont("FontS");
	SetDisplayString(IntToString(damage));
	SetColor(color);
	SetPosition(position);
	speed = 10.0f;

	theWorld.Add(this, "hud_items");
} // end DmgInd()

/*
void DmgInd::Update(float dt)
{
	speed = speed / (1 + (dt / 1000));

	// Do Movement Update here...
}
*/

//////////////////////////////////////////////////////////////////
// MapTile class /////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

MapTile::MapTile(char type, Vector2 screenPosition, Vector2 mapPosition, float size)
{
	SetPosition(screenPosition);
	SetSize(size);
	SetDrawShape(ADS_Square);
	StringList mapTileName (2);
	mapTileName[0] = FloatToString(mapPosition.X);
	mapTileName[1] = FloatToString(mapPosition.Y);
	SetName(JoinString(mapTileName, "m"));

	if (type == 'p')
		SetColor(0.2f, 1.0f, 0.2f);
	else if (type == 'f')
		SetColor(0.0f, 0.5f, 0.0f);
	else if (type == 'o')
		SetColor(0.3f, 0.7f, 1.0f);
	else if (type == 't')
		SetColor(0.5f, 0.2f, 0.0f);
	else
		SetColor(1.0f, 1.0f, 1.0f);

	theWorld.Add(this, "hud_items");
	theSwitchboard.SubscribeTo(this, "RemoveMap");
} // end MapTile()


//TODO: remove
void MapTile::SetUp()
{
	theSwitchboard.SubscribeTo(this, "RemoveMap");
} // end SetUp()

void MapTile::ReceiveMessage(Message* message)
{
	String messageName = message->GetMessageName();
	if (messageName == "RemoveMap")
		Destroy();
} // end ReceiveMessage()

//////////////////////////////////////////////////////////////////
// Mouse class ///////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

Mouse::Mouse()
{
} // end Mouse()

void Mouse::MouseDownEvent(Vec2i screenCoordinates, MouseButtonInput button)
{
	if (button == MOUSE_LEFT)
	{	TypedMessage<Vec2i> *m = new TypedMessage<Vec2i>("MouseDownLeft", screenCoordinates);
		theSwitchboard.Broadcast(m);
	}
	else if (button == MOUSE_MIDDLE)
	{	TypedMessage<Vec2i> *m = new TypedMessage<Vec2i>("MouseDownMiddle", screenCoordinates);
		theSwitchboard.Broadcast(m);
	}
	else if (button == MOUSE_RIGHT)
	{	TypedMessage<Vec2i> *m = new TypedMessage<Vec2i>("MouseDownRight", screenCoordinates);
		theSwitchboard.Broadcast(m);
	}
} // end MouseDownEvent
