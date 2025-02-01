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

#include "inAndOut.h"
#include "inAndOutHDF5.h"

#include <stdlib.h>


int startOverture( int argc, char *argv[] )
{
    Overture::start( argc, argv );  

    // number of processors:
    const int np   = max( 1, Communication_Manager::numberOfProcessors() ); 

    // my rank:
    const int myid = max( 0, Communication_Manager::My_Process_Number );

    return 0;
}


int getFileNameData(    int             argc, 
                        char            *argv[], 
                        aString         &nameOfOGFile, 
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
        printF( "Usage: gridShimIn gridName.hdf outfile.hdf \n" );   //////////
        Overture::abort( "error" );                                  //////////
    }                                                                //////////
                                                                     //////////
    const char *fileDir = "/home/overture/OvertureShim/textOutput/"; //////////
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
    printF( " Usage: gridShimIn gridName.hdf outfile.txt \n\n" );

    // initialize Overture 
    startOverture( argc, argv );
    

    /////////////////////////////////////////////////////////////////////////
    // Retrieve file names and save location ////////////////////////////////
    aString         nameOfOGFile;
    const char      *nameOfNewFile;
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
    
    // Read in a CompositeGrid data
    int status = getFromHDF5(       nameOfOGFile,
                                    &numOfComponentGrids,
                                    &numberOfDimensions, 
                                    interior_box, 
                                    domain_box, 
                                    xy, 
                                    desc );

    // Write grid data to text file
    status     = sendToTextFile(    saveLocation, 
                                    numOfComponentGrids,
                                    numberOfDimensions, 
                                    interior_box, 
                                    domain_box, 
                                    xy, 
                                    desc );


    printF( "Output written to file %s\n", nameOfNewFile );
    
    Overture::finish();     

    return 0;
}
