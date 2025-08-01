#include <string>
#ifdef OLD_HEADER_FILENAME
#include <iostream.h>
#else
#include <iostream>
#endif

#include "inAndOutHDF5.h"
#include "HydeCompositeGrid.h"
#include "HydeGridData.h"
#include "HydeInterpData.h"

#include "H5Cpp.h"


#include "Array2D.h"
#include "Array3D.h"
#include "Array4D.h"



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

#undef  RXI
#define RXI(I1,I2,I3,var,dir)  ( rx( I1, I2, I3, numOfDimensions * dir + var ) )
// #define RXI(I1,I2,I3,dir)  (inverseVertexDerivative(I1,I2,I3, 3 * dir + 0))
// #define SXI(I1,I2,I3,dir)  (inverseVertexDerivative(I1,I2,I3, 3 * dir + 1))
// #define TXI(I1,I2,I3,dir)  (inverseVertexDerivative(I1,I2,I3, 3 * dir + 2))

// #define RX(I1,I2,I3) inverseVertexDerivative(I1,I2,I3,0)
// #define SX(I1,I2,I3) inverseVertexDerivative(I1,I2,I3,1)
// #define TX(I1,I2,I3) inverseVertexDerivative(I1,I2,I3,2)
// #define RY(I1,I2,I3) inverseVertexDerivative(I1,I2,I3,0+numberOfDimensions)
// #define SY(I1,I2,I3) inverseVertexDerivative(I1,I2,I3,1+numberOfDimensions)
// #define TY(I1,I2,I3) inverseVertexDerivative(I1,I2,I3,5)
// #define RZ(I1,I2,I3) inverseVertexDerivative(I1,I2,I3,6)
// #define SZ(I1,I2,I3) inverseVertexDerivative(I1,I2,I3,7)
// #define TZ(I1,I2,I3) inverseVertexDerivative(I1,I2,I3,8)



