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

  std::string   nameOfNewFile;
  const char*   nameOfOGFile;

  if( argc == 3 )
  {
    nameOfOGFile  = argv[ 1 ];
    nameOfNewFile    = argv[ 2 ];
  }
  else
  {
    std::cerr << "Usage: gridShimOut gridName.in outfile.hdf " << std::endl;
  }


  // Initialize grid data 
  int                 numOfComponentGrids;
  int                 numberOfDimensions;
  // xy[ i ][ j ][ k ][ l ]: j -> x, k -> y, l -> z, i -> numOfComponentGrids
  Array4D<double>     *xy              = new Array4D< double >();      
  Array3D<int>        *interior_box    = new Array3D< int >();
  Array3D<int>        *domain_box      = new Array3D< int >();
  Array3D<int>        *desc            = new Array3D< int >();

  int status  =   getFromTextFile(  nameOfOGFile,
                                    &numOfComponentGrids,
                                    &numberOfDimensions, 
                                    interior_box, 
                                    domain_box, 
                                    xy, 
                                    desc );

  status      =   sendToHDF5(       saveLocation, 
                                    numOfComponentGrids,
                                    numberOfDimensions, 
                                    interior_box, 
                                    domain_box, 
                                    xy, 
                                    desc );
  
  return 0;
}