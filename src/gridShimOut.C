// =================================================================================
//    Read a grid from a data base and output the data to a plain text file. 
//   
//  This routine can be changed to create an output file with any desired format.
//
// Example:
//     gridShim cic.hdf cic.out
//     mpirun -np 2 gridShim cic.hdf cic.out
// ==================================================================================

#include "Overture.h"  
#include "PlotStuff.h"
#include "display.h"
#include "ParallelUtility.h"

#include "inAndOutHDF5.h"
#include "inAndOut.h"

#include <stdlib.h>
#include <iostream>


int getFileNameData(    int             argc, 
                        char            *argv[], 
                        const char*     &nameOfOGFile, 
                        const char*     &nameOfNewFile,
                        char            saveLocation[] )
{
    ///////////////////////////////////////////////////////////////////////////
    // Preperations for accessing and saving files ////////////////////////////
    if( argc == 3 )                                                  //////////                    
    {                                                                //////////
        nameOfOGFile    = argv[1];                                   //////////
        nameOfNewFile   = argv[2];                                   //////////
    }                                                                //////////                           
    else                                                             //////////
    {                                                                //////////
        std::cerr << "Usage: gridShimOut gridName.in outfile.hdf \n" << std::endl;   //////////
    }                                                                //////////
                                                                     //////////
    const char *fileDir = "/home/overture/OvertureShim/hydeGrids/"; //////////
                                                                     //////////
    strcpy( saveLocation, fileDir );                                 //////////
    strcat( saveLocation, nameOfNewFile );                           //////////
    //strcat( saveLocation, ".txt" );                                //////////
    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    return 0;
}


int main( int argc, char *argv[] )
{
  printF( " Usage: gridShimOut gridName.in outfile.hdf \n" );

  /////////////////////////////////////////////////////////////////////////
  // Retrieve file names and save location ////////////////////////////////
  const char*     nameOfNewFile;
  const char*     nameOfOGFile;
  char            saveLocation[100];

  getFileNameData(    argc, 
                      argv, 
                      nameOfOGFile, 
                      nameOfNewFile,
                      saveLocation );
  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////


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
  

  printF( "Output written to file %s\n", nameOfNewFile );

  return 0;
}