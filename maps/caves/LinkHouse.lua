-- This is a comment
log('Parsing lua script of LinkHouse')

-- global variables
father = -1

-- initialisation function
function init() 
	father = createCharacter("FatherOfLink", "0, 5, 0")
	setInnerObject(getObject("Lamp"), "Lamp")
end

-- start function
function start()
	displayText("${zelda_init_begin}")
	wait("endDisplayText", "initZeldaTextRead")	
end

function initZeldaTextRead()
	displayText("${link_father_begin}")
	wait("endDisplayText", "initFatherTextRead")
end

function initFatherTextRead()
	moveToTarget(father, "0, 0, 3")
	wait("waitTargetReached", "fatherLeave", father)
end

function fatherLeave() 
	delete(father)
end
