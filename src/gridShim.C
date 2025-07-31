// =================================================================================
//    Read a grid from a data base and output the data to a plain text file. 
//   
//  This routine can be changed to create an output file with any desired format.
//
// Example:
//     gridShimIn cic.hdf cic.out
//     mpirun -np 2 gridShimIn cic.hdf cic.out
// ==================================================================================

#include "inAndOut.h"
#include "inAndOutHDF5.h"
#include "HydeCompositeGrid.h"

#include <stdlib.h>
#include <iostream>



int startOverture( int argc, char *argv[] )
{
    Overture::start( argc, argv );  

    // number of processors:
    const int np   = max( 1, Communication_Manager::numberOfProcessors() ); 

    // my rank:
    const int myid = max( 0, Communication_Manager::My_Process_Number );

    return 0;
}



std::string getFileExtension(  const char*     fileName )
{
    std::string       fileNameStr( fileName );
    size_t pos      = fileNameStr.find_last_of( "." );

    if ( pos == std::string::npos ) 
    {
        printF( "Missing: file extension \n" ); 
        Overture::abort( "error" );  
    }                                                             

    return fileNameStr.substr( pos + 1 );
}



int getFileNameData(    int             argc, 
                        char            *argv[], 
                        const char**    nameOfOGFile, 
                        const char**    nameOfNewFile,
                        std::string*    ogFileExtension,
                        std::string*    newFileExtension,       
                        char            saveLocation[] )
{
    ///////////////////////////////////////////////////////////////////////////
    // Preperations for accessing and saving files ////////////////////////////
    if( argc == 3 )                                                                      
    {                                                                
        *nameOfOGFile    = argv[1];                                   
        *nameOfNewFile   = argv[2];                                   
    }                                                                                           
    else                                                             
    {                                                                
        printF( "Usage: gridShimIn gridName.hdf outfile.hdf \n" );   
        Overture::abort( "error" );                                  
    }                                                                


    // Get file extensions
    *ogFileExtension     = getFileExtension( *nameOfOGFile );
    *newFileExtension    = getFileExtension( *nameOfNewFile );


    const char *fileDir;
                                                                     
    if      ( *newFileExtension == "hdf" )                           
    {                                                                
        printF( "Reading from hdf file: %s \n", *nameOfOGFile );     
        fileDir = "/home/overture/overture-hyde-shim/hydeGrids/";        
    }                                                                
    else if ( *newFileExtension == "txt" )                             
    {                                                                
        printF( "Reading from txt file: %s \n", *nameOfOGFile );     
        fileDir = "/home/overture/overture-hyde-shim/textOutput/";        
    }   
    else                                                             
    {                                                                
        printF( "Error: File %s has no extension or is not a .hdf file \n", nameOfOGFile );   
        Overture::abort( "error" );                                  
    }                                                                

                                                                     
	// Comment
    strcpy( saveLocation, fileDir );                                 
    strcat( saveLocation, *nameOfNewFile );                          
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
    const char*     nameOfOGFile    	= NULL;
    const char*     nameOfNewFile   	= NULL;

    std::string*    ogFileExtension     = new std::string;
    std::string*    newFileExtension    = new std::string;

    char            saveLocation[100];

    getFileNameData(    argc, 
                        argv, 
                        &nameOfOGFile, 
                        &nameOfNewFile,
                        ogFileExtension,
                        newFileExtension,
                        saveLocation );
    /////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////


    // Initialize grid data 
    int                 	numOfComponentGrids;
    int                 	numberOfDimensions;


	HydeCompositeGrid 		*hydeCompositeGrid 		= new HydeCompositeGrid( nameOfOGFile, 1, 1 );


    ////////////////////////////////////////////////////////////////////////////////
    // Read grid data from file ////////////////////////////////////////////////////
    if ( *ogFileExtension == "hdf" )
    {
        // Read in a CompositeGrid data from hdf file
        int 	status 		= getFromHDF5(  nameOfOGFile,
											hydeCompositeGrid );
    }
    // else if ( *ogFileExtension == "txt" )
    // {
    //     // Read in a CompositeGrid data from txt file
    //     int 	status 		= getFromTextFile(  nameOfOGFile,
	// 											&numOfComponentGrids,
	// 											&numberOfDimensions, 
	// 											grid_index_range, 
	// 											ext_index_range,
	// 											bcs,
	// 											xy, 
	// 											desc );
    // }
    else
    {
        printF( "Error: File %s has no extension or is not a .hdf file \n", nameOfOGFile );
        Overture::abort( "error" );
    }
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////////////
    // Write grid data to file /////////////////////////////////////////////////////
    if ( *newFileExtension == "hdf" )
    {
        // Write grid data to hdf file
        sendToHDF5(         saveLocation, 
                            hydeCompositeGrid );
    }
    // else if ( *newFileExtension == "txt" )
    // {
    //     // Write grid data to text file
    //     sendToTextFile(     saveLocation, 
    //                         numOfComponentGrids,
    //                         numberOfDimensions, 
    //                         grid_index_range, 
    //                         ext_index_range,
	// 						bcs, 
    //                         xy, 
    //                         desc );
    // }
    else
    {
        printF( "Error: File %s has no extension or is not a .hdf file \n", nameOfNewFile );
        Overture::abort( "error" );
    }
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////


    printF( "Output written to file %s\n", nameOfNewFile );
    
    Overture::finish();     

    return 0;
}
