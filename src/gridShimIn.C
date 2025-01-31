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
#include "inAndOutHDF5.h"


int main( int argc, char *argv[] )
{
    /////////////////////////////////////////////////////////////////////////
    // initialize Overture //////////////////////////////////////////////////
    Overture::start( argc, argv );  

    // number of processors:
    const int np   = max( 1, Communication_Manager::numberOfProcessors() ); 

    // my rank:
    const int myid = max( 0, Communication_Manager::My_Process_Number );


    /////////////////////////////////////////////////////////////////////////
    // Preperations for accessing and saving files //////////////////////////
    printF( " Usage: gridShimIn gridName.hdf outfile.txt \n\n" );    //////////
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
        Overture::abort( "error" );                                  //////////
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


    //int     *dim;
    int     *numOfComponentGrids;
    int     *numberOfDimensions;
    int     ***interior_box;
    int     ***domain_box;
    double  ****xy;  // xy[ i ][ j ][ k ][ l ]: j -> x, k -> y, l -> z, i -> numOfComponentGrids
    //int     ***mask; // mask[ i ][ j ][ k ]: i -> numOfComponentGrids, j, k -> ptTypes
    int     ***desc;

    int status = getFromHDF5(       nameOfOGFile,
                                    numOfComponentGrids,
                                    numberOfDimensions, 
                                    //dim, 
                                    interior_box, 
                                    domain_box, 
                                    xy, 
                                    //mask,
                                    desc );


    status     = sendToTextFile(    saveLocation, 
                                    *numOfComponentGrids,
                                    *numberOfDimensions, 
                                    interior_box, 
                                    domain_box, 
                                    xy, 
                                    desc );


    printF( "Output written to file %s\n", nameOfNewFile );
    
    Overture::finish();        
    return 0;
}
