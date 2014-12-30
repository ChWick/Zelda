function start()
   if (hasItem("lamp")) then
      setInnerObject("lamp_chest", "rupee_blue")
   else 
      setInnerObject("lamp_chest", "tool_lamp");
   end

   playAnimation("links_father", "Sit");
   playAnimation("player", "Sleep");

   warp("player", "-0.165, 0, -0.255", "90", "0, 1, 0");
end
