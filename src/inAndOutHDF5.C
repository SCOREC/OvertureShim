#include <string>
#ifdef OLD_HEADER_FILENAME
#include <iostream.h>
#else
#include <iostream>
#endif

#include "inAndOutHDF5.h"
#include "H5Cpp.h"


#ifndef H5_NO_NAMESPACE
using namespace H5;
#endif

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



int sendToHDF5(   std::string     nameOfNewFile,
                  int             *numberOfComponentGrids, 
                  int             *numberOfDimensions,
                  Array3D<int>    *interior_box, 
                  Array3D<int>    *domain_box, 
                  Array4D<double> *xy,
                  Array3D<int>    *desc )
{
  try
  {

    H5File* file = new H5File( nameOfNewFile, H5F_ACC_TRUNC );
    

    // Create a group for the composite grid in the file.
    Group compGridGroup( file->createGroup( "/composite_grid") );

    // Create dataspace for dataset in file.
    DataSet   *dataset;


    ////////////////////////////////////////////////////////////////////////////////////////
    // Set dimensions //////////////////////////////////////////////////////////////////////
    hsize_t     dimsize[] = {1};
    DataSpace   fspace( 1, dimsize );

    dataset = new DataSet( 
                            compGridGroup.createDataSet(  "dim", 
                                                          PredType::NATIVE_INT, 
                                                          fspace ) 
                    );
    dataset ->      write(  numberOfDimensions, 
                            PredType::NATIVE_INT );

    delete dataset;
    /////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////
    

    ////////////////////////////////////////////////////////////////////////////////////////
    // Set numberOfComponentGrids ///////////////////////////////////////////////////////
    
    hsize_t     numOfGridsIntSize[] = {1};
    DataSpace   fspace( 1, numOfGridsIntSize );

    dataset = new DataSet( 
                            compGridGroup.createDataSet(  "num_of_component_grids", 
                                                          PredType::NATIVE_INT, 
                                                          fspace ) 
                    );
    dataset ->      write(  numberOfComponentGrids, 
                            PredType::NATIVE_INT );

    delete dataset;
    /////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////


    for ( int gridIndex = 0; gridIndex < *numberOfComponentGrids; gridIndex++ )
    {
      // Create a grid group in the file.
      Group group( compGridGroup.createGroup( "grid" + std::to_string( gridIndex ) ) );


      // Set interior box ///////////////////////////////////////////////////////
      hsize_t     boxsize[] = {2, 2};
      DataSpace   boxspace( 2, boxsize );

      dataset = new DataSet( 
                              group.createDataSet(  "interior_box", 
                                                    PredType::NATIVE_INT, 
                                                    boxspace ) 
                          );
      dataset ->      write(  interior_box -> data[ gridIndex ], 
                              PredType::NATIVE_INT );

      delete dataset;
      /////////////////////////////////////////////////////////////////////////////////////

      // Set domain box ///////////////////////////////////////////////////////
      dataset = new DataSet( 
                              group.createDataSet(  "domain_box", 
                                                    PredType::NATIVE_INT, 
                                                    boxspace ) 
                          );
      dataset ->      write(  domain_box -> data[ gridIndex ], 
                              PredType::NATIVE_INT );

      delete dataset;
      /////////////////////////////////////////////////////////////////////////////////////

      // Set xy ///////////////////////////////////////////////////////
      int nx = domain_box[ 1 ][ 0 ] - domain_box[ 0 ][ 0 ];
      int ny = domain_box[ 1 ][ 1 ] - domain_box[ 0 ][ 1 ];
      
      hsize_t   gridsize[] = {  static_cast<hsize_t>(nx + 1), 
                                static_cast<hsize_t>(ny + 1), 
                                2 };
      DataSpace xyspace(  ( *numberOfDimensions ) + 1, 
                          gridsize );
      
      dataset = new DataSet( 
                              group.createDataSet(  "xy", 
                                                    PredType::NATIVE_DOUBLE, 
                                                    xyspace ) 
                          );
      dataset ->      write(  xy -> data[ gridIndex ], 
                              PredType::NATIVE_DOUBLE );

      delete dataset;
      /////////////////////////////////////////////////////////////////////////////////////

      // Set mask ///////////////////////////////////////////////////////////////
      DataSpace   maskspace( 2, gridsize );

      dataset = new DataSet( 
                              group.createDataSet(  "mask", 
                                                    PredType::NATIVE_INT, 
                                                    maskspace ) 
                          );
      dataset ->      write(  mask -> data[ gridIndex ], 
                              PredType::NATIVE_INT );

      delete dataset;
      /////////////////////////////////////////////////////////////////////////////////////
    }


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



int getFromHDF5(    aString         nameOfOGFile, 
                    int             *numberOfComponentGrids, 
                    int             *numberOfDimensions,
                    Array3D<int>    *interior_box, 
                    Array3D<int>    *domain_box, 
                    Array4D<double> *xy,
                    Array3D<int>    *desc )
{   
  // Read in a CompositeGrid
  CompositeGrid compositeGrid;
  //aString nameOfOGFile;
  getFromADataBase( compositeGrid, nameOfOGFile );
  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////

  // initialize first dimension of grid data ///////////////////////////////////
  int numOfCompGrids    = compositeGrid.numberOfComponentGrids();
  *numberOfComponentGrids  = numOfCompGrids;

  int numOfDimensions   = compositeGrid.numberOfDimensions();
  *numberOfDimensions   = numOfDimensions;


  interior_box    ->    allocate( numOfCompGrids, 2, numOfDimensions,   -1, -1 );
  domain_box      ->    allocate( numOfCompGrids, 2, numOfDimensions,   -1, -1 );
  xy              ->    allocate( numOfCompGrids, 0, 0, 0,              -1, -1, -1 );
  desc            ->    allocate( numOfCompGrids, 0, 0,                 -1, -1 );
  ///////////////////////////////////////////////////////////////////////////////

  
  
  const IntegerArray & ni = compositeGrid.numberOfInterpolationPoints;
  
  for ( int gridIndex = 0; gridIndex < numOfCompGrids; gridIndex++ )
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
            gridIndex             , (const char*)grid.getName(),
            dim( 0, 0 )           , dim( 1, 0 )           , dim( 0, 1 ), 
            dim( 1, 1 )           , dim( 0, 2 )           , dim( 1, 2 ),
            gridIndexRange( 0, 0 ), gridIndexRange( 1, 0 ), gridIndexRange( 0, 1 ), 
            gridIndexRange( 1, 1 ), gridIndexRange( 0, 2 ), gridIndexRange( 1, 2 ),
            bc( 0, 0 ), bc( 1, 0 ), bc( 0, 1 ), bc( 1, 1 ), bc( 0, 2 ), bc( 1, 2 ),
            grid.isPeriodic(0)    , grid.isPeriodic(1)    , grid.isPeriodic(2) );
    
    

    const intArray & mask = grid.mask();
    intSerialArray maskLocal; 

    // local array on this processor
    getLocalArrayWithGhostBoundaries( mask, maskLocal );  
    const realArray & vertex = grid.vertex();
    realSerialArray vertexLocal; 

    // local array on this processor
    getLocalArrayWithGhostBoundaries( vertex, vertexLocal );          
    int nx = dim( 1, 0 ) - dim( 0, 0 );
    int ny = dim( 1, 1 ) - dim( 0, 1 );
    int k  = vertexLocal.getBase( 3 );


    for ( int i = 0; i < 2; i++ )
    {
      for ( int j = 0; j < numOfDimensions; j++ )
      {
        interior_box -> data[ gridIndex ][ i ][ j ] = gridIndexRange( i, j ) - dim( 0, j );
        domain_box   -> data[ gridIndex ][ i ][ j ] = dim( i, j ) - dim( 0, j );
      }
    }

    // Note that k below is just 0, not a real loop index. Only one Z index.
    desc  -> allocate( 0, ( nx + 1 ), ( ny + 1 ),                       gridIndex, -1 );
    xy    -> allocate( 0, ( nx + 1 ), ( ny + 1 ), numOfDimensions,      gridIndex, -1, -1 );
    
    for( int i = 0; i < nx + 1; i++ )
    {
      for( int j = 0; j < ny + 1; j++ )
      {
        desc -> data[ gridIndex ][ i ][ j ]    = maskLocal( i + dim( 0, 0 ), j + dim( 0, 1 ), k ) >= 0 ? 0: 1;

        for( int l = 0; l < numOfDimensions; l++ )
        {
          xy -> data[ gridIndex ][ i ][ j ][ l ] = vertexLocal( i + dim( 0, 0 ), j + dim( 0, 1 ), k + dim( 0, 2 ), l );
        }

      }
    }
    std::cout << interior_box -> data[ gridIndex ][ 0 ][ 0 ] << interior_box -> data[ gridIndex ][ 0 ][ 1 ] 
              << interior_box -> data[ gridIndex ][ 1 ][ 0 ] << interior_box -> data[ gridIndex ][ 1 ][ 1 ] 
              << std::endl;

    // destroy arrays to save space
    grid.destroy( MappedGrid::THEvertex | MappedGrid::THEmask );  
  }

  return 0;
}