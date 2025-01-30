#include <string>
#ifdef OLD_HEADER_FILENAME
#include <iostream.h>
#else
#include <iostream>
#endif

#include "H5Cpp.h"
#include "sendToHDF5.h"

#ifndef H5_NO_NAMESPACE
using namespace H5;
#endif


int sendToHDF5(   std::string   filename,
                  int           *spaceDim,
                  int           **interior_box,
                  int           **domain_box,
                  double        ***xy,
                  int           **mask )
{
  try
  {

    H5File* file = new H5File( filename, H5F_ACC_TRUNC );
    Group group( file->createGroup("/grid1") );

    // Create dataspace for dataset in file.
    DataSet   *dataset;

    // Set interior box ///////////////////////////////////////////////////////
    hsize_t     boxsize[] = {2, 2};
    DataSpace   boxspace( 2, boxsize );

    dataset = new DataSet( 
                            group.createDataSet(  "interior_box", 
                                                  PredType::NATIVE_INT, 
                                                  boxspace ) 
                         );
    dataset ->      write(  interior_box, 
                            PredType::NATIVE_INT );

    delete dataset;


    // Set dimensions ///////////////////////////////////////////////////////
    hsize_t     dimsize[] = {1};
    DataSpace   fspace( 1, dimsize );

    dataset = new DataSet( 
                            group.createDataSet(  "dim", 
                                                  PredType::NATIVE_INT, 
                                                  fspace ) 
                         );
    dataset ->      write(  spacedim, 
                            PredType::NATIVE_INT );

    delete dataset;


    // Set domain box ///////////////////////////////////////////////////////
    dataset = new DataSet( 
                            group.createDataSet(  "domain_box", 
                                                  PredType::NATIVE_INT, 
                                                  boxspace ) 
                         );
    dataset ->      write(  domain_box, 
                            PredType::NATIVE_INT );

    delete dataset;


    // Set xy ///////////////////////////////////////////////////////
    int nx = *(domain_box + 2*1 + 0) - *(domain_box + 2*0 + 0);
    int ny = *(domain_box + 2*1 + 1) - *(domain_box + 2*0 + 1);
    
    hsize_t   gridsize[] = {  static_cast<hsize_t>(nx + 1), 
                              static_cast<hsize_t>(ny + 1), 
                              2 };
    DataSpace xyspace(  ( *spacedim ) + 1, 
                        gridsize );
    
    dataset = new DataSet( 
                            group.createDataSet(  "xy", 
                                                  PredType::NATIVE_DOUBLE, 
                                                  xyspace ) 
                         );
    dataset ->      write(  xy, 
                            PredType::NATIVE_DOUBLE );

    delete dataset;


    // Set mask ///////////////////////////////////////////////////////
    DataSpace   maskspace( 2, gridsize );

    dataset = new DataSet( 
                            group.createDataSet(  "mask", 
                                                  PredType::NATIVE_INT, 
                                                  maskspace ) 
                         );
    dataset ->      write(  mask, 
                            PredType::NATIVE_INT );

    delete dataset;


    file->close();
  } 

   // catch failure caused by the DataSet operations
  catch( DataSetIException error )
  {
    std::cout << error.getDetailMsg() << std::endl;
    std::cout << "dataset exception" << std::endl;
    error.printErrorStack();
    return -1;
  }

  // catch failure caused by the DataSpace operations
  catch( DataSpaceIException error )
  {
    std::cout << error.getDetailMsg() << std::endl;
    std::cout << "dataspace exception" << std::endl;
    error.printErrorStack();
    return -1;
  }

  return 0;
}



int getFromHDF5(  const char*   fileName, 
                  int           *dim, 
                  int           **interior_box, 
                  int           **domain_box, 
                  double        **xy, 
                  int           **mask )
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
            gridIndex  , (const char*)grid.getName(),
            dim( 0, 0 )           , dim( 1, 0 )           , dim( 0, 1 ), 
            dim( 1, 1 )           , dim( 0, 2 )           , dim( 1, 2 ),
            gridIndexRange( 0, 0 ), gridIndexRange( 1, 0 ), gridIndexRange( 0, 1 ), 
            gridIndexRange( 1, 1 ), gridIndexRange( 0, 2 ), gridIndexRange( 1, 2 ),
            bc( 0, 0 ), bc( 1, 0 ), bc( 0, 1 ), bc( 1, 1 ), bc( 0, 2 ), bc( 1, 2 ),
            grid.isPeriodic(0)    , grid.isPeriodic(1)    , grid.isPeriodic(2) );
    
    

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
    {
      for ( int j = 0; j < numberOfDimensions; j++ )
      {
        interior_box[ i ][ j ] = gridIndexRange( i, j ) - dim( 0, j );
        domain_box[ i ][ j ]   = dim( i, j ) - dim( 0, j );
      }
    }

    // Note that k below is just 0, not a real loop index. Only one Z index.
    int     **desc  = (int **)      malloc( sizeof(int   *)  * ( nx + 1 ) );
    double  ***xy   = (double ***)  malloc( sizeof(double**) * ( nx + 1 ) );
    
    for( int i = 0; i < nx + 1; i++ )
    {
      *( desc + i ) = (int *)       malloc( sizeof(int   *) * ( ny + 1 ) );
      *( xy + i )   = (double **)   malloc( sizeof(double*) * ( ny + 1 ) );

      for( int j = 0; j < ny + 1; j++ )
      {
        desc[ i ][ j ] = maskLocal( i + dim( 0, 0 ), j + dim( 0, 1 ), k ) >= 0 ? 0: 1;
        *( *( xy + i ) + j ) = (double *) malloc( sizeof(double) * numberOfDimensions );

        for( int l = 0; l < numberOfDimensions; l++ )
        {
          xy[ i ][ j ][ l ] = vertexLocal( i + dim( 0, 0 ), j + dim( 0, 1 ), k + dim( 0, 2 ), l );
        }

      }
    }
    std::cout << interior_box[0][0] << interior_box[0][1] 
      << interior_box[1][0] << interior_box[1][1] 
      << std::endl;
  }
}