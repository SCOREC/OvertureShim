#pragma once

#include <iostream> // For debugging
#include <stdexcept>


template <typename T>
struct Array4D 
{
    T**** data;
    int dim1, dim2, dim3, dim4 = 0;

    // Constructor
    Array4D() : data( nullptr ), dim1( 0 ), dim2( 0 ), dim3( 0 ), dim4( 0 ) {}

    // Destructor
    ~Array4D() 
    {
        deallocate();
    }

    // Method to allocate the array and set dimensions
    void allocate(  int     d1, int     d2, int     d3, int     d4, 
                    int index1, int index2, int index3 ) 
    {
        if ( ( d1 < 0 || d2 < 0 || d3 < 0 || d4 < 0 ) 
             || 
             ( d1 == 0 && d2 == 0 && d3 == 0 && d4 == 0 ) ) 
        {
            std::cerr << "Invalid dimensions for allocation: " << d1 << ", " << d2 << ", " << d3 << ", " << d4 << std::endl;
            std::exit( 1 );
        }

        if ( d1 > 0 )
        {
            dim1 = d1;
        }
        if ( d2 > 0 ) 
        {
            dim2 = d2;
        }
        if ( d3 > 0 ) 
        {
            dim3 = d3;
        }
        if ( d4 > 0 ) 
        {
            dim4 = d4;
        }

        // Allocate 1st pointer memory
        if ( d1 > 0 )
        {
            data = new T***[ dim1 ];
        }
        // Allocate 2nd pointer memory
        if ( d2 > 0 && dim1 > 0 )
        {
            if ( index1 != -1 && d1 == 0 )
            {
                data[ index1 ] = new T**[ dim2 ];
                std::cout << "Allocated space to pointer at data[" << index1 << "]" << std::endl; // Debugging
            }
            else
            {
                for ( int i = 0; i < dim1; ++i ) 
                {
                    data[ i ] = new T**[ dim2 ];
                }
                std::cout << "Allocated space of dimensions " << dim1 << "x" << dim2 << std::endl; // Debugging
            }
        }
        // Allocate 3rd pointer memory
        if ( d3 > 0 && dim2 > 0 && dim1 > 0 )
        {
            if ( ( index1 != -1 && d1 == 0 ) && ( index2 != -1 && d2 == 0 ) )
            {
                data[ index1 ][ index2 ] = new T*[ dim3 ];
                std::cout << "Allocated space to pointer at data[" << index1 << "][" << index2 << "]" << std::endl; // Debugging
            }
            else if ( index2 != -1 && d2 == 0 )
            {
                for ( int i = 0; i < dim1; ++i ) 
                {
                    data[ i ][ index2 ] = new T*[ dim3 ];
                }
                std::cout << "Allocated Array4D of dimensions " << dim1 << "x" << dim2 << "x" << dim3 << std::endl; // Debugging
            }
            else if ( index1 != -1 && d1 == 0 )
            {
                for ( int j = 0; j < dim2; ++j ) 
                {
                    data[ index1 ][ j ] = new T*[ dim3 ];
                }
                std::cout << "Allocated Array4D of dimensions " << dim1 << "x" << dim2 << "x" << dim3 << std::endl; // Debugging
            }
            else
            {
                for ( int i = 0; i < dim1; ++i ) 
                {
                    for (int j = 0; j < dim2; ++j ) 
                    {
                        data[ i ][ j ] = new T*[ dim3 ];
                    }
                }
                std::cout << "Allocated Array4D of dimensions " << dim1 << "x" << dim2 << "x" << dim3 << std::endl; // Debugging
            }
        }
        // Allocate 4th pointer memory
        if ( d4 > 0 && dim3 > 0 && dim2 > 0 && dim1 > 0 )
        {
            if ( ( index1 != -1 && d1 == 0 ) && ( index2 != -1 && d2 == 0 ) && ( index3 != -1 && d3 == 0 ) )
            {
                data[ index1 ][ index2 ][ index3 ] = new T[ dim4 ];
                std::cout << "Allocated space to pointer at data[" << index1 << "][" << index2 << "][" << index3 << "]" << std::endl; // Debugging
            } /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            else if ( ( index2 != -1 && d2 == 0 ) && ( index3 != -1 && d3 == 0 ) )
            {
                for ( int i = 0; i < dim1; ++i ) 
                {
                    data[ i ][ index2 ][ index3 ] = new T[ dim4 ];
                }
                std::cout << "Allocated Array4D of dimensions " << dim1 << "x" << dim2 << "x" << dim3 << "x" << dim4 << std::endl; // Debugging
            }
            else if ( ( index1 != -1 && d1 == 0 ) && ( index3 != -1 && d3 == 0 ) )
            {
                for ( int j = 0; j < dim2; ++j ) 
                {
                    data[ index1 ][ j ][ index3 ] = new T[ dim4 ];
                }
                std::cout << "Allocated Array4D of dimensions " << dim1 << "x" << dim2 << "x" << dim3 << "x" << dim4 << std::endl; // Debugging
            }
            else if ( ( index1 != -1 && d1 == 0 ) && ( index2 != -1 && d2 == 0 ) )
            {
                for ( int k = 0; k < dim3; ++k ) 
                {
                    data[ index1 ][ index2 ][k] = new T[ dim4 ];
                }
                std::cout << "Allocated Array4D of dimensions " << dim1 << "x" << dim2 << "x" << dim3 << "x" << dim4 << std::endl; // Debugging
            } ////////////////////////////////////////////////////////////////////////////////////////////////////////////
            else if ( ( index1 != -1 && d1 == 0 ) )
            {
                for ( int j = 0; j < dim2; ++j ) 
                {
                    for ( int k = 0; k < dim3; ++k ) 
                    {
                        data[ index1 ][ j ][k] = new T[ dim4 ];
                    }
                }
                std::cout << "Allocated Array4D of dimensions " << dim1 << "x" << dim2 << "x" << dim3 << "x" << dim4 << std::endl; // Debugging
            }
            else if ( ( index2 != -1 && d2 == 0 ) )
            {
                for ( int i = 0; i < dim1; ++i ) 
                {
                    for ( int k = 0; k < dim3; ++k ) 
                    {
                        data[ i ][ index2 ][k] = new T[ dim4 ];
                    }
                }
                std::cout << "Allocated Array4D of dimensions " << dim1 << "x" << dim2 << "x" << dim3 << "x" << dim4 << std::endl; // Debugging
            }
            else if ( ( index3 != -1 && d3 == 0 ) )
            {
                for ( int i = 0; i < dim1; ++i ) 
                {
                    for ( int j = 0; j < dim2; ++j ) 
                    {
                        data[ i ][ j ][index3] = new T[ dim4 ];
                    }
                }
                std::cout << "Allocated Array4D of dimensions " << dim1 << "x" << dim2 << "x" << dim3 << "x" << dim4 << std::endl; // Debugging
            } /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            else
            {
                for ( int i = 0; i < dim1; ++i ) 
                {
                    for ( int j = 0; j < dim2; ++j ) 
                    {
                        for ( int k = 0; k < dim3; ++k ) 
                        {
                            data[ i ][ j ][k] = new T[ dim4 ];
                        }
                    }
                }
                std::cout << "Allocated Array4D of dimensions " << dim1 << "x" << dim2 << "x" << dim3 << "x" << dim4 << std::endl; // Debugging
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
                        delete[] data[ i ][ j ][k];
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
        return data[ i ][ j ][k][l];
    }

    // Const access operator
    const T& operator()( int i, int j, int k, int l ) const 
    {
        if ( i < 0 || i >= dim1 || j < 0 || j >= dim2 || k < 0 || k >= dim3 || l < 0 || l >= dim4 ) 
        {
            throw std::out_of_range( "Index out of range" );
        }
        return data[ i ][ j ][k][l];
    }
};