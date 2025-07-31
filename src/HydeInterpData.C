#include <algorithm>

#include "HydeInterpData.h"

#include "Array2D.h"
// #include "Array1D.h"


HydeInterpData::HydeInterpData( int _gridNumber )
{
    gridNumber                          = _gridNumber;

    allInterpPtsCoords                  = new Array2D< double >();
    allInterpPtsIndices                 = new Array2D< int >();
    allSigPtsIndices                    = new Array2D< int >();

    interpWidth                         = new Array2D< int >();
    stencilSizes                        = new Array2D< int >();

    // gridsSharingOL                      = new Array1D< int >();
    //sigGridNumbers                  = new Array1D< int >();
}