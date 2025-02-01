#include <iostream>
#include <fstream>
#include "inAndOut.h"


int sendToTextFile( const char*         fileName,
                    int                 numOfComponentGrids, 
                    int                 dimension, 
                    Array3D<int>        *interior_box, 
                    Array3D<int>        *domain_box, 
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

  outputFile << numOfComponentGrids << std::endl;
  outputFile << dimension << std::endl;

  for ( int gridIndex = 0; gridIndex < numOfComponentGrids; gridIndex++ )
  {
    std::cout   << interior_box -> data[ gridIndex ][ 0 ][ 0 ] << "\t" << interior_box -> data[ gridIndex ][ 0 ][ 1 ] << "\t" 
                << interior_box -> data[ gridIndex ][ 1 ][ 0 ] << "\t" << interior_box -> data[ gridIndex ][ 1 ][ 1 ] 
                << std::endl;

    outputFile  << interior_box -> data[ gridIndex ][ 0 ][ 0 ] << "\t" << interior_box -> data[ gridIndex ][ 0 ][ 1 ] << "\t" 
                << interior_box -> data[ gridIndex ][ 1 ][ 0 ] << "\t" << interior_box -> data[ gridIndex ][ 1 ][ 1 ] 
                << std::endl;
        
    outputFile  << domain_box -> data[ gridIndex ][ 0 ][ 0 ] << "\t" << domain_box -> data[ gridIndex ][ 0 ][ 1 ] << "\t" 
                << domain_box -> data[ gridIndex ][ 1 ][ 0 ] << "\t" << domain_box -> data[ gridIndex ][ 1 ][ 1 ] 
                << std::endl;
    
    for ( int i = domain_box -> data[ gridIndex ][ 0 ][ 0 ]; i <= domain_box -> data[ gridIndex ][ 1 ][ 0 ]; i++ )
    {
      for ( int j = domain_box -> data[ gridIndex ][ 0 ][ 1 ]; j <= domain_box -> data[ gridIndex ][ 1 ][ 1 ]; j++ )
      {
        for ( int k = 0; k < dimension; k++ )
        {
          outputFile << xy -> data[ gridIndex ][ i ][ j ][ k ] << "\t";
        }
      }
    }

    outputFile << std::endl;

    for ( int i = domain_box -> data[ gridIndex ][ 0 ][ 0 ]; i <= domain_box -> data[ gridIndex ][ 1 ][ 0 ]; i++ )
    {
      for ( int j = domain_box -> data[ gridIndex ][ 0 ][ 1 ]; j <= domain_box -> data[ gridIndex ][ 1 ][ 1 ]; j++ )
      {
        outputFile << mask -> data[ gridIndex ][ i ][ j ] << "\t";
      }
    }
    outputFile << "\n" << std::endl;
  }

  outputFile.close();
  return 0;
  
}


int getFromTextFile(  const char*         fileName,
                      int                 *numOfComponentGrids, 
                      int                 *dimension, 
                      Array3D<int>        *interior_box, 
                      Array3D<int>        *domain_box, 
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


  int numOfCompGrids, dim;

  inFile                >> numOfCompGrids;
  *numOfComponentGrids  =  numOfCompGrids;

  inFile                >> dim;
  *dimension            =  dim;


  interior_box      ->    allocate( numOfCompGrids, 2, dim,         -1, -1 ); 
  domain_box        ->    allocate( numOfCompGrids, 2, dim,         -1, -1 );
  xy                ->    allocate( numOfCompGrids, 0, 0, 0,        -1, -1, -1 );
  mask              ->    allocate( numOfCompGrids, 0, 0,           -1, -1 );

  for ( int gridIndex = 0; gridIndex < numOfCompGrids; gridIndex++ )
  {

    for ( int i = 0; i < 2; i++ )
    {
      for ( int j = 0; j < dim; j++ )
      {
        inFile >> interior_box -> data[ gridIndex ][ i ][ j ];
      }
    }

    for ( int i = 0; i < 2; i++ )
    {
      for ( int j = 0; j < dim; j++ )
      {
        inFile >> domain_box -> data[ gridIndex ][ i ][ j ];
      }
    }


    int nx  =   domain_box -> data[ gridIndex ][ 1 ][ 0 ] - domain_box -> data[ gridIndex ][ 0 ][ 0 ];
    int ny  =   domain_box -> data[ gridIndex ][ 1 ][ 1 ] - domain_box -> data[ gridIndex ][ 0 ][ 1 ];

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


    mask -> allocate( 0, ( nx + 1 ), ( ny + 1 ),        gridIndex, -1 );

    for( int i = 0; i < nx + 1; i++ )
    {
      for( int j = 0; j < ny + 1; j++ )
      {
        inFile >> mask -> data[ gridIndex ][ i ][ j ];
      }
    }
  }

  inFile.close();
  return 0;
}