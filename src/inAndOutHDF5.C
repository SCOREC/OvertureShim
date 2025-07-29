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
                  int             numberOfComponentGrids, 
                  int             numberOfDimensions,
                  Array3D<int>    *interior_box, 
                  Array3D<int>    *domain_box, 
                  Array4D<double> *xy,
                  Array3D<int>    *mask )
{
	try
	{

		H5File* 			file 							= new H5File( nameOfNewFile, H5F_ACC_TRUNC );
		

		// Create a group for the composite grid in the file.
		Group 				compGridGroup( file->createGroup( "/composite_grid") );

		// Create dataspace for dataset in file.
		DataSet   			*dataset;


		////////////////////////////////////////////////////////////////////////////////////////
		// Set dimensions //////////////////////////////////////////////////////////////////////
		hsize_t     		dimsize[] 						= {1};
		DataSpace   		dimSpace( 1, dimsize );

		dataset = new DataSet( 
								compGridGroup.createDataSet(  "dim", 
															PredType::NATIVE_INT, 
															dimSpace ) 
						);
		dataset ->      write(  &numberOfDimensions, 
								PredType::NATIVE_INT );

		delete 				dataset;
		////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////
		

		////////////////////////////////////////////////////////////////////////////////////////
		// Set numberOfComponentGrids //////////////////////////////////////////////////////////
		
		hsize_t     		numOfGridsIntSize[] 						= {1};
		DataSpace   		numGridSpace( 1, numOfGridsIntSize );

		dataset 														= new 	DataSet( 
																							compGridGroup.createDataSet(  	"num_of_component_grids", 
																															PredType::NATIVE_INT, 
																															numGridSpace ) 
																						);
		dataset ->      write(  &numberOfComponentGrids, 
								PredType::NATIVE_INT );

		delete 				dataset;
		////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////


		for ( int gridIndex = 0; gridIndex < numberOfComponentGrids; gridIndex++ )
		{
			// Create a grid group in the file.
			Group 			group( compGridGroup.createGroup( "grid_" + std::to_string( gridIndex ) ) );


			/////////////////////////////////////////////////////////////////////////////////////
			// Set interior box /////////////////////////////////////////////////////////////////
			hsize_t     	boxsize[] 					= {2, 2};
			DataSpace   	boxspace( 2, boxsize );

			dataset 									= new 	DataSet( 
																			group.createDataSet(  	"interior_box", 
																									PredType::NATIVE_INT, 
																									boxspace ) 
																		);
			dataset ->      write(  interior_box -> data[ gridIndex ], 
									PredType::NATIVE_INT );

			delete 			dataset;
			/////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////


			/////////////////////////////////////////////////////////////////////////////////////
			// Set domain box ///////////////////////////////////////////////////////////////////
			dataset 									= new 	DataSet( 
																			group.createDataSet(  	"domain_box", 
																									PredType::NATIVE_INT, 
																									boxspace ) 
																		);

			dataset ->      write(  domain_box -> data[ gridIndex ], 
									PredType::NATIVE_INT );

			delete 			dataset;
			/////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////


			/////////////////////////////////////////////////////////////////////////////////////
			// Set xy ///////////////////////////////////////////////////////////////////////////
			int 			nx  						=   	domain_box -> data[ gridIndex ][ 1 ][ 0 ] 
															- 	domain_box -> data[ gridIndex ][ 0 ][ 0 ];
			int 			ny  						=   	domain_box -> data[ gridIndex ][ 1 ][ 1 ] 
															- 	domain_box -> data[ gridIndex ][ 0 ][ 1 ];
			
			hsize_t   		gridsize1[] 				= { static_cast<hsize_t> ( nx + 1 ), 
															static_cast<hsize_t> ( ny + 1 ), 
															static_cast<hsize_t> ( numberOfDimensions ) };

			DataSpace 		xyspace(  	numberOfDimensions + 1, 
										gridsize1 );
			
			dataset 									= new 	DataSet( 
																			group.createDataSet(  	"xy", 
																									PredType::NATIVE_DOUBLE, 
																									xyspace ) 
																		);


			dataset ->      write(  xy -> data[ gridIndex ], 
									PredType::NATIVE_DOUBLE );

			delete 			dataset;
			/////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////


			/////////////////////////////////////////////////////////////////////////////////////
			// Set mask /////////////////////////////////////////////////////////////////////////
			hsize_t   		gridsize2[] 				= { static_cast<hsize_t>(nx + 1), 
															static_cast<hsize_t>(ny + 1) };

			DataSpace   	maskspace( 2, gridsize2 );

			dataset 									= new 	DataSet( 
																			group.createDataSet(  	"mask", 
																									PredType::NATIVE_INT, 
																									maskspace ) 
																		);


			dataset ->      write(  mask -> data[ gridIndex ], 
									PredType::NATIVE_INT );

			delete 			dataset;
			/////////////////////////////////////////////////////////////////////////////////////
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



int getFromHDF5(    const char*     fileName, 
                    int             *numberOfComponentGrids, 
                    int             *numberOfDimensions,
                    Array3D<int>    *interior_box, 
                    Array3D<int>    *domain_box, 
                    Array4D<double> *xy,
                    Array3D<int>    *desc )
{   
	// Read in a CompositeGrid
	aString 					nameOfOGFile      		= fileName;
	CompositeGrid             	compositeGrid;
	getFromADataBase( compositeGrid, nameOfOGFile );


	/////////////////////////////////////////////////////////////////////////////////
	// Initialize number of component grids and dimension of grid data /////////////
	int 		numOfGrids        		= compositeGrid.numberOfComponentGrids();
	*numberOfComponentGrids   			= numOfGrids;

	int 		numOfDimensions       	= compositeGrid.numberOfDimensions();
	*numberOfDimensions       			= numOfDimensions;

	// Allocate space where possible for grid data
	interior_box    ->    allocate( numOfGrids, 2, numOfDimensions,   -1, -1 );
	domain_box      ->    allocate( numOfGrids, 2, numOfDimensions,   -1, -1 );
	xy              ->    allocate( numOfGrids, 0, 0, 0,              -1, -1, -1 );
	desc            ->    allocate( numOfGrids, 0, 0,                 -1, -1 );
	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////

	
	// Get number of interp pts from CompositeGrid
	const IntegerArray 		&ni 		= compositeGrid.numberOfInterpolationPoints;
	
	for ( int gridIndex = 0; gridIndex < numOfGrids; gridIndex++ )
	{
		// Utilize Overture methods to easily retrieve data
		MappedGrid 				&grid 					= compositeGrid[ gridIndex ];
		grid.update( MappedGrid::THEvertex | MappedGrid::THEmask );  // create the vertex and mask arrays

		const IntegerArray 		&gridDimensions 		= grid.dimension();          // grid array dimensions
		const IntegerArray 		&gridIndexRange 		= grid.gridIndexRange();
		const IntegerArray 		&bc             		= grid.boundaryCondition();  // unused for now


		printf( "%i %s (grid and name)\n"
				"%i %i %i %i %i %i ( dimension(0:1,0:2), array dimensions )\n"
				"%i %i %i %i %i %i ( gridIndexRange(0:1,0:2), grid bounds )\n"
				"%i %i %i %i %i %i ( boundaryCondition(0:1,0:2) )\n"
				"%i %i %i          ( isPeriodic(0:2), 0 = not, 1 = deriv, 2 = function )\n",
				gridIndex             , (const char*)grid.getName(),
				gridDimensions( 0, 0 ), gridDimensions( 1, 0 ), gridDimensions( 0, 1 ), 
				gridDimensions( 1, 1 ), gridDimensions( 0, 2 ), gridDimensions( 1, 2 ),
				gridIndexRange( 0, 0 ), gridIndexRange( 1, 0 ), gridIndexRange( 0, 1 ), 
				gridIndexRange( 1, 1 ), gridIndexRange( 0, 2 ), gridIndexRange( 1, 2 ),
				bc( 0, 0 ), bc( 1, 0 ), bc( 0, 1 ), bc( 1, 1 ), bc( 0, 2 ), bc( 1, 2 ),
				grid.isPeriodic(0)    , grid.isPeriodic(1)    , grid.isPeriodic(2) );
		
		
		const intArray 			&mask 				= grid.mask();
		intSerialArray 			maskLocal; 

		// local array on this processor
		getLocalArrayWithGhostBoundaries( mask, maskLocal );  
		const realArray 		&vertex 			= grid.vertex();
		realSerialArray 		vertexLocal; 

		// local array on this processor
		getLocalArrayWithGhostBoundaries( vertex, vertexLocal );   
		// Note that k below is just 0, not a real loop index. Only one Z index.
		int 	k       = vertexLocal.getBase( 3 );


		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// Set interior_box and domain_box /////////////////////////////////////////////////////////////////
		for ( int i = 0; i < 2; i++ )
		{
			for ( int j = 0; j < numOfDimensions; j++ )
			{
				interior_box -> data[ gridIndex ][ i ][ j ] 	= gridIndexRange( i, j ) - gridDimensions( 0, j );
				domain_box   -> data[ gridIndex ][ i ][ j ] 	= gridDimensions( i, j ) - gridDimensions( 0, j );
			}
		}
		
		// Print interior_box for verification
		std::cout << interior_box -> data[ gridIndex ][ 0 ][ 0 ] << interior_box -> data[ gridIndex ][ 0 ][ 1 ] 
				<< interior_box -> data[ gridIndex ][ 1 ][ 0 ] << interior_box -> data[ gridIndex ][ 1 ][ 1 ] 
				<< std::endl;
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Set desc, xy, and mask //////////////////////////////////////////////////////////////////////////////
		int 	nx      = gridDimensions( 1, 0 ) - gridDimensions( 0, 0 );
		int 	ny      = gridDimensions( 1, 1 ) - gridDimensions( 0, 1 );

		// Allocate space for gridIndex'th grid in desc and xy
		desc  -> allocate( 0, ( nx + 1 ), ( ny + 1 ),                       gridIndex, -1 );
		xy    -> allocate( 0, ( nx + 1 ), ( ny + 1 ), numOfDimensions,      gridIndex, -1, -1 );
		
		for( int i = 0; i < nx + 1; i++ )
		{
			for( int j = 0; j < ny + 1; j++ )
			{
				desc -> data[ gridIndex ][ i ][ j ]   		= maskLocal(  	i + gridDimensions( 0, 0 ), 
																			j + gridDimensions( 0, 1 ), 
																			k ) >= 0 ? -1: 1;

				for( int l = 0; l < numOfDimensions; l++ )
				{
					xy -> data[ gridIndex ][ i ][ j ][ l ]  	= vertexLocal(  i + gridDimensions( 0, 0 ), 
																				j + gridDimensions( 0, 1 ), 
																				k + gridDimensions( 0, 2 ), l );
				}

			}
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		

		// destroy arrays to save space
		grid.destroy( MappedGrid::THEvertex | MappedGrid::THEmask );  
	}

	return 0;
}