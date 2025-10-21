#pragma once

#include <iostream> // For debugging
#include <stdexcept>


template <typename T>
struct Array4D_V3
{
    T**** 		data;

    int 		dim1 			= 0;
	int 		dim2 			= 0;
	int 		dim3 			= 0;
	int 		dim4 			= 0;

    // Constructor
    Array4D_V3() : data( nullptr ), dim1( 0 ), dim2( 0 ), dim3( 0 ), dim4( 0 ) {}

    // Destructor
    ~Array4D_V3() 
    {
        deallocate();
    }

    // Method to allocate the array and set dimensions

    // Allocate the array: [rows][cols][dim][dim]
    void allocate( int r, int c, int d, int l )
    {
        dim1 		= r;
        dim2 		= c;
        dim3 		= d;
        dim4 		= l;

        data 		= new T***[ dim1 ];

        for (int i = 0; i < dim1; ++i)
        {
            data[ i ] = new T**[ dim2 ];

            for (int j = 0; j < dim2; ++j)
            {
                data[ i ][ j ] = new T*[ dim3 ];
				
                for (int k = 0; k < dim3; ++k)
                {
                    data[ i ][ j ][ k ] = new T[ dim4 ];
                }
            }
        }
    }

    // Method to deallocate the array
    void deallocate() 
    {
        if ( data ) 
        {
            for ( int i = 0; i < dim1; ++i ) 
            {
                for ( int j = 0; j < dim2; ++j ) 
                {
                    for ( int k = 0; k < dim3; ++k ) 
                    {
                        delete[] data[ i ][ j ][ k ];
                    }
                    delete[] data[ i ][ j ];
                }

                delete[] data[ i ];
            }

            delete[] data;
            data = nullptr;
            dim1 = dim2 = dim3 = dim4 = 0;
            std::cout << "Deallocated Array4D" << std::endl; // Debugging
        }
    }

    // Access operator
    T& operator()( int i, int j, int k, int l ) 
    {
        if ( i < 0 || i >= dim1 || j < 0 || j >= dim2 || k < 0 || k >= dim3 || l < 0 || l >= dim4 ) 
        {
            throw std::out_of_range( "Index out of range" );
        }

        return data[ i ][ j ][ k ][ l ];
    }

    // Const access operator
    const T& operator()( int i, int j, int k, int l ) const 
    {
        if ( i < 0 || i >= dim1 || j < 0 || j >= dim2 || k < 0 || k >= dim3 || l < 0 || l >= dim4 ) 
        {
            throw std::out_of_range( "Index out of range" );
        }
		
        return data[ i ][ j ][ k ][ l ];
    }
};