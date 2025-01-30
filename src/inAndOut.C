#include <iostream>
#include <fstream>
#include "inAndOut.h"


int sendToTextFile( const char*   fileName, 
                    int           dim, 
                    int           **interior_box, 
                    int           **domain_box, 
                    double        ***xy, 
                    int           **mask )
{
  std::ofstream   outputFile;

  std::cout << "Arrived in sendToTextFile" << std::endl;
  outputFile.open( fileName );

  if( !outputFile ){
    std::cerr << "Error: file '" << fileName << "' could not be opened!" << std::endl;
    std::exit( 1 );
  }

  outputFile << dim << std::endl;

  std::cout   << interior_box[ 0 ][ 0 ] << "\t" << interior_box[ 0 ][ 1 ] << "\t" 
              << interior_box[ 1 ][ 0 ] << "\t" << interior_box[ 1 ][ 1 ] 
              << std::endl;

  outputFile  << interior_box[ 0 ][ 0 ] << "\t" << interior_box[ 0 ][ 1 ] << "\t" 
              << interior_box[ 1 ][ 0 ] << "\t" << interior_box[ 1 ][ 1 ] 
              << std::endl;
      
  outputFile  << domain_box[ 0 ][ 0 ] << "\t" << domain_box[ 0 ][ 1 ] << "\t" 
              << domain_box[ 1 ][ 0 ] << "\t" << domain_box[ 1 ][ 1 ] 
              << std::endl;
  
  for ( int i = domain_box[ 0 ][ 0 ]; i <= domain_box[ 1 ][ 0 ]; i++ )
  {
    for ( int j = domain_box[ 0 ][ 1 ]; j <= domain_box[ 1 ][ 1 ]; j++ )
    {
      for ( int k = 0; k < dim; k++ )
      {
        outputFile << xy[ i ][ j ][ k ] << "\t";
      }
    }
  }

  outputFile << std::endl;

  for ( int i = domain_box[ 0 ][ 0 ]; i <= domain_box[ 1 ][ 0 ]; i++ )
  {
    for ( int j = domain_box[ 0 ][ 1 ]; j <= domain_box[ 1 ][ 1 ]; j++ )
    {
      outputFile << mask[ i ][ j ] << "\t";
    }
  }
  outputFile << std::endl;

  outputFile.close();
  return 0;
  
}


int getFromTextFile(  const char*   fileName, 
                      int           *dim, 
                      int           **interior_box, 
                      int           **domain_box, 
                      double        ***xy, 
                      int           **mask )
{
  std::ifstream inFile;

  inFile.open( fileName );
  if( !inFile )
  {
    std::cerr << "Error: file '" << fileName << "' could not be opened!" << std::endl;
    std::exit( 1 );
  }

  inFile >> *dim;
  interior_box      = (int **) malloc( sizeof(int *) * 2 );
  interior_box[0]   = (int *)  malloc( sizeof(int) * (*dim) );
  interior_box[1]   = (int *)  malloc( sizeof(int) * (*dim) );

  domain_box        = (int **) malloc( sizeof(int *) * 2 );
  domain_box[0]     = (int *)  malloc( sizeof(int) * (*dim) );
  domain_box[1]     = (int *)  malloc( sizeof(int) * (*dim) );

  for ( int i = 0; i < 2; i++ )
  {
    for ( int j = 0; j < *dim; j++ )
    {
      inFile >> interior_box[ i ][ j ];
    }
  }

  for ( int i = 0; i < 2; i++ )
  {
    for ( int j = 0; j < *dim; j++ )
    {
      inFile >> domain_box[ i ][ j ];
    }
  }


  int nx  =   domain_box[ 1 ][ 0 ] - domain_box[ 0 ][ 0 ];
  int ny  =   domain_box[ 1 ][ 1 ] - domain_box[ 0 ][ 1 ];

  xy = ( double *** ) malloc( sizeof(double) * ( nx + 1 ) );

  double d;
  for( int i = 0; i < nx + 1; i++ )
  {
    xy[ i ]   = (double **) malloc( sizeof(double*) * ( ny + 1 ) );

    for( int j = 0; j < ny + 1; j++ )
    {
      xy[ i ][ j ] = (double *) malloc( sizeof(double) * (*dim) );

      for( int k = 0; k < *dim; k++ )
      {
        inFile >> d;
        xy[ i ][ j ][ k ] = d;
      }
    }
  }


  mask = ( int **) malloc(  sizeof(double) * ( nx + 1 ) );

  for( int i = 0; i < nx + 1; i++ )
  {
    mask[ i ] = (int *) malloc( sizeof(int) * ( ny + 1 ) );
    
    for( int j = 0; j < ny + 1; j++ )
    {
      inFile >> mask[ i ][ j ];
    }
  }

  inFile.close();
  return 0;
}