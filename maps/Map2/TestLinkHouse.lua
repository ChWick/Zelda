
block1 = -1
block2 = -1
block3 = -1
block4 = -1
blockade1 = -1
blockade2 = -1
luaScriptInstructionText = -1

-- initialisation function
function init() 
end

-- start function
function start()
	log("Start main map")
	block1 = createCharacter("Blocker", "11, 1, 50", 180)
	block2 = createCharacter("Blocker", "12, 1, 50", 180)
	block3 = createCharacter("Blocker", "27, 1, 44", -90)
	block4 = createCharacter("Blocker", "27, 1, 45", -90)
	blockade1 = addInvisibleBlockade("9, -3, 51", "10, 10, 1")
	blockade2 = addInvisibleBlockade("28, -1, 44", "1, 10, 10")
	luaScriptInstructionText = createLuaScript("InstructionText.lua")
	setNextBlockStartMessage()
end

function stop()
	log("Stop main map")
	delete(blockade1, blockade2, block1, block2, block3, block4, luaScriptInstructionText)
end

function setNextBlockStartMessage()
	setChat(block1, luaScriptInstructionText)
	setChat(block2, luaScriptInstructionText)
	setChat(block3, luaScriptInstructionText)
	setChat(block4, luaScriptInstructionText)
end
