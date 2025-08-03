#pragma once

#include <iostream>
#include <stdexcept>


template <typename T>
struct Array2D 
{
    T** data;
    int rows;
    int cols;

	int *dim2;

    // Constructor
    Array2D() : data( nullptr ), rows( 0 ), cols( 0 ), dim2( nullptr ) {}

    // Destructor
    ~Array2D() 
    {
        if (data) 
        {
            for ( int i = 0; i < rows; ++i ) 
            {
                delete[] data[ i ];
            }
            delete[] data;
        }
    }

    // Method to allocate the array and set dimensions
    void allocate( int r, int c ) 
    {
        rows        	= r;
        cols       		= c;
		dim2	   		= new int[ rows ];

        data        	= new T*[ rows ];

        for ( int i = 0; i < rows; ++i ) 
        {
			dim2[ i ] 	= c;
            data[ i ] 	= new T[ cols ];
        }
    }

    void allocate( int r, int *c ) 
    {
        rows        = r;
		dim2	   	= new int[ rows ];

        data        = new T*[ rows ];

        for ( int i = 0; i < rows; ++i ) 
        {
			dim2[ i ] 		= c[ i ];
            data[ i ] 		= new T[ dim2[ i ] ];
        }
    }

    // Method to fill the array with a single value
    void fill( const T& value ) 
    {

        for (int i = 0; i < rows; ++i) 
        {
            for (int j = 0; j < dim2[ i ]; ++j) 
            {
                data[ i ][ j ] = value;
            }
        }

    }

    void setIdentity()
    {


        for ( int i = 0; i < rows; ++i ) 
        {
			if ( rows != dim2[ i ] ) 
			{
				throw std::invalid_argument( "Array must be square to set identity" );
			}

            for ( int j = 0; j < dim2[ i ]; ++j ) 
            {
                data[ i ][ j ]      = ( i == j ) ? 1 : 0;
            }
        }

    }

    // Method to deallocate the array
    void deallocate(  ) 
    {

        if ( data ) 
        {
            for ( int i = 0; i < rows; ++i ) 
            {
				dim2[ i ] = 0; // Reset cols to zero
                delete[] data[ i ];
            }
            delete[] data;
            data = nullptr;
            rows = cols = 0;
            std::cout << "Deallocated Array2D" << std::endl; // Debugging
        }

    }

    // Access operator
    T* operator[]( int index ) 
    {

        if ( index < 0 || index >= rows ) 
        {
            throw std::out_of_range( "Index out of range" );
        }

        return data[ index ];

    }

    // Const access operator
    const T* operator[]( int index ) const 
    {

        if ( index < 0 || index >= rows ) 
        {
            throw std::out_of_range( "Index out of range" );
        }

        return data[ index ];

    }

    // Access operator
    T& operator()( int i, int j ) 
	{

		if ( i < 0 || i >= rows || j < 0 || j >= dim2[ i ] ) 
		{
			throw std::out_of_range( "Index out of range" );
		}

		return data[ i ][ j ];

	}

    // Const access operator
    const T& operator()( int i, int j ) const 
	{

		if ( i < 0 || i >= rows || j < 0 || j >= dim2[ i ] ) 
		{
			throw std::out_of_range( "Index out of range" );
		}

		return data[ i ][ j ];

	}

};
