# make an extended channel
create mappings
  rectangle
    mappingName
      outside
    set corners
      -1,7, -1,4
    lines
      8 5
    boundary conditions
      -1 -1 1 1
    #share
    #  1 2 0 0
  exit
  rectangle
    mappingName
      inside4
    set corners
      2,4, 1,2
    lines
      6 3
    boundary conditions
      0 0 0 0
  exit
exit
#
generate an overlapping grid
    outside
    inside4
  done
  change parameters
    interpolation type
      explicit for all grids
    order of accuracy
      fourth order
    #interpolation width
    #  all
    #  all
    #  2 2
    ghost points
      all
      2 2 2 2 2 2
  exit
  compute overlap
exit
#
save an overlapping grid
  simpleBorderShare.hdf
  simpleBorderShare
exit
