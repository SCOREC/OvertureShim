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
  Overture::start( argc, argv );  // initialize Overture
  const int np   = max( 1, Communication_Manager::numberOfProcessors() );  // number of processors
  const int myid = max( 0, Communication_Manager::My_Process_Number );    // my rank 

  printF( " Usage: gridShimIn gridName.hdf outfile.txt \n" );

  aString nameOfOGFile;
  const char *fileName;
  if( argc == 3 )
  {
    nameOfOGFile = argv[1];
    fileName     = argv[2];
  }
  else
  {
    printF( "Usage: gridShimIn gridName.hdf outfile.hdf \n" );
    Overture::abort( "error" );
  }
  
  const char *fileDir = "/home/overture/shim/textOutputs/";

  char saveLocation[100];
  strcpy( saveLocation, fileDir );
  strcat( saveLocation, fileName );
  strcat( saveLocation, ".txt" );


  // Read in a CompositeGrid
  CompositeGrid compositeGrid;
  getFromADataBase( compositeGrid, nameOfOGFile );
  const int numberOfDimensions = compositeGrid.numberOfDimensions();
  
  const IntegerArray & ni = compositeGrid.numberOfInterpolationPoints;
  
  for ( int gridIndex = 0; gridIndex < compositeGrid.numberOfComponentGrids(); gridIndex++ )
  {
    MappedGrid & grid = compositeGrid[ gridIndex ];
    grid.update( MappedGrid::THEvertex | MappedGrid::THEmask );  // create the vertex and mask arrays

    const IntegerArray & dim            = grid.dimension();
    const IntegerArray & gridIndexRange = grid.gridIndexRange();
    const IntegerArray & bc             = grid.boundaryCondition();  // unused for now


    printf( "%i %s (grid and name)\n"
	    "%i %i %i %i %i %i ( dimension(0:1,0:2), array dimensions )\n"
	    "%i %i %i %i %i %i ( gridIndexRange(0:1,0:2), grid bounds )\n"
	    "%i %i %i %i %i %i ( boundaryCondition(0:1,0:2) )\n"
	    "%i %i %i          ( isPeriodic(0:2), 0 = not, 1 = deriv, 2 = function )\n",
	    gridIndex,(const char*)grid.getName(),
	    dim( 0, 0 ), dim( 1, 0 ), dim( 0, 1 ), dim( 1, 1 ), dim( 0, 2 ), dim( 1, 2 ),
	    gridIndexRange( 0, 0 ), gridIndexRange( 1, 0 ), gridIndexRange( 0, 1 ), 
        gridIndexRange( 1, 1 ), gridIndexRange( 0, 2 ), gridIndexRange( 1, 2 ),
	    bc( 0, 0 ), bc( 1, 0 ), bc( 0, 1 ), bc( 1, 1 ), bc( 0, 2 ), bc( 1, 2 ),
	    grid.isPeriodic(0), grid.isPeriodic(1), grid.isPeriodic(2) );
    
    

    const intArray & mask = grid.mask();
    intSerialArray maskLocal; 
    getLocalArrayWithGhostBoundaries( mask, maskLocal );          // local array on this processor
    const realArray & vertex = grid.vertex();
    realSerialArray vertexLocal; 
    getLocalArrayWithGhostBoundaries( vertex, vertexLocal );          // local array on this processor

    int nx = dim( 1, 0 ) - dim( 0, 0 );
    int ny = dim( 1, 1 ) - dim( 0, 1 );
    int k  = vertexLocal.getBase( 3 );

    int **interior_box = (int **) malloc( sizeof(int *) * 2 );
    interior_box[0]    = (int *) malloc( sizeof(int) * numberOfDimensions );
    interior_box[1]    = (int *) malloc( sizeof(int) * numberOfDimensions );
    int **domain_box   = (int **) malloc( sizeof(int *) * 2 );
    domain_box[0]      = (int *) malloc( sizeof(int) * numberOfDimensions );
    domain_box[1]      = (int *) malloc( sizeof(int) * numberOfDimensions );

    for ( int i = 0; i < 2; i++ )
      for ( int j = 0; j < numberOfDimensions; j++ )
      {
        interior_box[i][j] = gridIndexRange( i, j ) - dim( 0, j );
        domain_box[i][j]   = dim( i, j ) - dim( 0, j );
      }

    // Note that k below is just 0, not a real loop index. Only one Z index.
    int  **desc  = (int **) malloc( sizeof(int   *) * ( nx + 1 ) );
    double ***xy = (double ***) malloc( sizeof(double**) * ( nx + 1 ) );
    
    for( int i = 0; i < nx + 1; i++ )
    {
      *( desc + i ) = (int *) malloc( sizeof(int   *) * ( ny + 1 ) );
      *( xy + i )   = (double **) malloc( sizeof(double*) * ( ny + 1 ) );
      for( int j = 0; j < ny + 1; j++ )
      {
        desc[i][j] = maskLocal( i + dim( 0, 0 ), j + dim( 0, 1 ), k ) >= 0 ? 0: 1;
        *( *( xy + i ) + j ) = (double *) malloc( sizeof(double) * numberOfDimensions );
        for( int l = 0; l < numberOfDimensions; l++ )
        {
          xy[i][j][l] = vertexLocal( i + dim( 0, 0 ), j + dim( 0, 1 ), k + dim( 0, 2 ), l );
        }

      }
    }
    std::cout << interior_box[0][0] << interior_box[0][1] 
      << interior_box[1][0] << interior_box[1][1] 
      << std::endl;

    int status = sendToTextFile(  saveLocation, 
                                  numberOfDimensions, 
                                  interior_box, 
                                  domain_box, 
                                  xy, 
                                  desc );

    grid.destroy( MappedGrid::THEvertex | MappedGrid::THEmask );  // destroy arrays to save space

  }
  printF( "Output written to file %s\n", fileName );
  
  Overture::finish();          
  return 0;
}
