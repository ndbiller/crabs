-- This file is used for tuning variables. 

-- if > 0, test_map is loaded, otherwise map is procedurely generated
	loadMap = {
	  type = "int",
	  value = 0
	}
-- whether or not crabs should be rotated in direction of movement
	rotateCrab = {
	  type = "int",
	  value = 0
	}
-- smooth movement (non-linear movement - turning this off might improve performance)
	smoothMovement = {
	  type = "int",
	  value = 1
	}

-- temporary: damage done by the player with each hit (enemy has 10 hp)
	dudeDamage = {
	  type = "int",
	  value = 5
	}
	antDamage = {
	  type = "int",
	  value = 1
	}
	dudeHealth = {
	  type = "int",
	  value = 10
	}
	dudeViewDistance = {
	  type = "int",
	  value = 9
	}
	antHealth = {
	  type = "int",
	  value = 10
	}
	antViewDistance = {
	  type = "int",
	  value = 6
	}


-- Duration of each move in seconds 
--  (that includes moving from tile to tile, combat animations, etc.)
	moveDuration = {
	  type = "float",
	  value = 0.2
	}
	rotationDuration = {
	  type = "float",
	  value = 0.1
	}
	idleDuration = {
	  type = "float",
	  value = 0.4
	}
	fightDuration = {
	  type = "float",
	  value = 0.2
	}
	tileAnimDuration = {
	  type = "float",
	  value = 0.8
	}
	deathAnimDuration = {
	  type = "float",
	  value = 0.6
	}

-- duration of tile fade-in at the start of a game
	tileFadeInDuration = {
	  type = "float",
	  value = 0.2
	}
	tileFadeInDelayMaxMS = {
	  type = "int",
	  value = 800
	}
	
	
-- First and last frames of each animation. 
--  (e.g. 2 -> Resources/Images/Sprites/Animations/dude_002.png)
--  for now dude_001.png is the default frame (when no animation is playing)


-- Crab Animations:
-- (value 1 ist Ausgangsposition)

-- Crab Idle
	animDudeIdleFirst = {
	  type 	= "int",
	  value = 5
	}
	animDudeIdleLast = {
	  type 	= "int",
	  value = 8
	}

-- Crab Move
	animDudeMoveFirst = {
	  type 	= "int",
	  value = 1
	}
	animDudeMoveLast = {
	  type 	= "int",
	  value = 3
	}

-- Crab Fight
	animDudeFightFirst = {
	  type 	= "int",
	  value = 9
	}
	animDudeFightLast = {
	  type 	= "int",
	  value = 11
	}
	
-- Crab Death
	animDudeDeathFirst = {
	  type  = "int",
	  value = 12
	}
	animDudeDeathLast = {
	  type = "int",
	  value = 32
	}

--Crab Hit
	animDudeHit = {
	  type 	= "int",
	  value = 4
	}


-- Ant Animations:
-- (value 1 ist Ausgangsposition)

-- Ant Idle
	animAntIdleFirst = {
	  type  = "int",
	  value = 6
	}
	animAntIdleLast = {
	  type  = "int",
	  value = 9
	}

-- Ant Move
	animAntMoveFirst = {
	  type 	= "int",
	  value = 1
	}
	animAntMoveLast = {
	  type 	= "int",
	  value = 4
	}

-- Ant Fight
	animAntFightFirst = {
	  type 	= "int",
	  value = 9
	}
	animAntFightLast = {
	  type 	= "int",
	  value = 11
	}

-- Ant Death
	animAntDeathFirst = {
	  type  = "int",
	  value = 12
	}
	animAntDeathLast = {
	  type = "int",
	  value = 12
	}

-- Ant Hit
	animAntHit = {
	  type 	= "int",
	  value = 5
	}

	
	
-- Tile Animation:

-- Ocean
	animOceanFirst = {
	  type 	= "int",
	  value = 1
	}
	animOceanLast = {
	  type 	= "int",
	  value = 3
	}
	