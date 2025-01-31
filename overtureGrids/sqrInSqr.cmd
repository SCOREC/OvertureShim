# make a simple square
create mappings
  rectangle
    mappingName
      outside
    set corners
      -12.8,12.8, -6.4,6.4
    lines
      65 33
    boundary conditions
      -1 -1 1 1
  exit
  rectangle
    mappingName
      inside
    set corners
      -6.4,6.4, -3.2,3.2
    lines
      65 33
    boundary conditions
      0 0 0 0
  exit
exit
#
generate an overlapping grid
    outside
    inside
  done
  change parameters
    interpolation type
      explicit for all grids
    order of accuracy
      fourth order
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
exit
#
save an overlapping grid
  sqrInSqr.hdf
  sqrInSqr
exit
