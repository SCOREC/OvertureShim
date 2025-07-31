#pragma once


// Forward declaration
// class SigInterpPtsData;
template <class T> class Array2D;
// template <class T> class Array1D;


class HydeInterpData
{
	public:

		int                                   gridNumber;

		// Collected after above
		bool                                  gridHasHydeInterpData                   = false;

		Array2D< double >                     *allInterpPtsCoords;
		Array2D< int >                        *allInterpPtsIndices;

		Array2D< int >                        *allSigPtsIndices;

		Array2D< int >                        *interpWidth;
		Array2D< int >                        *stencilSizes;

		// This is the corresponding grid number for each significant point,
		// ordered by address placement for the incoming grid indices.
		// However, it avoids confusion to sort these afterwards rather than
		// pass this around everywhere.
		// Array1D< int >                        *gridsSharingOL;

		// Array1D< int >                        *sigGridNumbers;


		// SigInterpPtsData                      **sigInterpPtsData;



		// InterpolationOptions                  interpOpt;


		HydeInterpData( int _gridNumber );

};



