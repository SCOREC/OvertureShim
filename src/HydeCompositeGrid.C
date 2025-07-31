#include <H5Cpp.h>

#include "HydeCompositeGrid.h"

#include "HydeGridData.h"
#include "HydeInterpData.h"


HydeCompositeGrid::HydeCompositeGrid( 	const char 		*_fileName,
										int 			_numOfGrids,
										int 			_numOfDimensions )
{
	this -> fileName               		= _fileName;

	// Instantiate numOfGrids HydeGridData objects for this HydeCompositeGrid
	this -> hydeGridData            	= new HydeGridData*  [ _numOfGrids ];
	// Instantiate numOfGrids HydeInterpData objects for this HydeCompositeGrid
	this -> hydeInterpData         		= new HydeInterpData*[ _numOfGrids ];

	this -> dimOfProblem               	= _numOfDimensions;
}



