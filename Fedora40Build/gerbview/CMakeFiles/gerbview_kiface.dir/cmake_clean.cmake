file(REMOVE_RECURSE
  "_gerbview.kiface"
  "_gerbview.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/gerbview_kiface.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
