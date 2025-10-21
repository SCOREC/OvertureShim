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
    arrayMask          		= new Array3D< int >();
    xy          			= new Array3D< double >();
    ///////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
    // Initialize matrixMap and matrixMapInv. /////////////////////////////////
    // All maps are identity for single grid.
    // matrixMap          		= new Array2D< double >();
    rx          			= new Array4D< double >();
    rx_inv          		= new Array4D< double >();

	jacobDet 				= new Array2D< double >();

    // matrixMap       -> allocate( 4, 4 );
    // matrixMapInv    -> allocate( 4, 4 );

    // matrixMap       -> setIdentity();
    // matrixMapInv    -> setIdentity();
    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
}


// It is preferable if this information can be found within Overture.
void HydeGridData::setGridType()
{
	this -> gridType 			= "Cartesian"; // Default grid type

	for ( int i = 0; i < ( this -> rx -> rows ); i++ )
	{
		for ( int j = 0; j < ( this -> rx -> cols ); j++ )
		{
			for ( int d = 0; d < this -> dim; d++ )
			{
				for ( int d2 = d + 1; d2 < this -> dim; d2++ )
				{
					if ( this -> rx -> data[ i ][ j ][ d ][ d2 ] != 0.0 || 
						 this -> rx -> data[ i ][ j ][ d2 ][ d ] != 0.0 )
					{
						this -> gridType 		= "Curvilinear";
						break;
					}
				}
			}
		}
	}
}


// It is preferable if this information can be found within Overture.
void HydeGridData::setInteriorBox()
{
	// If grid is Cartesian, and interior can be specified from grid spacing.
	if ( this -> gridType == "Cartesian" )
	{
		int startIndex, endIndex;

		startIndex 							= this -> numOfGhostPts[ 0 ][ 0 ];
		endIndex 							= this -> xy -> dim1 - this -> numOfGhostPts[ 1 ][ 0 ] - 1;

		this -> interiorBox[ 0 ][ 0 ] 		= this -> xy -> data[ startIndex ][ 0 ][ 0 ];
		this -> interiorBox[ 1 ][ 0 ] 		= this -> xy -> data[ endIndex ][ 0 ][ 0 ];


		startIndex 							= this -> numOfGhostPts[ 0 ][ 1 ];
		endIndex 							= this -> xy -> dim1 - this -> numOfGhostPts[ 1 ][ 1 ] - 1;

		this -> interiorBox[ 0 ][ 1 ] 		= this -> xy -> data[ 0 ][ startIndex ][ 1 ];
		this -> interiorBox[ 1 ][ 1 ] 		= this -> xy -> data[ 0 ][ endIndex ][ 1 ];
	}
	else
	{
		for ( int i = 0; i < this -> dim; i++ )
		{
			this -> interiorBox[ 0 ][ i ] 	= 0.0;
			this -> interiorBox[ 1 ][ i ] 	= 1.0;
		}
	}

}


void HydeGridData::setDx()
{

	// If grid is Cartesian, and interior can be specified from grid spacing.
	if ( this -> gridType == "Cartesian" )
	{
		for ( int i = 0; i < this -> dim; i++ )
		{
			this -> dx[ i ] 		= ( this -> interiorBox[ 1 ][ i ] 
											-   this -> interiorBox[ 0 ][ i ] ) 
												* ( this -> gridSpacing[ i ] );
		}
	}
	else
	{
		for ( int i = 0; i < this -> dim; i++ )
		{
			this -> dx[ i ] 		= this -> gridSpacing[ i ];
		}
	}
}


void HydeGridData::processGridData()
{
	this -> setGridType();
	this -> setInteriorBox();
	this -> setDx();
}