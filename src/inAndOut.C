#include <iostream>
#include <fstream>
#include "inAndOut.h"


int sendToTextFile(   	const char*         fileName,
						int                 numOfComponentGrids, 
						int                 dimension, 
						Array3D<int>        *grid_index_range, 
                      	Array3D<int>        *ext_index_range,
                  		Array3D<int>    	*bcs, 
                      	Array4D<double>     *xy,  
                      	Array3D<int>        *mask )
{
  std::ofstream   outputFile;

  std::cout << "Arrived in sendToTextFile" << std::endl;
  outputFile.open( fileName );

  if( !outputFile )
  {
    std::cerr << "Error: file '" << fileName << "' could not be opened!" << std::endl;
    std::exit( 1 );
  }

  // Send number of component grids and dimension to file ////////////////////////////////////
  outputFile << numOfComponentGrids << std::endl;
  outputFile << dimension << std::endl;
  ///////////////////////////////////////////////////////////////////////////////////////////

  for ( int gridIndex = 0; gridIndex < numOfComponentGrids; gridIndex++ )
  {
    // Send grid_index_range to file //////////////////////////////////////////////////////////////
    std::cout   << grid_index_range -> data[ gridIndex ][ 0 ][ 0 ] << "\t" << grid_index_range -> data[ gridIndex ][ 0 ][ 1 ] << "\n" 
                << grid_index_range -> data[ gridIndex ][ 1 ][ 0 ] << "\t" << grid_index_range -> data[ gridIndex ][ 1 ][ 1 ] << "\n" 
                << std::endl;

    outputFile  << grid_index_range -> data[ gridIndex ][ 0 ][ 0 ] << "\t" << grid_index_range -> data[ gridIndex ][ 0 ][ 1 ] << "\t" 
                << grid_index_range -> data[ gridIndex ][ 1 ][ 0 ] << "\t" << grid_index_range -> data[ gridIndex ][ 1 ][ 1 ] 
                << std::endl;
    /////////////////////////////////////////////////////////////////////////////////////////
        
    // Send ext_index_range to file ////////////////////////////////////////////////////////////////
    outputFile  << ext_index_range -> data[ gridIndex ][ 0 ][ 0 ] << "\t" << ext_index_range -> data[ gridIndex ][ 0 ][ 1 ] << "\t" 
                << ext_index_range -> data[ gridIndex ][ 1 ][ 0 ] << "\t" << ext_index_range -> data[ gridIndex ][ 1 ][ 1 ] 
                << std::endl;
    /////////////////////////////////////////////////////////////////////////////////////////
        
    // Send bcs to file /////////////////////////////////////////////////////////////////////
    outputFile  << bcs -> data[ gridIndex ][ 0 ][ 0 ] << "\t" << bcs -> data[ gridIndex ][ 0 ][ 1 ] << "\t" 
                << bcs -> data[ gridIndex ][ 1 ][ 0 ] << "\t" << bcs -> data[ gridIndex ][ 1 ][ 1 ] 
                << std::endl;
    /////////////////////////////////////////////////////////////////////////////////////////
    
    // Send xy to file ///////////////////////////////////////////////////////////////////////
    for ( int i = 0; i < xy -> dim2[ gridIndex ]; i++ )
    {
      for ( int j = 0; j < xy -> dim3[ gridIndex ]; j++ )
      {
        for ( int k = 0; k < xy -> dim4[ gridIndex ]; k++ )
        {
          outputFile << xy -> data[ gridIndex ][ i ][ j ][ k ] << "\t";
        }
      }
    }

    outputFile << std::endl;
    /////////////////////////////////////////////////////////////////////////////////////////

    // Send mask to file /////////////////////////////////////////////////////////////////////
    for ( int i = 0; i < mask -> dim2[ gridIndex ]; i++ )
    {
      for ( int j = 0; j < mask -> dim3[ gridIndex ]; j++ )
      {
        outputFile << mask -> data[ gridIndex ][ i ][ j ] << "\t";
      }
    }

    outputFile << std::endl;
    /////////////////////////////////////////////////////////////////////////////////////////
  }

  outputFile.close();
  return 0;
  
}


int getFromTextFile(  const char*         fileName,
                      int                 *numOfComponentGrids, 
                      int                 *dimension, 
                      Array3D<int>        *grid_index_range, 
                      Array3D<int>        *ext_index_range,
                      Array3D<int>        *bcs, 
                      Array4D<double>     *xy, 
                      Array3D<int>        *mask )
{
  std::ifstream inFile;

  inFile.open( fileName );
  if( !inFile )
  {
    std::cerr << "Error: file '" << fileName << "' could not be opened!" << std::endl;
    std::exit( 1 );
  }

  // Read in number of component grids and dimension /////////////////////////////////////////
  int numOfCompGrids, dim;

  inFile                >> numOfCompGrids;
  *numOfComponentGrids  =  numOfCompGrids;

  inFile                >> dim;
  *dimension            =  dim;
  ///////////////////////////////////////////////////////////////////////////////////////////////

  // Allocate space where possible for grid data
  grid_index_range      ->    allocate( numOfCompGrids, 2, dim,         -1, -1 ); 
  ext_index_range        ->    allocate( numOfCompGrids, 2, dim,         -1, -1 );
  xy                ->    allocate( numOfCompGrids, 0, 0, 0,        -1, -1, -1 );
  mask              ->    allocate( numOfCompGrids, 0, 0,           -1, -1 );

  // Read in grid_index_range /////////////////////////////////////////////////////////////////
  for ( int gridIndex = 0; gridIndex < numOfCompGrids; gridIndex++ )
  {

    for ( int i = 0; i < 2; i++ )
    {
      for ( int j = 0; j < dim; j++ )
      {
        inFile >> grid_index_range -> data[ gridIndex ][ i ][ j ];
      }
    }
    /////////////////////////////////////////////////////////////////////////////////

    // Read in ext_index_range ////////////////////////////////////////////////////////////
    for ( int i = 0; i < 2; i++ )
    {
      for ( int j = 0; j < dim; j++ )
      {
        inFile >> ext_index_range -> data[ gridIndex ][ i ][ j ];
      }
    }
    /////////////////////////////////////////////////////////////////////////////////

    // Read in xy ///////////////////////////////////////////////////////////////////////
    int nx  =   ext_index_range -> data[ gridIndex ][ 1 ][ 0 ] - ext_index_range -> data[ gridIndex ][ 0 ][ 0 ];
    int ny  =   ext_index_range -> data[ gridIndex ][ 1 ][ 1 ] - ext_index_range -> data[ gridIndex ][ 0 ][ 1 ];

    xy  -> allocate( 0, ( nx + 1 ), ( ny + 1 ), dim,      gridIndex, -1, -1 );

    double d;
    for( int i = 0; i < nx + 1; i++ )
    {
      for( int j = 0; j < ny + 1; j++ )
      {
        for( int k = 0; k < dim; k++ )
        {
          inFile >> d;
          xy -> data[ gridIndex ][ i ][ j ][ k ] = d;
        }
      }
    }
    ///////////////////////////////////////////////////////////////////////////////////

    // Read in mask //////////////////////////////////////////////////////////////////////
    mask -> allocate( 0, ( nx + 1 ), ( ny + 1 ),        gridIndex, -1 );

    for( int i = 0; i < nx + 1; i++ )
    {
      for( int j = 0; j < ny + 1; j++ )
      {
        inFile >> mask -> data[ gridIndex ][ i ][ j ];
      }
    }
    //////////////////////////////////////////////////////////////////////////////////
  }

  inFile.close();
  return 0;
}