function start()
   playAnimation("links_father", "StandUp", true);
   waitForAnimationHasStopped("links_father", "StandUp");
   moveTo("links_father", "-0.1 0 0");
   moveTo("links_father", "0.2 0 0.2");
   delete("links_father");
end
