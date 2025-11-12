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


#include "Array1D.h"
#include "Array2D.h"
#include "Array3D.h"
#include "Array4D.h"
#include "Array4D_V3.h"



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
		Group 				compGridGroup( file->createGroup( "/CompositeGrid") );

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
		// Set N ///////////////////////////////////////////////////////////////////////////////
		hsize_t     		gridDimSize[] 					= {3};
		DataSpace   		gridDimSpace( 1, gridDimSize );

		dataset 											= new 	DataSet( 
																				group.createDataSet(  	"N", 
																										PredType::NATIVE_INT, 
																										gridDimSpace ) 
																			);

		dataset ->      write(  &( hydeGridData -> N ), 
								PredType::NATIVE_INT );

		delete 				dataset;
		////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////
		// Set NP ///////////////////////////////////////////////////////////////////////////////
		dataset 											= new 	DataSet( 
																				group.createDataSet(  	"NP", 
																										PredType::NATIVE_INT, 
																										gridDimSpace ) 
																			);

		dataset ->      write(  &( hydeGridData -> NP ), 
								PredType::NATIVE_INT );

		delete 				dataset;
		////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////
		// Set npoints ///////////////////////////////////////////////////////////////////////////////
		dataset 											= new 	DataSet( 
																				group.createDataSet(  	"numOfGridPts", 
																										PredType::NATIVE_INT, 
																										gridNumSpace ) 
																			);

		dataset ->      write(  &( hydeGridData -> npoints ), 
								PredType::NATIVE_INT );

		delete 				dataset;
		////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////
		
			////////////////////////////////////////////////////////////////////////////////////////
			// Set grid type ///////////////////////////////////////////////////////////////////
			hsize_t     		gridTypeStrSize[] 							= { hydeGridData -> gridType.size() };
			DataSpace   		gridTypeSpace( 1, gridTypeStrSize );

			dataset 														= new 	DataSet(
																								group.createDataSet("gridType",
																													PredType::C_S1,
																													gridTypeSpace)
																							);

			dataset -> write( hydeGridData -> gridType.c_str(), PredType::C_S1 );

			delete dataset;
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

			////////////////////////////////////////////////////////////////////////////////////////
			// Set dx ////////////////////////////////////////////////////////////////////////////
			DataSpace   		dxSpace( 1, gridSpacingIntSize );

			dataset 														= new 	DataSet( 
																								group.createDataSet(  	"dx", 
																														PredType::NATIVE_DOUBLE, 
																														dxSpace ) 
																							);
			dataset ->      write(  &( hydeGridData -> dx ), 
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
			// Set bounding box /////////////////////////////////////////////////////////////////
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
																			group.createDataSet(  	"interiorBox", 
																									PredType::NATIVE_DOUBLE, 
																									boxspace ) 
																		);
			dataset ->      write(  &( hydeGridData -> interiorBox ), 
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
			int 			maskDim1 					= hydeGridData -> arrayMask -> dim1;
			int 			maskDim2 					= hydeGridData -> arrayMask -> dim2;
			int 			maskDim3 					= hydeGridData -> arrayMask -> dim3;

			hsize_t   		gridsize2[] 				= { static_cast<hsize_t>(maskDim1), 
															static_cast<hsize_t>(maskDim2),
															static_cast<hsize_t>(maskDim3) };

			DataSpace   	maskspace( 3, gridsize2 );

			dataset 									= new 	DataSet( 
																			group.createDataSet(  	"arrayMask", 
																									PredType::NATIVE_INT, 
																									maskspace ) 
																		);

			// Dynamically allocate memory for large arrays to avoid stack overflow
			size_t 			totalSize 					= static_cast<size_t>(maskDim1 * maskDim2 * maskDim3);
			int 			*mask_Placeholder 			= new int[ totalSize ];
			
			for( int i = 0; i < ( maskDim1 ); i++ )
			{
				for( int j = 0; j < ( maskDim2 ); j++ )
				{
					for( int k = 0; k < ( maskDim3 ); k++ )
					{
						size_t 		index 					= static_cast<size_t>( i * maskDim2 + j ) * maskDim3 + k;
						mask_Placeholder[ index ] 			= hydeGridData -> arrayMask -> data[ i ][ j ][ k ];
					}
				}
			}


			dataset ->      write(  mask_Placeholder, 
									PredType::NATIVE_INT );

			// Clean up dynamically allocated memory
			delete[] mask_Placeholder;
			hydeGridData -> arrayMask -> deallocate();

			delete 			dataset;
			/////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////


			/////////////////////////////////////////////////////////////////////////////////////
			// Set xy ///////////////////////////////////////////////////////////////////////////
			hsize_t   		gridsize1[] 				= { static_cast<hsize_t> ( maskDim1 ), 
															static_cast<hsize_t> ( maskDim2 ),
															static_cast<hsize_t> ( maskDim3 ),
															static_cast<hsize_t> ( numOfDimensions ) };

			DataSpace 		xyspace(  	4, 
										gridsize1 );
			
			dataset 									= new 	DataSet( 
																			group.createDataSet(  	"xy", 
																									PredType::NATIVE_DOUBLE, 
																									xyspace ) 
																		);

			// Dynamically allocate memory for large arrays to avoid stack overflow
			totalSize 									= static_cast<size_t>(maskDim1 * maskDim2 * maskDim3 * numOfDimensions);
			double 			*xy_Placeholder 			= new double[ totalSize ];

			for( int i = 0; i < ( maskDim1 ); i++ )
			{
				for( int j = 0; j < ( maskDim2 ); j++ )
				{
					for( int k = 0; k < ( maskDim3 ); k++ )
					{
						for ( int d = 0; d < numOfDimensions; d++ )
						{
							size_t 		index 			= (static_cast<size_t>(i * maskDim2 + j) * maskDim3 + k) * numOfDimensions + d;

							// xy_Placeholder[ i ][ j ][ k ] 			= xy -> data[ gridIndex ][ i ][ j ][ k ];
							xy_Placeholder[ index ] 			= hydeGridData 
																				-> xy 
																					-> data[ i ][ j ][ k ][ d ];
						}
					}
				}
			}


			dataset ->      write(  xy_Placeholder, 
									PredType::NATIVE_DOUBLE );

			// Clean up dynamically allocated memory
			delete[] xy_Placeholder;
			hydeGridData -> xy -> deallocate();

			delete 			dataset;
			/////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////
		
			if ( hydeGridData -> gridType == "Curvilinear" )
			{
				////////////////////////////////////////////////////////////////////////////////////////
				// Set jacobDet ////////////////////////////////////////////////////////////////////////
				hsize_t     	interpJacobDetSize[] 						= { static_cast<hsize_t>( hydeGridData -> jacobDet -> dim1 ),
																				static_cast<hsize_t>( hydeGridData -> jacobDet -> dim2 ),
																				static_cast<hsize_t>( hydeGridData -> jacobDet -> dim3 ) };
				DataSpace   		interpNumPtsSpace( 3, interpJacobDetSize );
				dataset 														= new 	DataSet( 
																									group.createDataSet(  	"jacobDet", 
																															PredType::NATIVE_DOUBLE, 
																															interpNumPtsSpace ) 
																								);

				// Dynamically allocate memory for large arrays to avoid stack overflow
				totalSize 										= static_cast<size_t>(hydeGridData -> jacobDet -> dim1) 
																					* (hydeGridData -> jacobDet -> dim2) 
																					* (hydeGridData -> jacobDet -> dim3);
				double 			*jacobDet_placeholder 			= new double[ totalSize ];

				for( int i = 0; i < hydeGridData -> jacobDet -> dim1; i++ )
				{
					for( int j = 0; j < hydeGridData -> jacobDet -> dim2; j++ )
					{
						for( int k = 0; k < hydeGridData -> jacobDet -> dim3; k++ )
						{
							size_t 		index 			= static_cast<size_t>( i * (hydeGridData -> jacobDet -> dim2) + j ) * (hydeGridData -> jacobDet -> dim3) + k;

							jacobDet_placeholder[ index ] = hydeGridData 
															-> jacobDet 
																-> data[ i ][ j ][ k ];
						}
					}
				}
				
				dataset ->      write(  jacobDet_placeholder, 
										PredType::NATIVE_DOUBLE );

				// Clean up dynamically allocated memory
				delete[] jacobDet_placeholder;
				hydeGridData -> jacobDet -> deallocate();

				delete 				dataset;
				////////////////////////////////////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////////////////////////////////////


				/////////////////////////////////////////////////////////////////////////////////////
				// Set rx ///////////////////////////////////////////////////////////////////////////
				hsize_t   		gridsize3[] 				= { static_cast<hsize_t> ( maskDim1 ), 
																static_cast<hsize_t> ( maskDim2 ), 
																static_cast<hsize_t> ( maskDim3 ), 
																static_cast<hsize_t> ( numOfDimensions), 
																static_cast<hsize_t> ( numOfDimensions ) };

				DataSpace 		rxSpace(  	5, 
											gridsize3 );
				
				dataset 									= new 	DataSet( 
																				group.createDataSet(  	"rx", 
																										PredType::NATIVE_DOUBLE, 
																										rxSpace ) 
																			);

				// Create contiguous memory layout for HDF5
				totalSize 									= static_cast<size_t>(maskDim1 * maskDim2 * maskDim3 * numOfDimensions * numOfDimensions);
				double 			*rx_Placeholder 			= new double[ totalSize ];

				// Copy data in row-major order (C-style)
				for( int i = 0; i < maskDim1; i++ )
				{
					for( int j = 0; j < maskDim2; j++ )
					{
						for( int k = 0; k < maskDim3; k++ )
						{
							for( int l = 0; l < numOfDimensions; l++ )
							{
								for ( int m = 0; m < numOfDimensions; m++ )
								{
									size_t 		index 			= static_cast<size_t>( ( ( i * maskDim2 + j ) * maskDim3 + k ) * numOfDimensions + l ) * numOfDimensions + m;
									
									rx_Placeholder[index] 		= hydeGridData->rx->data[i][j][k][l][m];
								}
							}
						}
					}
				}

				dataset ->      write(  rx_Placeholder, 
										PredType::NATIVE_DOUBLE );

				// Clean up contiguous array
				delete[] rx_Placeholder;
				hydeGridData -> rx -> deallocate();

				delete 			dataset;
				/////////////////////////////////////////////////////////////////////////////////////
				/////////////////////////////////////////////////////////////////////////////////////


				/////////////////////////////////////////////////////////////////////////////////////
				// Set rx_inv ///////////////////////////////////////////////////////////////////////////
				
				dataset 									= new 	DataSet( 
																				group.createDataSet(  	"rxInv", 
																										PredType::NATIVE_DOUBLE, 
																										rxSpace ) 
																			);

				// Create contiguous memory layout for HDF5
				totalSize 										= static_cast<size_t>(maskDim1 * maskDim2 * maskDim3 * numOfDimensions * numOfDimensions);
				double 				*rxInv_Placeholder 			= new double[ totalSize ];

				// Copy data in row-major order (C-style)
				for( int i = 0; i < maskDim1; i++ )
				{
					for( int j = 0; j < maskDim2; j++ )
					{
						for( int k = 0; k < maskDim3; k++ )
						{
							for( int l = 0; l < numOfDimensions; l++ )
							{
								for ( int m = 0; m < numOfDimensions; m++ )
								{
									size_t 		index 			= static_cast<size_t>( ( ( i * maskDim2 + j ) * maskDim3 + k ) * numOfDimensions + l ) * numOfDimensions + m;
									
									rxInv_Placeholder[index] 		= hydeGridData->rx_inv->data[i][j][k][l][m];
								}
							}
						}
					}
				}

				dataset ->      write(  rxInv_Placeholder, 
										PredType::NATIVE_DOUBLE );

				// Clean up contiguous array
				delete[] rxInv_Placeholder;
				hydeGridData -> rx_inv -> deallocate();

				delete 			dataset;
				/////////////////////////////////////////////////////////////////////////////////////
				/////////////////////////////////////////////////////////////////////////////////////
			}



			if ( hydeCompositeGrid -> numOfGrids > 1 )
			{
				HydeInterpData 	*hydeInterpData 				= hydeCompositeGrid -> hydeInterpData[ gridIndex ];

				// Create a grid group in the file.
				Group 			group2( compGridGroup.createGroup( "interpData" + std::to_string( gridIndex ) ) );

				/////////////////////////////////////////////////////////////////////////////////////
				// Set grid number ///////////////////////////////////////////////////////////////////
				hsize_t     	interpGridNumIntSize[] 						= {1};
				DataSpace   		interpGridNumSpace( 1, interpGridNumIntSize );
				dataset 														= new 	DataSet( 
																									group2.createDataSet(  	"numOfInterpPts", 
																															PredType::NATIVE_INT, 
																															interpGridNumSpace ) 
																								);
				dataset ->      write(  &( hydeInterpData -> numOfInterpPts ), 
										PredType::NATIVE_INT );
				delete 				dataset;
				/////////////////////////////////////////////////////////////////////////////////////
				/////////////////////////////////////////////////////////////////////////////////////


				hsize_t   		numInterpPtsIntSize[] 			= { static_cast<hsize_t>( hydeInterpData -> allInterpPtsIndices -> rows ),
																	static_cast<hsize_t>( hydeInterpData -> allInterpPtsIndices -> cols ) };
				DataSpace 		numInterpPtsSpace( 	2, 
													numInterpPtsIntSize );
				
				dataset 									= new 	DataSet( 
																				group2.createDataSet(  	"interpPtsIndices", 
																										PredType::NATIVE_INT, 
																										numInterpPtsSpace ) 
																			);

				// Dynamically allocate memory for arrays to avoid stack overflow
				totalSize 													= static_cast<size_t>(hydeInterpData -> allInterpPtsIndices -> rows) 
																								* hydeInterpData -> allInterpPtsIndices -> cols;
				int 				*interpPtsIndices_Placeholder 			= new int[ totalSize ];
				
				for( int i = 0; i < ( hydeInterpData -> allInterpPtsIndices -> rows ); i++ )
				{
					for( int j = 0; j < ( hydeInterpData -> allInterpPtsIndices -> cols ); j++ )
					{
						size_t 		index 				= static_cast<size_t>(i) * hydeInterpData -> allInterpPtsIndices -> cols + j;
						interpPtsIndices_Placeholder[ index ] 		= hydeInterpData 
																			-> allInterpPtsIndices 
																				-> data[ i ][ j ];
					}
				}

				dataset ->      write(  interpPtsIndices_Placeholder, 
										PredType::NATIVE_INT );

				// Clean up dynamically allocated memory
				delete[] interpPtsIndices_Placeholder;


				delete 			dataset;
				/////////////////////////////////////////////////////////////////////////////////////
				/////////////////////////////////////////////////////////////////////////////////////
				dataset 									= new 	DataSet( 
																				group2.createDataSet(  	"interpPtsCoords", 
																										PredType::NATIVE_DOUBLE, 
																										numInterpPtsSpace ) 
																			);

				// Dynamically allocate memory for arrays to avoid stack overflow
				totalSize 													= static_cast<size_t>(hydeInterpData -> allInterpPtsCoords -> rows) 
																								* hydeInterpData -> allInterpPtsCoords -> cols;
				double 				*interpPtsCoords_Placeholder 			= new double[ totalSize ];

				for( int i = 0; i < ( hydeInterpData -> allInterpPtsCoords -> rows ); i++ )
				{
					for( int j = 0; j < ( hydeInterpData -> allInterpPtsCoords -> cols ); j++ )
					{
						size_t 		index 				= static_cast<size_t>(i) * hydeInterpData -> allInterpPtsCoords -> cols + j;

						interpPtsCoords_Placeholder[ index ] 		= hydeInterpData 
																			-> allInterpPtsCoords 
																				-> data[ i ][ j ];
					}
				}

				dataset ->      write(  interpPtsCoords_Placeholder, 
										PredType::NATIVE_DOUBLE );

				// Clean up dynamically allocated memory
				delete[] interpPtsCoords_Placeholder;

				delete 			dataset;
				/////////////////////////////////////////////////////////////////////////////////////
				/////////////////////////////////////////////////////////////////////////////////////
				dataset 									= new 	DataSet( 
																				group2.createDataSet(  	"sigPtsIndices", 
																										PredType::NATIVE_INT, 
																										numInterpPtsSpace ) 
																			);
				// Dynamically allocate memory for arrays to avoid stack overflow
				totalSize 													= static_cast<size_t>(hydeInterpData -> allSigPtsIndices -> rows) 
																								* hydeInterpData -> allSigPtsIndices -> cols;
				int 				*sigInterpGrid_Placeholder 			= new int[ totalSize ];

				for( int i = 0; i < ( hydeInterpData -> allSigPtsIndices -> rows ); i++ )
				{
					for ( int j = 0; j < ( hydeInterpData -> allSigPtsIndices -> cols ); j++ )
					{
						size_t 		index 						= static_cast<size_t>(i) * hydeInterpData -> allSigPtsIndices -> cols + j;

						sigInterpGrid_Placeholder[ index ] 		= hydeInterpData 
																		-> allSigPtsIndices 
																			-> data[ i ][ j ];
					}
				}

				dataset ->      write(  sigInterpGrid_Placeholder, 
										PredType::NATIVE_INT );

				// Clean up dynamically allocated memory
				delete[] sigInterpGrid_Placeholder;

				delete 			dataset;
				/////////////////////////////////////////////////////////////////////////////////////
				/////////////////////////////////////////////////////////////////////////////////////


				hsize_t   		numInterpPtsIntSize2[] 			= { static_cast<hsize_t>( hydeInterpData -> sigGridNumbers -> size ) };
				DataSpace 		numInterpPtsSpace2( 1, 
													numInterpPtsIntSize2 );

				dataset 									= new 	DataSet( 
																				group2.createDataSet(  	"interpWidth", 
																										PredType::NATIVE_INT, 
																										numInterpPtsSpace2 ) 
																			);
				// Not sure why this is needed for now...
				int 			interpWidth_Placeholder[ hydeInterpData -> interpWidth -> size ];
				for( int i = 0; i < ( hydeInterpData -> interpWidth -> size ); i++ )
				{
					interpWidth_Placeholder[ i ] 		= hydeInterpData 
															-> interpWidth 
																-> data[ i ];
				}

				dataset ->      write(  &interpWidth_Placeholder, 
										PredType::NATIVE_INT );

				delete 			dataset;
				/////////////////////////////////////////////////////////////////////////////////////
				/////////////////////////////////////////////////////////////////////////////////////


				dataset 									= new 	DataSet( 
																				group2.createDataSet(  	"sigGridNumbers", 
																										PredType::NATIVE_INT, 
																										numInterpPtsSpace2 ) 
																			);
				// Not sure why this is needed for now...
				int 			sigGridNumbers_Placeholder[ hydeInterpData -> sigGridNumbers -> size ];
				for( int i = 0; i < ( hydeInterpData -> sigGridNumbers -> size ); i++ )
				{
					sigGridNumbers_Placeholder[ i ] 			= hydeInterpData 
																	-> sigGridNumbers 
																		-> data[ i ];  // Assuming sigGridNumbers is 1D
				}

				dataset ->      write(  &sigGridNumbers_Placeholder, 
										PredType::NATIVE_INT );
				delete 			dataset;
			}


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
	int 		numOfGrids 				= ( compositeGrid.numberOfComponentGrids() == 0 ) ? 1 : compositeGrid.numberOfComponentGrids();

	int 		numOfDimensions       	= compositeGrid.numberOfDimensions();


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
	hydeCompositeGrid 
		-> hydeInterpData            	= new HydeInterpData*[ numOfGrids ];				


	
	// Get number of interp pts from CompositeGrid
	const IntegerArray 		&numInterpPts 		= compositeGrid.numberOfInterpolationPoints;
	const IntegerArray 		&interpWidth2 		= compositeGrid.interpolationWidth;
	
	for ( int gridIndex = 0; gridIndex < numOfGrids; gridIndex++ )
	{
		HydeGridData 			*hydeGridData 			= new HydeGridData( gridIndex );
		hydeGridData -> dim    							= numOfDimensions;


		// Utilize Overture methods to easily retrieve data
		MappedGrid 				&grid 					= compositeGrid[ gridIndex ];
		grid.update( 	MappedGrid::THEvertex 
					| 	MappedGrid::THEmask 
					| 	MappedGrid::THEinverseVertexDerivative
					| 	MappedGrid::THEvertexDerivative
					| 	MappedGrid::THEvertexJacobian );  // create the vertex and mask arrays

		// const GenericGrid::GridTypeEnum   &gridType 	= grid.getGridType();

		const IntegerArray 		&gridDimensions 		= grid.dimension();          // grid array dimensions
		const IntegerArray 		&gridIndexRange 		= grid.gridIndexRange();
    	const IntegerArray 		&extIndexRange 			= grid.extendedIndexRange();
		const IntegerArray 		&bc             		= grid.boundaryCondition();  // unused for now

		const RealArray 		&det 					= grid.vertexJacobian(  ); // get the vertex jacobian determinant
		const RealArray 		&rx 					= grid.vertexDerivative();
		const RealArray 		&rx_inv 				= grid.inverseVertexDerivative();

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

		for ( int j = 0; j < 3; j++ )
		{
			hydeGridData 
				-> gridSpacing[ j ] 			= std::isfinite( gridSpacing( j ) ) ? gridSpacing( j ) : 0.0;
			

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
			}

            // Set numbers of relevant grid pts data.
            hydeGridData -> N[ j ]                  = (     gridIndexRange( 1, j )
                                                    	-   gridIndexRange( 0, j )    )   +   1;

            hydeGridData -> NP[ j ]                 = (     gridDimensions( 1, j )
                                                    	-   gridDimensions( 0, j )    )   +   1;

            // Check for overflow in npoints calculation
            if ( hydeGridData -> npoints  >   ( INT_MAX / hydeGridData -> NP[ j ] ) ) 
            {
                throw std::overflow_error( "npoints multiplication would overflow int" );
            }

            hydeGridData -> npoints                 = ( hydeGridData -> npoints ) * ( hydeGridData -> NP[ j ] );
		}

		for ( int d = hydeGridData -> dim; d < 3; d++ )
		{
			hydeGridData 
				-> dx[ d ] 						= 0.0;
			hydeGridData 
				-> gridSpacing[ d ] 			= 0.0;
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
		int 		gridSize3 			= ( I3.getBound() - gridDimensions( 0, 2 ) ) - ( I3.getBase() - gridDimensions( 0, 2 ) ) + 1;

		hydeGridData 
				-> arrayMask  
					-> allocate( 	gridSize1, 
									gridSize2,
									gridSize3 );

		hydeGridData 
				-> xy	
					-> allocate( 	gridSize1, 
									gridSize2,
									gridSize3,
									numOfDimensions );

		hydeGridData 
				-> rx	
					-> allocate( 	gridSize1, 
									gridSize2, 
									gridSize3,
									numOfDimensions );
		hydeGridData
				-> rx -> setIdentity();  // initialize to I

		hydeGridData 
				-> rx_inv	
					-> allocate( 	gridSize1, 
									gridSize2, 
									gridSize3,
									numOfDimensions );
		hydeGridData
				-> rx_inv -> setIdentity();  // initialize to I

		hydeGridData 
				-> jacobDet -> allocate( gridSize1, gridSize2, gridSize3 );

		hydeGridData 
				-> jacobDet -> fill( 1.0 );  // initialize to 1s
		

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
				int 		index3 			= i3 - gridDimensions( 0, 2 );

				// printf( "(%i, %i, %i), (%i, %i), %i ( i1, i2, i3, index1, index2, m )\n", 
				// 		i1, i2, i3, index1, index2, m );

				hydeGridData 
					-> arrayMask  
						-> data[ index1 ][ index2 ][ index3 ]   		= m;
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
				int 		index3 			= i3 - gridDimensions( 0, 2 );

				// printf( "(%i, %i, %i), (%i, %i) ( i1, i2, i3, index1, index2 )\n", 
				// 		i1, i2, i3, index1, index2 );

				for ( int k = 0; k < numOfDimensions; k++ )
				{
					hydeGridData 
							-> xy 
								-> data[ index1 ][ index2 ][ index3 ][ k ]  		= vertexLocal( i1, i2, i3, k );
				}

				for ( int dir = 0; dir < numOfDimensions; dir++ ) // x, y, z
				{
					for ( int var = 0; var < numOfDimensions; var++ ) // r, s, t
					{
						hydeGridData 
								-> rx 
									-> data[ index1 ][ index2 ][ index3 ][ dir ][ var ] 		= RXI( i1, i2, i3, var , dir);
						hydeGridData 
								-> rx_inv 
									-> data[ index1 ][ index2 ][ index3 ][ dir ][ var ] 		= rx_inv( i1, i2, i3, numOfDimensions * dir + var );
					}
				}

				hydeGridData 
						-> jacobDet 
							-> data[ index1 ][ index2 ][ index3 ]  		= det( i1, i2, i3 );

			}
			// printf( "\n" );

		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		
		// Process collected values to populate remaining grid data
		hydeGridData -> processGridData();


		hydeCompositeGrid 
				-> hydeGridData[ gridIndex ] 			= hydeGridData;



		///////////////////////////////////////////////////////////////////////////////////////////////////////
		// Collect all interp grid data for this grid /////////////////////////////////////////////////////////

		HydeInterpData 			*hydeInterpData 			= new HydeInterpData( gridIndex );

		if(  numInterpPts( gridIndex ) > 0  )
		{
			intSerialArray    sigInterpGrid, interpWidth, interpPtsIndices, sigPtsIndices;
			realSerialArray   interpPtsCoords;

			// const IntegerArray &interpWidth2 = compositeGrid.interpolationWidth[ gridIndex ];

			if(  compositeGrid->localInterpolationDataState == CompositeGridData::noLocalInterpolationData  )
			{
				printF( "Found interpolationPoint\n" );
			
				// If the grid was written in serial,  the interpolation arrays are saved here:
				interpPtsIndices.reference 	( compositeGrid.interpolationPoint[ gridIndex ].getLocalArray(  ) );
				sigPtsIndices.reference 	( compositeGrid.interpoleeLocation[ gridIndex ].getLocalArray(  ) );
				sigInterpGrid.reference 	( compositeGrid.interpoleeGrid[ gridIndex ].getLocalArray(  ) );
				interpWidth.reference		( compositeGrid.variableInterpolationWidth[ gridIndex ].getLocalArray(  ) );
				// interpWidth2.reference		( compositeGrid.interpolationWidth[ gridIndex ].getLocalArray(  ) );
				interpPtsCoords.reference 	( compositeGrid.interpolationCoordinates[ gridIndex ].getLocalArray(  ) );
			}
			else
			{
				// If the grid was written in parallel,  the interpolation arrays are saved as separate serial arrays:
				printF( "Found interpolationPointLocal\n" );

				interpPtsIndices.reference 	( compositeGrid->interpolationPointLocal[ gridIndex ] );
				sigPtsIndices.reference 	( compositeGrid->interpoleeLocationLocal[ gridIndex ] );
				sigInterpGrid.reference 	( compositeGrid->interpoleeGridLocal[ gridIndex ] );
				interpWidth.reference		( compositeGrid->variableInterpolationWidthLocal[ gridIndex ] );
				interpPtsCoords.reference 	( compositeGrid->interpolationCoordinatesLocal[ gridIndex ] );
			}
			
			if(  false  )
			{ 
				// show how to copy the interpolation point data to one processor
				Index Iv[2];
				Iv[0] = interpPtsIndices.dimension( 0 );
				Iv[1] = interpPtsIndices.dimension( 1 );

				// create an aggregate array on proc. 0 holding all ip values
				int p0 = 0;   
				IntegerArray ip0;
				CopyArray::getAggregateArray(  interpPtsIndices,  Iv,  ip0, p0 );
				// fprintf( file, "%i ( number of interpolation points in the aggregate array )\n", ip0.getLength( 0 ) );

				// for(  int i = ip0.getBase( 0 ); i <= ip0.getBound( 0 ); i++  )
				// {
				// 	fprintf( file, "%i %i %i  ( interpPtsIndices )\n", 
				// 				ip0( i, 0 ), ip0( i, 1 ), ( numberOfDimensions == 2 ? 0 : ip0( i, 2 ) ) 
				// 			);
				// }

			}

			// sigInterpGrid : donor grid 
			// interpWidth : interpolation width 
			// interpPtsIndices : interpolation point ( on grid )
			// sigPtsIndices : lower left corner of donor stencil ( on the donor grid )
			// interpPtsCoords : unit square coordinates of the interpolation point in the donor grid

			int 	numInterpPtsLocal 		= interpPtsIndices.getLength( 0 );  // number of interpolation points on this processor

			hydeInterpData
				-> numOfInterpPts 
											= numInterpPtsLocal;


			hydeInterpData 
				-> allInterpPtsIndices 
					-> allocate( numInterpPtsLocal, numOfDimensions ); 
			hydeInterpData 
				-> allSigPtsIndices 
					-> allocate( numInterpPtsLocal, numOfDimensions ); 
			hydeInterpData 
				-> allInterpPtsCoords 
					-> allocate( numInterpPtsLocal, numOfDimensions );
			hydeInterpData 
				-> sigGridNumbers 
					-> allocate( numInterpPtsLocal );
			hydeInterpData 
				-> interpWidth 
					-> allocate( numInterpPtsLocal );

			for(  int i = interpPtsIndices.getBase( 0 ); i <= interpPtsIndices.getBound( 0 ); i++  )
			{
				for ( int d = 0; d < numOfDimensions; d++ )
				{
					// interpPtsIndices is 2D or 3D
					hydeInterpData 
						-> allInterpPtsIndices
							-> data[ i ][ d ] 		= interpPtsIndices( i, d );
					// sigPtsIndices is 2D or 3D
					hydeInterpData 
						-> allSigPtsIndices
							-> data[ i ][ d ] 		= sigPtsIndices( i, d );
					// interpPtsCoords is 2D or 3D
					hydeInterpData 
						-> allInterpPtsCoords
							-> data[ i ][ d ] 		= interpPtsCoords( i, d );
					// sigInterpGrid is 1D
					hydeInterpData 
						-> sigGridNumbers
							-> data[ i ] 			= sigInterpGrid( i );
					// interpWidth is 1D
					hydeInterpData 
						-> interpWidth
							-> data[ i ] 			= interpWidth( i );
				}
				
				// fprintf(  file, "%i,  %i,  (%i,  %i,  %i),   (%i,  %i,  %i),   (%i,  %i,  %i),  (%e,  %e,  %e) ( donor,  width,  interpPtsIndices,  sigPtsIndices,  interpPtsCoords )\n", 
				// 			sigInterpGrid( i )   ,    interpWidth( i ), 
				// 			interpPtsIndices( i, 0 ),  interpPtsIndices( i, 1 ),  ( numberOfDimensions==2 ? 0 : interpPtsIndices( i, 2 ) ), 
				// 			sigPtsIndices( i, 0 ),  sigPtsIndices( i, 1 ),  ( numberOfDimensions==2 ? 0 : sigPtsIndices( i, 2 ) ), 
				// 			interpPtsCoords( i, 0 ),  interpPtsCoords( i, 1 ),  ( numberOfDimensions==2 ? 0.0 : interpPtsCoords( i, 2 ) ) 
					// );
			}

		}

		hydeCompositeGrid 
				-> hydeInterpData[ gridIndex ] 			= hydeInterpData;
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////

		// destroy arrays to save space
		grid.destroy( 	MappedGrid::THEvertex 
					| 	MappedGrid::THEmask 
					| 	MappedGrid::THEinverseVertexDerivative 
					| 	MappedGrid::THEvertexJacobian );  
	}

	return 0;
}