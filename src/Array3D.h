#pragma once


#include <iostream> // For debugging
#include <stdexcept>


template <typename T>
struct Array3D 
{
    T*** data;
    int dim1, dim2, dim3;

    // Constructor
    Array3D(  ) : data( nullptr ), dim1( 0 ), dim2( 0 ), dim3( 0 ) {}

    // Destructor
    ~Array3D(  ) 
    {
        deallocate(  );
    }

    // Method to allocate the array and set dimensions
    void allocate( int d1, int d2, int d3 ) 
    {
        if ( d1 <= 0 || d2 <= 0 || d3 <= 0 ) 
        {
            std::cerr << "Invalid dimensions for allocation: " << d1 << ", " << d2 << ", " << d3 << std::endl;
            return;
        }
        dim1 = d1;
        dim2 = d2;
        dim3 = d3;
        data = new T**[ dim1 ];
        for ( int i = 0; i < dim1; ++i ) 
        {
            data[ i ] = new T*[ dim2 ];
            for ( int j = 0; j < dim2; ++j ) 
            {
                data[ i ][ j ] = new T[ dim3 ];
            }
        }
        std::cout << "Allocated Array3D of dimensions " << dim1 << "x" << dim2 << "x" << dim3 << std::endl; // Debugging
    }


    // Method to fill the array with a single value
    void fill( const T& value ) 
    {

        for (int i = 0; i < dim1; ++i) 
        {
            for (int j = 0; j < dim2; ++j) 
            {
                for (int k = 0; k < dim3; ++k) 
                {
                    data[ i ][ j ][ k ] = value;
                }
            }
        }

    }

    // Method to deallocate the array
    void deallocate(  ) 
    {
        if ( data ) 
        {
            for ( int i = 0; i < dim1; ++i ) 
            {
                for ( int j = 0; j < dim2; ++j ) 
                {
                    delete[] data[ i ][ j ];
                }
                delete[] data[ i ];
            }
            delete[] data;
            data = nullptr;
            dim1 = dim2 = dim3 = 0;
            std::cout << "Deallocated Array3D" << std::endl; // Debugging
        }
    }

    // Access operator
    T& operator(  )( int i, int j, int k ) 
    {
        if ( i < 0 || i >= dim1 || j < 0 || j >= dim2 || k < 0 || k >= dim3 ) 
        {
            throw std::out_of_range( "Index out of range" );
        }
        return data[ i ][ j ][ k ];
    }

    // Const access operator
    const T& operator(  )( int i, int j, int k ) const 
    {
        if ( i < 0 || i >= dim1 || j < 0 || j >= dim2 || k < 0 || k >= dim3 ) 
        {
            throw std::out_of_range( "Index out of range" );
        }
        return data[ i ][ j ][ k ];
    }
};
