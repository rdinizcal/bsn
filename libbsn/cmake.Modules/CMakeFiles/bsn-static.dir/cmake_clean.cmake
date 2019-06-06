file(REMOVE_RECURSE
  "libbsn-static.pdb"
  "libbsn-static.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/bsn-static.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
