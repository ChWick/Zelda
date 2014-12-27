function start()
  playAnimation("links_father", "StandUp");
  waitForAnimationHasStopped("links_father", "StandUp");
  moveTo("links_father", "0 0 -0.2");
  moveTo("links_father", "0 0 0.4");
  delete("links_father");
end
