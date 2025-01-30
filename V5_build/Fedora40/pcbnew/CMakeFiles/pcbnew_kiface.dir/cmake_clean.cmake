file(REMOVE_RECURSE
  "_pcbnew.kiface"
  "_pcbnew.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/pcbnew_kiface.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
