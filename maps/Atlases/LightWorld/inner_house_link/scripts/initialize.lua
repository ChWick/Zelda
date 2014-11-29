function start()
   if (hasItem("lamp")) then
      setInnerObject("lamp_chest", "rupee_blue")
   else 
      setInnerObject("lamp_chest", "tool_lamp");
   end
end
