blockStartMessage = 0


function makeXmlMessage(chatText)
  return "<message message_type=\"show_text\" language_string=\"" .. chatText .. "\"/>"
end

function start()
  blockStartMessage = blockStartMessage + 1
  if blockStartMessage > 7 then
    blockStartMessage = 1
  end

  status = 1	
  chatText = "${block_text_" .. tostring(blockStartMessage) .. "}"
  res = textMessage(makeXmlMessage(chatText))
  if (res == 2) then
     res = textMessage(makeXmlMessage(chatText))
  end
     
end