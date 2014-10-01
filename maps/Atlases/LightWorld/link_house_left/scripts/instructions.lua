function makeXmlMessage(chatText)
  return "<message message_type=\"show_text\" language_string=\"" .. chatText .. "\"/>"
end

function start()
  blockStartMessage = readIntFromMemory("blockStartMessage", 0)

  blockStartMessage = blockStartMessage + 1
  if blockStartMessage > 7 then
    blockStartMessage = 1
  end

  chatText = "${block_text_" .. tostring(blockStartMessage) .. "}"
  textMessage(makeXmlMessage(chatText))
  
  writeIntToMemory("blockStartMessage", blockStartMessage)
end