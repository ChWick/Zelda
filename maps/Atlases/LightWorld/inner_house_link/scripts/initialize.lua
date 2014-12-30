function start()
  if (hasItem("lamp")) then
    setInnerObject("lamp_chest", "rupee_blue")
  else 
    setInnerObject("lamp_chest", "tool_lamp");
  end

  playAnimation("links_father", "Sit");
  playAnimation("player", "Sleep");

  -- Setup position
  warp("player", "-0.165, 0, -0.255", "90", "0, 1, 0");

  -- start with help call of zelda
  textMessage("<message message_type=\"show_text\" language_string=\"${help_call}\"/>");

  -- Move links father
  playAnimation("links_father", "StandUp");
  waitForAnimationHasStopped("links_father", "StandUp");
  moveTo("links_father", "0 0 -0.2");
  moveTo("links_father", "0 0 0.4");
  delete("links_father");

  -- now awake and normal play starts
  playAnimation("player", "Awake")
  waitForAnimationHasStopped("player", "Awake")
end