int sendToHDF5(   	std::string     		nameOfNewFile,
                	HydeCompositeGrid 		*hydeCompositeGrid )
{
	try
	{

		H5File* 			file 							= new H5File( nameOfNewFile, H5F_ACC_TRUNC );
		

		// Create a group for the composite grid in the file.
		Group 				compGridGroup( file->createGroup( "/compositeGrid") );

		// Create dataspace for dataset in file.
		DataSet   			*dataset;


		////////////////////////////////////////////////////////////////////////////////////////
		// Set dimensions //////////////////////////////////////////////////////////////////////
		hsize_t     		dimsize[] 						= {1};
		DataSpace   		dimSpace( 1, dimsize );

		dataset 											= new 	DataSet( 
																				compGridGroup.createDataSet(  	"dim", 
																												PredType::NATIVE_INT, 
																												dimSpace ) 
																			);

		dataset ->      write(  &( hydeCompositeGrid -> dimOfProblem ), 
								PredType::NATIVE_INT );

		delete 				dataset;
		////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////
		

		////////////////////////////////////////////////////////////////////////////////////////
		// Set numberOfComponentGrids //////////////////////////////////////////////////////////
		hsize_t     		numOfGridsIntSize[] 						= {1};
		DataSpace   		numGridSpace( 1, numOfGridsIntSize );

		dataset 														= new 	DataSet( 
																							compGridGroup.createDataSet(  	"numOfGrids", 
																															PredType::NATIVE_INT, 
																															numGridSpace ) 
																						);
		dataset ->      write(  &( hydeCompositeGrid -> numOfGrids ), 
								PredType::NATIVE_INT );

		delete 				dataset;
		////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////


		for ( int gridIndex = 0; gridIndex < hydeCompositeGrid -> numOfGrids; gridIndex++ )
		{
			HydeGridData 	*hydeGridData 				= hydeCompositeGrid -> hydeGridData[ gridIndex ];

			// Create a grid group in the file.
			Group 			group( compGridGroup.createGroup( "grid" + std::to_string( gridIndex ) ) );
		

			////////////////////////////////////////////////////////////////////////////////////////
			// Set grid number ///////////////////////////////////////////////////////////////////
			hsize_t     		gridNumIntSize[] 						= {1};
			DataSpace   		gridNumSpace( 1, gridNumIntSize );

			dataset 														= new 	DataSet( 
																								group.createDataSet(  	"gridNumber", 
																														PredType::NATIVE_INT, 
																														gridNumSpace ) 
																							);
			dataset ->      write(  &( hydeGridData -> gridNumber ), 
									PredType::NATIVE_INT );

			delete 				dataset;
			////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////
		

			////////////////////////////////////////////////////////////////////////////////////////
			// Set useGhostPts ///////////////////////////////////////////////////////////////////
			dataset 														= new 	DataSet( 
																								group.createDataSet(  	"useGhostPts", 
																														PredType::NATIVE_INT, 
																														gridNumSpace ) 
																							);
			dataset ->      write(  &( hydeGridData -> useGhostPts ), 
									PredType::NATIVE_INT );

			delete 				dataset;
			////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////
		

			////////////////////////////////////////////////////////////////////////////////////////
			// Set grid dim ////////////////////////////////////////////////////////////////////////
			int 				numOfDimensions 							= hydeGridData -> dim;
			dataset 														= new 	DataSet( 
																								group.createDataSet(  	"gridDim", 
																														PredType::NATIVE_INT, 
																														gridNumSpace ) 
																							);
			dataset ->      write(  &numOfDimensions, 
									PredType::NATIVE_INT );

			delete 				dataset;
			////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////

			////////////////////////////////////////////////////////////////////////////////////////
			// Set grid spacing ////////////////////////////////////////////////////////////////////
			hsize_t     		gridSpacingIntSize[] 					= {3};
			DataSpace   		gridSpacingSpace( 1, gridSpacingIntSize );

			dataset 														= new 	DataSet( 
																								group.createDataSet(  	"gridSpacing", 
																														PredType::NATIVE_DOUBLE, 
																														gridSpacingSpace ) 
																							);
			dataset ->      write(  &( 	hydeGridData -> gridSpacing ), 
										PredType::NATIVE_DOUBLE );

			delete 				dataset;
			////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////

			/////////////////////////////////////////////////////////////////////////////////////
			// Set interior box /////////////////////////////////////////////////////////////////
			hsize_t     	boxsize[] 					= {2, 3};
			DataSpace   	boxspace( 2, boxsize );

			dataset 									= new 	DataSet( 
																			group.createDataSet(  	"numOfGhostPts", 
																									PredType::NATIVE_INT, 
																									boxspace ) 
																		);
			dataset ->      write(  &( hydeGridData -> numOfGhostPts ), 
									PredType::NATIVE_INT );

			delete 			dataset;
			/////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////


			/////////////////////////////////////////////////////////////////////////////////////
			// Set interior box /////////////////////////////////////////////////////////////////
			dataset 									= new 	DataSet( 
																			group.createDataSet(  	"boundingBox", 
																									PredType::NATIVE_DOUBLE, 
																									boxspace ) 
																		);
			dataset ->      write(  &( hydeGridData -> boundingBox ), 
									PredType::NATIVE_DOUBLE );

			delete 			dataset;
			/////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////


			/////////////////////////////////////////////////////////////////////////////////////
			// Set interior box /////////////////////////////////////////////////////////////////
			dataset 									= new 	DataSet( 
																			group.createDataSet(  	"gridIndexRange", 
																									PredType::NATIVE_INT, 
																									boxspace ) 
																		);
			dataset ->      write(  &( hydeGridData -> gridIndexRange ), 
									PredType::NATIVE_INT );

			delete 			dataset;
			/////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////


			/////////////////////////////////////////////////////////////////////////////////////
			// Set extIndexRange ///////////////////////////////////////////////////////////////////
			dataset 									= new 	DataSet( 
																			group.createDataSet(  	"extIndexRange", 
																									PredType::NATIVE_INT, 
																									boxspace ) 
																		);

			dataset ->      write(  &( hydeGridData -> extGridIndexRange ), 
									PredType::NATIVE_INT );

			delete 			dataset;
			/////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////


			/////////////////////////////////////////////////////////////////////////////////////
			// Set bcs //////////////////////////////////////////////////////////////////////////
			dataset 									= new 	DataSet( 
																			group.createDataSet(  	"boundaryCondition", 
																									PredType::NATIVE_INT, 
																									boxspace ) 
																		);

			dataset ->      write(  &( hydeGridData -> boundaryCondition ), 
									PredType::NATIVE_INT );

			delete 			dataset;
			/////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////


			/////////////////////////////////////////////////////////////////////////////////////
			// Set mask /////////////////////////////////////////////////////////////////////////
			int 			maskDim1 					= hydeGridData -> arrayMask -> rows;
			int 			maskDim2 					= hydeGridData -> arrayMask -> cols;

			hsize_t   		gridsize2[] 				= { static_cast<hsize_t>(maskDim1), 
															static_cast<hsize_t>(maskDim2) };

			DataSpace   	maskspace( 2, gridsize2 );

			dataset 									= new 	DataSet( 
																			group.createDataSet(  	"arrayMask", 
																									PredType::NATIVE_INT, 
																									maskspace ) 
																		);

			// Not sure why this is needed for now...
			int mask_Placeholder[ maskDim1 ][ maskDim2 ];

			for( int i = 0; i < ( maskDim1 ); i++ )
			{
				for( int j = 0; j < ( maskDim2 ); j++ )
				{
					mask_Placeholder[ i ][ j ] 			= hydeGridData -> arrayMask -> data[ i ][ j ];
				}
			}


			dataset ->      write(  &( mask_Placeholder ), 
									PredType::NATIVE_INT );

			delete 			dataset;
			/////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////


			/////////////////////////////////////////////////////////////////////////////////////
			// Set xy ///////////////////////////////////////////////////////////////////////////
			hsize_t   		gridsize1[] 				= { static_cast<hsize_t> ( maskDim1 ), 
															static_cast<hsize_t> ( maskDim2 ), 
															static_cast<hsize_t> ( numOfDimensions ) };

			DataSpace 		xyspace(  	numOfDimensions + 1, 
										gridsize1 );
			
			dataset 									= new 	DataSet( 
																			group.createDataSet(  	"xy", 
																									PredType::NATIVE_DOUBLE, 
																									xyspace ) 
																		);

			// Not sure why this is needed for now...
			double 		xy_Placeholder[ maskDim1 ][ maskDim2 ][ numOfDimensions ];

			for( int i = 0; i < ( maskDim1 ); i++ )
			{
				for( int j = 0; j < ( maskDim2 ); j++ )
				{
					for( int k = 0; k < numOfDimensions; k++ )
					{
						// xy_Placeholder[ i ][ j ][ k ] 			= xy -> data[ gridIndex ][ i ][ j ][ k ];
						xy_Placeholder[ i ][ j ][ k ] 			= hydeGridData 
																			-> xy 
																				-> data[ i ][ j ][ k ];
					}
				}
			}


			dataset ->      write(  &xy_Placeholder, 
									PredType::NATIVE_DOUBLE );

			delete 			dataset;
			/////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////


			/////////////////////////////////////////////////////////////////////////////////////
			// Set xy ///////////////////////////////////////////////////////////////////////////
			hsize_t   		gridsize3[] 				= { static_cast<hsize_t> ( maskDim1 ), 
															static_cast<hsize_t> ( maskDim2 ), 
															static_cast<hsize_t> ( numOfDimensions), 
															static_cast<hsize_t> ( numOfDimensions ) };

			DataSpace 		rxSpace(  	numOfDimensions + 1, 
										gridsize3 );
			
			dataset 									= new 	DataSet( 
																			group.createDataSet(  	"rx", 
																									PredType::NATIVE_DOUBLE, 
																									rxSpace ) 
																		);

			// Not sure why this is needed for now...
			double 		rx_Placeholder[ maskDim1 ][ maskDim2 ][ numOfDimensions ][ numOfDimensions ];

			for( int i = 0; i < ( maskDim1 ); i++ )
			{
				for( int j = 0; j < ( maskDim2 ); j++ )
				{
					for( int k = 0; k < numOfDimensions; k++ )
					{
						for( int l = 0; l < numOfDimensions; l++ )
						{
							rx_Placeholder[ i ][ j ][ k ][ l ] 		= hydeGridData 
																			-> rx 
																				-> data[ i ][ j ][ k ][ l ];
						}
					}
				}
			}


			dataset ->      write(  &rx_Placeholder, 
									PredType::NATIVE_DOUBLE );

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



int getFromHDF5(    const char     			*fileName, 
                    HydeCompositeGrid 		*hydeCompositeGrid )
{   
	// Read in a CompositeGrid
	aString 					nameOfOGFile      		= fileName;
	CompositeGrid             	compositeGrid;
	getFromADataBase( compositeGrid, nameOfOGFile );


	/////////////////////////////////////////////////////////////////////////////////
	// Initialize number of component grids and dimension of grid data /////////////
	int 		numOfGrids        		= compositeGrid.numberOfComponentGrids();
	// *numberOfComponentGrids   			= numOfGrids;

	int 		numOfDimensions       	= compositeGrid.numberOfDimensions();
	// *numberOfDimensions       			= numOfDimensions;


	// hydeCompositeGrid 					= new HydeCompositeGrid( 	fileName,
	// 																numOfGrids, 
	// 																numOfDimensions );
	
	hydeCompositeGrid 
		-> numOfGrids 					= numOfGrids;
	hydeCompositeGrid 
		-> dimOfProblem 				= numOfDimensions;

	// Instantiate numOfGrids GridData objects for this CompositeGrid
	hydeCompositeGrid 
		-> hydeGridData            		= new HydeGridData*  [ numOfGrids ];						


	
	// Get number of interp pts from CompositeGrid
	const IntegerArray 		&ni 		= compositeGrid.numberOfInterpolationPoints;
	
	for ( int gridIndex = 0; gridIndex < numOfGrids; gridIndex++ )
	{
		HydeGridData 			*hydeGridData 			= new HydeGridData( gridIndex );
		hydeGridData -> dim    							= numOfDimensions;


		// Utilize Overture methods to easily retrieve data
		MappedGrid 				&grid 					= compositeGrid[ gridIndex ];
		grid.update( 	MappedGrid::THEvertex 
					| 	MappedGrid::THEmask 
					| 	MappedGrid::THEinverseVertexDerivative 
					| 	MappedGrid::THEvertexJacobian );  // create the vertex and mask arrays

		const IntegerArray 		&gridDimensions 		= grid.dimension();          // grid array dimensions
		const IntegerArray 		&gridIndexRange 		= grid.gridIndexRange();
    	const IntegerArray 		&extIndexRange 			= grid.extendedIndexRange();
		const IntegerArray 		&bc             		= grid.boundaryCondition();  // unused for now

		const RealArray 		&det 					= grid.vertexJacobian(  ); // get the vertex jacobian determinant
		const RealArray 		&rx 					= grid.inverseVertexDerivative();
		const RealArray 		&rx_inv 				= grid.vertexDerivative();

    	const IntegerArray 		&numOfGhosts 			= grid.numberOfGhostPoints();
    	const Logical 			&useGhostPoints 		= grid.useGhostPoints();
    	const RealArray 		&boundingBox 			= grid.boundingBox();
    	const RealArray 		&gridSpacing 			= grid.gridSpacing();


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
		


		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// Set grid_index_range and ext_index_range /////////////////////////////////////////////////////////////////
		hydeGridData
			-> useGhostPts 							= useGhostPoints;
		int 					ghostPtsCounter 	= 0;

		for ( int j = 0; j < 3; j++ )
		{
			hydeGridData 
				-> gridSpacing[ j ] 			= gridSpacing( j );
			

            ghostPtsCounter                     = 0;

			for ( int i = 0; i < 2; i++ )
			{
				hydeGridData 
					-> boundingBox[ i ][ j ] 				= boundingBox( i, j );

				hydeGridData 
					-> gridIndexRange[ i ][ j ] 			= gridIndexRange( i, j );

				hydeGridData 
					-> extGridIndexRange[ i ][ j ] 			= extIndexRange( i, j );

				hydeGridData 
					-> numOfGhostPts[ i ][ j ] 				= numOfGhosts( i, j );

				hydeGridData 
					-> boundaryCondition[ i ][ j ]			= bc( i, j );

                ghostPtsCounter                             += hydeGridData -> numOfGhostPts[ i ][ j ];
			}

            // Set numbers of relevant grid pts data.
            hydeGridData -> N[ j ]                  = (     gridDimensions( 1, j )
                                                    	-   gridDimensions( 0, j )    )   +   1;

            hydeGridData -> NP[ j ]                 = hydeGridData -> N[ j ]      +   ghostPtsCounter;
		}
		
		// Print grid_index_range for verification
		std::cout 	<< hydeGridData -> extGridIndexRange[ 0 ][ 0 ] << " " 
					<< hydeGridData -> extGridIndexRange[ 0 ][ 1 ] << " " 
					<< hydeGridData -> extGridIndexRange[ 1 ][ 0 ] << " " 
					<< hydeGridData -> extGridIndexRange[ 1 ][ 1 ] << " " 
					<< std::endl;
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Set desc, xy, and mask //////////////////////////////////////////////////////////////////////////////
				
		Index 					I1, I2, I3;
		getIndex( grid.dimension(  ), I1, I2, I3 );
		int 					i1, i2, i3;

		const intArray 			&mask 				= grid.mask();
		intSerialArray 			maskLocal; 
		// local array on this processor
		getLocalArrayWithGhostBoundaries( mask, maskLocal );  

		int 					includeGhost 		= 0;
		bool 					ok 					= ParallelUtility::getLocalArrayBounds( mask, maskLocal, 
																							I1, I2, I3, 
																							includeGhost );  // get bounds of the local array ( no ghost )


		// const realArray 		&vertex 			= grid.vertex();
		// realSerialArray 		vertexLocal; 
		// local array on this processor
		// getLocalArrayWithGhostBoundaries( vertex, vertexLocal );   
		// Note that k below is just 0, not a real loop index. Only one Z index.
		// int 					k       			= vertexLocal.getBase( 3 );
		
		
		// Allocate space for gridIndex'th grid in desc and xy
		int 		gridSize1 			= ( I1.getBound() - gridDimensions( 0, 0 ) ) - ( I1.getBase() - gridDimensions( 0, 0 ) ) + 1;
		int 		gridSize2 			= ( I2.getBound() - gridDimensions( 0, 1 ) ) - ( I2.getBase() - gridDimensions( 0, 1 ) ) + 1;

		hydeGridData 
				-> arrayMask  
					-> allocate( 	gridSize1, 
									gridSize2 );

		hydeGridData 
				-> xy	
					-> allocate( 	gridSize1, 
									gridSize2, 
									numOfDimensions );

		hydeGridData 
				-> rx	
					-> allocate( 	gridSize1, 
									gridSize2, 
									numOfDimensions );
		hydeGridData
				-> rx -> setIdentity();  // initialize to zero

		// hydeGridData 
		// 		-> rx_inv	
		// 			-> allocate( 	gridSize1, 
		// 							gridSize2, 
		// 							numOfDimensions );
		// hydeGridData
		// 		-> rx_inv -> setIdentity();  // initialize to zero
		

		if(  ok  )  // if there are points on this processor
		{
			printf( "mask ( based on the dimension array,  -1=interp,  0=not used,  1=used )\n" );

			FOR_3D( i1, i2, i3, I1, I2, I3 )
			{
				int 			m 					= maskLocal( i1, i2, i3 );

				if(  m < 0  )
				{
					m 	= -1;   // interpolation point
				}
				else if(  m > 0  )
				{
					m 	= 1;    // discretization point 
				}
				else
				{
					m 	= 0;    // not used point
				}

				int 		index1 			= i1 - gridDimensions( 0, 0 );
				int 		index2 			= i2 - gridDimensions( 0, 1 );

				// printf( "(%i, %i, %i), (%i, %i), %i ( i1, i2, i3, index1, index2, m )\n", 
				// 		i1, i2, i3, index1, index2, m );

				hydeGridData 
					-> arrayMask  
						-> data[ index1 ][ index2 ]   		= m;
			}
      		// printf( "\n" );
			
			const realArray 			&vertex 			= grid.vertex(  );
			realSerialArray 			vertexLocal; 
			getLocalArrayWithGhostBoundaries( vertex, vertexLocal );          // local array on this processor


			printf( "vertex ( based on the dimension array )\n" );

			FOR_3( i1, i2, i3, I1, I2, I3 )
			{
				int 		index1 			= i1 - gridDimensions( 0, 0 );
				int 		index2 			= i2 - gridDimensions( 0, 1 );

				// printf( "(%i, %i, %i), (%i, %i) ( i1, i2, i3, index1, index2 )\n", 
				// 		i1, i2, i3, index1, index2 );

				for ( int k = 0; k < numOfDimensions; k++ )
				{
					hydeGridData 
							-> xy 
								-> data[ index1 ][ index2 ][ k ]  		= vertexLocal( i1, i2, i3, k );
				}

				for ( int dir = 0; dir < numOfDimensions; dir++ ) // x, y, z
				{
					for ( int var = 0; var < numOfDimensions; var++ ) // r, s, t
					{
						hydeGridData 
								-> rx 
									-> data[ index1 ][ index2 ][ dir ][ var ] 		= RXI( i1, i2, i3, var , dir);
						// hydeGridData 
						// 		-> rx_inv 
						// 			-> data[ index1 ][ index2 ][ dir ][ var ] 		= rx_inv( i1, i2, i3, numOfDimensions * dir + var );
					}
				}

			}
			// printf( "\n" );
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		

		hydeCompositeGrid 
				-> hydeGridData[ gridIndex ] 			= hydeGridData;


		// destroy arrays to save space
		grid.destroy( MappedGrid::THEvertex | MappedGrid::THEmask );  
	}

	return 0;
}