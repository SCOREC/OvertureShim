# make a simple square 
create mappings 
  rectangle 
    mappingName 
    outside 
    set corners 
    -2.0,2.0, -2.0,2.0 
    lines 
    20 20 
    boundary conditions 
    1 2 3 4 
    exit 
  rectangle 
    mappingName 
    inside 
    set corners 
    -1.5,-0.5, -1.5,-0.5 
    lines 
    5 5 
    boundary conditions 
    0 0 0 0 
    exit 
  rectangle 
    mappingName 
    inside2 
    set corners 
    0.6,1.6, 0.6,1.6 
    lines 
    5 5 
    boundary conditions 
    0 0 0 0 
    exit 
  exit 
# 
generate an overlapping grid 
  outside 
  inside 
  inside2 
  done 
  change parameters 
    ghost points 
      all 
      2 2 2 2 2 2 
    exit 
  compute overlap 
  exit 
# 
save an overlapping grid 
sqrInSqr6.hdf 
sqrInSqr6 
exit 
