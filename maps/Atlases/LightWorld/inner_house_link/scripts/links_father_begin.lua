function start()
   playAnimation("links_father", "walk");
   waitForAnimationHasStopped("links_father", "walk");
  moveTo("links_father", "-0.1 0 0");
  moveTo("links_father", "0.2 0 0.2");
  delete("links_father");
end
