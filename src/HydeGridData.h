#pragma once


// Forward declaration
template <class T> class Array2D;
template <class T> class Array3D;
template <class T> class Array4D;


class HydeGridData
{
	public:

		std::string 			gridName;
		std::string 			gridType;
		// GridType                gridType                = GridType::Cartesian;

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
		double    				dx[ 3 ];

		int     				boundaryCondition[ 2 ][ 3 ];
		// BCConditionType       	bcsAtGrid[ 2 ][ 3 ];


		HydeGridData( int _gridNumber );

		void 		setGridType();
		void 		setInteriorBox();
		void 		setDx();
		void 		processGridData();


		// Define a Kokkos View to store arrayMask used points data
		Array2D< int >    		*arrayMask; // arrayData.p0
			
		// Array4D< double >    	*matrixMap          	= new Array4D< double >();
		// Array4D< double >    	*matrixMapInv          	= new Array4D< double >();

		Array4D< double >    	*rx;
		Array4D< double >    	*rx_inv;
		// Array2D< double >    	*matrixMapInv;

		Array2D< double > 		*jacobDet;

		Array3D< double >    	*xy; // arrayData.p1
};