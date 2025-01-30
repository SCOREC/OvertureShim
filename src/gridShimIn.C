// =================================================================================
//    Read a grid from a data base and output the data to a plain text file. 
//   
//  This routine can be changed to create an output file with any desired format.
//
// Example:
//     gridShimIn cic.hdf cic.out
//     mpirun -np 2 gridShimIn cic.hdf cic.out
// ==================================================================================

#include "Overture.h"  
#include "PlotStuff.h"
#include "display.h"
#include "ParallelUtility.h"
#include <stdlib.h>
#include "inAndOut.h"

#define FOR_3D( i1, i2, i3, I1, I2, I3 ) \
    int I1Base  = I1.getBase(),   I2Base  = I2.getBase(),  I3Base  = I3.getBase();  \
    int I1Bound = I1.getBound(),  I2Bound = I2.getBound(), I3Bound = I3.getBound(); \
    for( i3 = I3Base; i3 <= I3Bound; i3++ ) \
    for( i2 = I2Base; i2 <= I2Bound; i2++ ) \
    for( i1 = I1Base; i1 <= I1Bound; i1++ )

#define FOR_3( i1, i2, i3, I1, I2, I3 ) \
    I1Base  = I1.getBase(),   I2Base  = I2.getBase(),  I3Base  = I3.getBase();  \
    I1Bound = I1.getBound(),  I2Bound = I2.getBound(), I3Bound = I3.getBound(); \
    for( i3 = I3Base; i3 <= I3Bound; i3++ ) \
    for( i2 = I2Base; i2 <= I2Bound; i2++ ) \
    for( i1 = I1Base; i1 <= I1Bound; i1++ )


int main( int argc, char *argv[] )
{
    /////////////////////////////////////////////////////////////////////////
    // Preperations for accessing and saving files //////////////////////////
    printF( " Usage: gridShimIn gridName.hdf outfile.txt \n" );    //////////
                                                                //////////
    aString nameOfOGFile;                                          //////////
    const char *nameOfNewFile;                                     //////////
                                                                //////////
    if( argc == 3 )                                                //////////                    
    {                                                              //////////
        nameOfOGFile    = argv[1];                                   //////////
        nameOfNewFile   = argv[2];                                   //////////
    }                                                              //////////                           
    else                                                           //////////
    {                                                              //////////
        printF( "Usage: gridShimIn gridName.hdf outfile.hdf \n" );   //////////
        //Overture::abort( "error" );                                  //////////
    }                                                              //////////
                                                                //////////
    const char *fileDir = "/home/overture/shim/textOutputs/";      //////////
                                                                    //////////
    char saveLocation[100];                                        //////////
    strcpy( saveLocation, fileDir );                               //////////
    strcat( saveLocation, nameOfNewFile );                         //////////
    strcat( saveLocation, ".txt" );                                //////////
    /////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////

    


    int     *dim;
    int     ***interior_box;
    int     ***domain_box;
    double  ****xy;  // xy[ i ][ j ][ k ][ l ]: j -> x, k -> y, l -> z, i -> numOfComponentGrids
    int     ***mask; // mask[ i ][ j ][ k ]: i -> numOfComponentGrids, j, k -> ptTypes

    int status = getFromHDF5(   nameOfOGFile, 
                                dim, 
                                interior_box, 
                                domain_box, 
                                xy, 
                                mask );


    status     = sendToTextFile(  saveLocation, 
                                  numberOfDimensions, 
                                  interior_box, 
                                  domain_box, 
                                  xy, 
                                  desc );


  }
  printF( "Output written to file %s\n", fileName );
  
  Overture::finish();          
  return 0;
}
