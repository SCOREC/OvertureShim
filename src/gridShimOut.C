// =================================================================================
//    Read a grid from a data base and output the data to a plain text file. 
//   
//  This routine can be changed to create an output file with any desired format.
//
// Example:
//     gridShim cic.hdf cic.out
//     mpirun -np 2 gridShim cic.hdf cic.out
// ==================================================================================

#include "inAndOutHDF5.h"
#include "inAndOut.h"

#include <stdlib.h>
#include <iostream>
#include <string>


int main( int argc, char *argv[] )
{

  std::string   outhdf;
  const char*   fileName;

  if( argc == 3 )
  {
    fileName  = argv[ 1 ];
    outhdf    = argv[ 2 ];
  }
  else
  {
    std::cerr << "Usage: gridShimOut gridName.in outfile.hdf " << std::endl;
  }

  int     *dim;
  int     **interior_box;
  int     **domain_box;
  double  ***xy;
  int     **mask;

  int status  =   getFromTextFile(  fileName, 
                                    dim, 
                                    interior_box, 
                                    domain_box, 
                                    xy, 
                                    mask );

  status      =   sendToHDF5(       outhdf, 
                                    dim, 
                                    interior_box, 
                                    domain_box, 
                                    xy, 
                                    mask );
  
  return 0;
}