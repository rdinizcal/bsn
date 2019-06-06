file(REMOVE_RECURSE
  "libbsn.pdb"
  "libbsn.so"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/bsn.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
