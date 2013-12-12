blockStartMessage = 0

function getNext(interactionDirection)
	blockStartMessage = blockStartMessage + 1
	if blockStartMessage > 7 then
		blockStartMessage = 1
	end
	
	chatText = "${block_text_" .. tostring(blockStartMessage) .. "}"
	return chatText
end
