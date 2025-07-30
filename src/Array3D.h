#pragma once


#include <iostream> // For debugging
#include <stdexcept>


template <typename T>
struct Array3D 
{
    T*** 	data;
    int 	dim1 		= 0;

	int 	i2, i3 		= -1;
	int 	dim2[ 3 ] 	= {0, 0, 0};
	int 	dim3[ 3 ] 	= {0, 0, 0};

    // Constructor
    Array3D() : data( nullptr ), dim1( 0 ), i2( -1 ), i3( -1 ), dim2{ 0, 0, 0 }, dim3{ 0, 0, 0 } {}

    // Destructor
    ~Array3D(  ) 
    {
        deallocate(  );
    }

    // Method to allocate the array and set dimensions
    void allocate(  int     d1, int     d2, int     d3, 
                    int index1, int index2 ) 
    {
        if (    ( d1 < 0 || d2 < 0 || d3 < 0 )
                || 
                ( d1 == 0 && d2 == 0 && d3 == 0 ) ) 
        {
            std::cerr << "Invalid dimensions for allocation: " << d1 << ", " << d2 << ", " << d3 << std::endl;
            std::exit( 1 );
        }

        // Positive d1, d2, d3 indicate which dimension is being set
        if ( d1 > 0 ) 
        {
            dim1 = d1;
        }
        if ( d2 > 0 ) 
        {
			i2 += 1;
            dim2[ i2 ] = d2;
        }
        if ( d3 > 0 ) 
        {
			i3 += 1;
            dim3[ i3 ] = d3;
        }
		

        // Allocate 1st pointer memory
        if ( d1 > 0 )
        {
            data = new T**[ dim1 ];
        }
        // Allocate 2nd pointer memory
        if ( d2 > 0 && dim1 > 0 )
        {
            if ( index1 != -1 && d1 == 0 )
            {
                data[ index1 ] 				= new T*[ d2 ];
				dim2[ index1 ] 				= d2; // Store dimension for index1

                std::cout << "Allocated space to pointer at data[" << index1 << "]" << std::endl; // Debugging
            }
            else
            {
                for ( int i = 0; i < dim1; ++i ) 
                {
                    data[ i ] 			= new T*[ d2 ];
					dim2[ i ] 			= d2; // Store dimension for each i
                }

                std::cout << "Allocated space of dimensions " << dim1 << "x" << d2 << std::endl; // Debugging
            }
        }
        // Allocate 3rd pointer memory
        if ( d3 > 0 && dim2[ i2 ] > 0 && dim1 > 0 )
        {
            if ( ( index1 != -1 && d1 == 0 ) && ( index2 != -1 && d2 == 0 ) )
            {
                data[ index1 ][ index2 ] 		= new T[ d3 ];
				dim3[ index1 ] 					= d3; // Store dimension for index2

                std::cout << "Allocated space to pointer at data[" << index1 << "][" << index2 << "]" << std::endl; // Debugging
            }
            else if ( index2 != -1 && d2 == 0 )
            {
                for ( int i = 0; i < dim1; ++i ) 
                {
                    data[ i ][ index2 ] 		= new T[ d3 ];
					dim3[ i ] 					= d3; // Store dimension for each i
                }

                std::cout << "Allocated Array3D of dimensions " << dim1 << "x" << 1 << "x" << d3 << std::endl; // Debugging
            }
            else if ( index1 != -1 && d1 == 0 )
            {
                for ( int j = 0; j < dim2[ index1 ]; ++j ) 
                {
                    data[ index1 ][ j ] 			= new T[ d3 ];
					dim3[ index1 ] 					= d3; // Store dimension for index1
                }

                std::cout << "Allocated Array3D of dimensions " << 1 << "x" << dim2[ index1 ] << "x" << d3 << std::endl; // Debugging
            }
            else
            {
                for ( int i = 0; i < dim1; ++i ) 
                {
                    for (int j = 0; j < dim2[ i ]; ++j ) 
                    {
                        data[ i ][ j ] 				= new T[ d3 ];
						dim3[ i ] 					= d3; // Store dimension for each i

                    }

                	std::cout << "Allocated Array3D of dimensions " << dim1 << "x" << dim2[ i ] << "x" << d3 << std::endl; // Debugging
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
                for ( int j = 0; j < dim2[ i ]; ++j ) 
                {
                    delete[] data[ i ][ j ];
                }

                delete[] data[ i ];
				dim2[ i ] = dim3[ i ] = 0;
            }

            delete[] data;
            data = nullptr;
            dim1 = 0;
            std::cout << "Deallocated Array3D" << std::endl; // Debugging
        }
    }

    // Access operator
    T& operator(  )( int i, int j, int k ) 
    {
        if ( i < 0 || i >= dim1 || j < 0 || j >= dim2[ i ] || k < 0 || k >= dim3[ i ] ) 
        {
            throw std::out_of_range( "Index out of range" );
        }
        return data[ i ][ j ][ k ];
    }

    // Const access operator
    const T& operator(  )( int i, int j, int k ) const 
    {
        if ( i < 0 || i >= dim1 || j < 0 || j >= dim2[ i ] || k < 0 || k >= dim3[ i ] ) 
        {
            throw std::out_of_range( "Index out of range" );
        }
        return data[ i ][ j ][ k ];
    }
};
