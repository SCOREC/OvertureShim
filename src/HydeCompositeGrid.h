#pragma once


#include <string>

// Forward declaration
class HydeGridData;
class HydeInterpData;


class HydeCompositeGrid
{
	public:

		const char 			*fileName;

		int           	dimOfProblem;
		int           	numOfGrids;

		HydeGridData      	**hydeGridData;
		HydeInterpData    	**hydeInterpData;

		HydeCompositeGrid( 	const char 		*_fileName,
							int 			_numOfGrids,
							int 			_numOfDimensions );

};

