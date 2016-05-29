This is the changelog for the original crabs prototype written in C++ with the Angel3D engine.
The final version (a_1.0) is in this github repository.
Note: to run the game in fullscreen run, for example: "bla.exe 1920 1080 1"
-----------------


a_0.1
-----

- movement with vi-keys:
	z k u
	 \|/
	h-.-l
	 /|\
	b j n
  (or arrow keys in only 4 directions)
- [Space] = Wait
- [Q] or [Esc] = Quit
- Resolution can be changed by arguments. 
  (Create shortcut, right click, Properties, Shortcut, Target:
  e.g. "C:\pathtofile\My GameJam Game.exe" 1024 600 true
  for 1024 x 600 resolution and fullscreen (true))
- sprites are named for example dude_001, dude_002, etc.
  -> the first one is the default sprite (no animation)
  -> which of the other frames will be used for animation is defined in Config/tuning.lua
- also defined in tuning.lua: movement & animation duration


a_0.2
-----

- switched to 24x24 pixel tiles (to fit more tiles on the screen while keeping the 640*360 base resolution)
	-> 640*360 scales nicely to 1280*720 (x2) and 1920*1080 (x3)
- the enemy now moves less erratically (moves straight more frequently and doesn't turn around unless necessary)


a_0.3
-----
- 16x16 tiles (needs to be power-of-two for crisp display on some systems)
- crabs vs. ants!
- enemies move towards dude
- "fog of war"
- enemy sprite rotation


a_0.4 (20.10.2013)
-----
- movement through Numpad-Keys (5 = wait)
- new art and animations (including fight) for characters. wiii
- enemy rotation during fight
- enemies set their goal towards current player location as long as it's visible to them. once they reach the goal (exactly) it is unset.
- enemies have limited view distance (10)
- enemies fade from view with fog
- fog now starts after first obstacle


a_0.5 (08.11.2013)
-----
- [F1] toggles Fog
- floating damage indicators
- new tile type "water" ('w')
- procedural island generation (256x256, only plains atm)
	-> set variable in tuning.lua to load map from text file as before.
- [M] toggles Map (very resource intensive; only for testing map generation...)
- now with teleportation! travel anywhere in the blink of an eye by clicking on the map. (magic)
BUG: world gets drawn after first move... 


a_0.6 (13.11.2013)
-----
- LAST VERSION BEFORE CHANGING TO INDIVIDUAL LEVELS
- bitmasking for terrain-transitions (see resources: angryfishstudios)
	-> new tile type "coast" ('c')
- 'w' (water) is now 'o' (ocean)
- movement key press possible before end of current movement
- framerate forced to 60 fps
BUG: world gets drawn after first move... 
BUG: framerate drop every 0.8 sec (due to tile animation...)
		-> create/destroy tiles during UpdateVisibleTiles()?
todo: pass bitmasking to Tile*


a_0.7 (03.03.2014)
-----
- removed island generation in favour of individual levels (different levels not yet implemented)
- now each time the player dies, he respawns together with previous incarnations ("friends"). (those don't move yet)
	-> revamped death system (tags "dead" and "limbo") and creature movement AI
- introduced tile tags "occupied_by_friend" & "occupied_by_enemy" for faster collision check
- fixed "world gets drawn after first move" bug [not completely!]
- ant view distance to 6 (-> tuning.lua)
BUG: world gets drawn after first move... (sometimes?)
todo: 	- ants should check whether the crab marked as goal is alive and reset goal if not. (see todo in code comment)


a_0.8 (05.03.2014)
-----
- basic friend movement (follows player path if possible)
- death animation
- tree texture test
- changes to readyForInput and MovementDone messaging (Fight, wait and collision now behave like Move)
- World::SetBackgroundColor() instead of background actor (fixes black shape bug on eee)
BUG: world gets drawn after first move... (sometimes? netbook?)
BUG(fixed): alpha of fog changes after death for tiles around place of death


a_0.9 (17.03.2014)
-----
- replay moves with the keypad keys '-' (previous turn) and '+' (next turn) for testing purposes. very basic
	-> return to current turn (turn viewed = -1) to continue moving. 
	   only actors visible and alive during current turn will be visible. no rotation. tiles rendering will not update.- basic friend "A.I."
- fog bug resolved (see a_0.8)
- camera stops moving at level boundaries
- no animations for creatures & dudes out of view
- player death animation
- ants lock onto closest crab in view (random if multiple) until closer crab is found
- crisp map
BUG: weird sprite jitter for dead friends sometimes (rarely..) 
		-> SetSprite instead of SetSpriteFrame when dying outside of view or when it comes back (if on time of death perhaps with small delay for low end performance)

		
a_1.0 (24.09.2014):
--------------------
- real time map (.bmp) with tiles and characters
- no damage indicators outside of view
- ant movement deterministic (always the same movement path until the player is the closest crab)
- ants no longer get destroyed upon death
- ant death animation (no sprites yet)
- changed openGL clampmode from GL_REPEAT to GL_CLAMP_TO_EDGE (-> no black lines between tiles when moving and improved performance)
- added tile "fade-in" with random delay
- win condition (reach right level border)
- press R to restart level
BUG(fixed)*: when ant is standing on an (ant) corpse and fighting with a crab, the crab moves onto the ant position on the second attack and the ant moves away
		also: both an ant and a crab can move onto the same corpse-tile and stack when this happens.


development version:
--------------------
- fixed bug* above. (dead ants were attacked in Dude->Fight, then "died" again and set the tile as unoccupied.)
- special move 1: "StepOff" (KP_0 followed by direction)


TODO: 	- unset goal if near?
	- search time before unsetting goal?
	- use BresenhamLine for ant movement towards goal (at least incorporate)
	- fog not symmetrical, some obstacles visible, others not, etc...
		-> cast longer rays?
	- set an ant's goal when it's visible to the player (is not the same as the current LineOfSight ant -> dude; maybe just switch direction)
		-> but check for distance (view distance of ants) [probably already fixed by switching direction of bresenham line (always from crab to enemy)]
	- negatively accelerated movement for damage indicators; one damage indicator displaying total damage this turn.
	- zoom out camera for testing
	- load tile with correct animation frame
	- ocean tile animation causes framerate drop...
	- optimize fog (framerate breaks down with fog on)
	- general optimization
	- make everything (creature movement, level generation, etc.) determined+
	- ants should preferably attack the oldest incarnations if there are more than one in reach (or pick random one and attack it consistently if in range (or follow?!))
	- random delay in start of idle animation
	- trim unnecessary animation sprites
	- dead characters & damage indicators get drawn in fog
	- draw order of corpses on the ground (most recently deceased is not necessarily on top) -> more layers?
	- map too large (/small?) on certain resolutions (not 640/1280/1920 or not 16:9)
	- Message broadcasting seems to be slow.. (check for messaging that can be avoided)
	- fog gets removed upon player death, gets redrawn if moving
	- visualize special priming
	- waiting on corpses / leaves -> heal?
	- camera LERP (linear interpolation)? -> slight lag in camera movement

RESOURCES:	-http://nehe.gamedev.net/tutorial/masking/15006/   (alpha masking in opengl)
			-http://www.codeproject.com/Articles/15604/Ray-casting-in-a-2D-tile-based-environment 	(source for the BresenhamLine algorithm)
			-http://www.angryfishstudios.com/2011/04/adventures-in-bitmasking/  (Bit masking for tile engine)
			-http://www.policyalmanac.org/games/aStarTutorial.htm 	(A* pathfinding algorithm [-> note note #3: known terrain])
			-https://www.youtube.com/watch?v=Fy0aCDmgnxg 	(Juice it or lose it - a talk by Martin Jonasson & Petri Purho)



