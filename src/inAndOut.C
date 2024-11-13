#include <iostream>
#include <fstream>
#include "inAndOut.h"

int sendToFile(const char* fileName, int dim, int **interior_box, int **domain_box, double ***xy, int **mask){
  std::ofstream out;

  std::cout << "Arrived in sendToFile" <<std::endl;
  out.open( fileName );

  if( !out ){
    std::cerr << "Error: file '" << fileName << "' could not be opened!" << std::endl;
    std::exit(1);
  }

  out << dim << std::endl;

  std::cout << interior_box[0][0] << "\t" << interior_box[0][1] 
      << "\t" << interior_box[1][0] << "\t" << interior_box[1][1] 
      << std::endl;
  out << interior_box[0][0] << "\t" << interior_box[0][1] 
      << "\t" << interior_box[1][0] << "\t" << interior_box[1][1] 
      << std::endl;
  out << domain_box[0][0] << "\t" << domain_box[0][1] 
      << "\t" << domain_box[1][0] << "\t" << domain_box[1][1] 
      << std::endl;
  
  for ( int i = domain_box[0][0]; i <= domain_box[1][0]; i++ )
  {
    for ( int j = domain_box[0][1]; j <= domain_box[1][1]; j++ )
    {
      for ( int k = 0; k < dim; k++ ){
        out << xy[i][j][k] << "\t";
      }
    }
  }
  out << std::endl;

  for ( int i = domain_box[0][0]; i <= domain_box[1][0]; i++ )
  {
    for ( int j = domain_box[0][1]; j <= domain_box[1][1]; j++ )
    {
      out << mask[i][j] << "\t";
    }
  }
  out << std::endl;

  out.close();
  return 0;
  
}

int getFromFile(const char* fileName, int *dim, int **interior_box, int **domain_box, double **xy, int **mask){
  std::ifstream in;

  in.open(fileName);
  if(!in){
    std::cerr << "Error: file '" << fileName << "' could not be opened!" << std::endl;
    std::exit(1);
  }

  in >> *dim;
  *interior_box = ( int *) malloc(sizeof( int) * 2 * *dim);
  *domain_box   = ( int *) malloc(sizeof( int) * 2 * *dim);

  for ( int i = 0; i < 2; i++ )
  {
    for ( int j = 0; j < *dim; j++ )
    {
      in >> *( *interior_box + (2*i) + j );
    }
  }

  for ( int i = 0; i < 2; i++ )
  {
    for ( int j = 0; j < *dim; j++ )
    {
      in >> *( *domain_box + (2*i) + j );
    }
  }

  int nx = *( *domain_box + 2*1 + 0 ) - *( *domain_box + 2*0 + 0 );
  int ny = *( *domain_box + 2*1 + 1 ) - *( *domain_box + 2*0 + 1 );

  *xy = (double *) malloc(sizeof(double) *
                         ( nx + 1 ) *
                         ( ny + 1 ) *
                         *dim);

  double d;
  for( int i = 0; i < nx + 1; i++ )
  {
    for( int j = 0; j < ny + 1; j++ )
    {
      for( int k = 0; k < *dim; k++ )
      {
        in >> d;
        *( *xy + i * ( ny + 1 ) * 2 * 1
                    + j * 2 * 1
                    + k * 1 ) = d;
      }
    }
  }

  *mask = ( int *) malloc(sizeof(double) *
                         ( nx + 1 ) *
                         ( ny + 1 ) );

  for( int i = 0; i < nx + 1; i++ )
  {
    for( int j = 0; j < ny + 1; j++ )
    {
      in >> *( *mask + i * ( ny + 1 ) * 1 
                    + j * 1 );
    }
  }

  in.close();
  return 0;
}