#include <iostream> // For catching errors
#include <stdexcept>

#include "HydeGridData.h"
#include "Array2D.h"
#include "Array3D.h"
#include "Array4D.h"



HydeGridData::HydeGridData( int _gridNumber )
{
	gridNumber              = _gridNumber;
	
    ///////////////////////////////////////////////////////////////////
	// Initialize mask. ///////////////////////////////////////////////
    arrayMask          		= new Array2D< int >();
    xy          			= new Array3D< double >();
    ///////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    // Initialize matrixMap and matrixMapInv. /////////////////////////////////
    // All maps are identity for single grid.
    // matrixMap          		= new Array2D< double >();
    rx          			= new Array4D< double >();
    rx_inv          		= new Array4D< double >();
    matrixMapInv          	= new Array2D< double >();

    // matrixMap       -> allocate( 4, 4 );
    matrixMapInv    -> allocate( 4, 4 );

    // matrixMap       -> setIdentity();
    matrixMapInv    -> setIdentity();
    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
}
