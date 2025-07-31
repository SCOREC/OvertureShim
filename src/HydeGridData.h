#pragma once


// Forward declaration
template <class T> class Array2D;
template <class T> class Array3D;


class HydeGridData
{
	public:

		std::string 			gridName;
		int       				gridNumber;
		int       				dim;


		//Kokkos::View< const double[ 2 ][ 3 ], HYDE_DEVICE_LAYOUT, HYDE_DEVICE >  interiorBox;
		double    				interiorBox[ 2 ][ 3 ];
		double    				boundingBox[ 2 ][ 3 ];

		int       				N[ 3 ];
		int       				NP[ 3 ]; // Number of axis grid pts including ghosts.
		int 					npoints                     	= 1; // Total number of grid pts initialized to 1.

		int       				numOfGhostPts[ 2 ][ 3 ];
		bool       				useGhostPts;


		int       				gridIndexRange[ 2 ][ 3 ];
		int       				extGridIndexRange[ 2 ][ 3 ];

		// All grid disc sizes can be derived from spacing.
		double    				gridSpacing[ 3 ];

		int     				boundaryCondition[ 2 ][ 3 ];
		// BCConditionType       	bcsAtGrid[ 2 ][ 3 ];

		// GridType                gridType                = GridType::Cartesian;

		HydeGridData( int _gridNumber );
		

		// Define a Kokkos View to store arrayMask used points data
		Array2D< int >    		*arrayMask; // arrayData.p0
			
		// Array4D< double >    	*matrixMap          	= new Array4D< double >();
		// Array4D< double >    	*matrixMapInv          	= new Array4D< double >();

		Array2D< double >    	*matrixMap;
		Array2D< double >    	*matrixMapInv;

		Array3D< double >    	*xy; // arrayData.p1
};