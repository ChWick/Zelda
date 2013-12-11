FILE(REMOVE_RECURSE
  "CMakeFiles/Example1.dir/Example1.o"
  "../bin/Example1.pdb"
  "../bin/Example1"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang CXX)
  INCLUDE(CMakeFiles/Example1.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
