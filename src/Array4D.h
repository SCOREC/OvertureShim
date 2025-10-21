#pragma once

#include <iostream>
#include <stdexcept>

template <typename T>
struct Array4D
{
    T***** data;
    int rows;
    int cols;
	int depth;

	int dim;

    // Constructor
    Array4D() : data(nullptr), rows(0), cols(0), depth(0), dim(0) {}

    // Destructor
    ~Array4D()
    {
        deallocate();
    }

    // Allocate the array: [rows][cols][dim][dim]
    void allocate( int r, int c, int d, int di )
    {
        rows 		= r;
        cols 		= c;
		depth 		= d;
        dim 		= di;

        data 		= new T****[rows];

        for ( int i = 0; i < rows; ++i )
        {
            data[i] = new T***[cols];
            for ( int j = 0; j < cols; ++j )
            {
                data[i][j] = new T**[depth];
                for ( int k = 0; k < depth; ++k )
                {
                    data[i][j][k] = new T*[dim];
					for ( int l = 0; l < dim; ++l )
					{
						data[i][j][k][l] = new T[dim];
					}
                }
            }
        }
    }

    // Fill all elements with a value
    void fill( const T& value )
    {
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
				for (int k = 0; k < depth; ++k)
					for (int l = 0; l < dim; ++l)
						for (int m = 0; m < dim; ++m)
							data[i][j][k][l][m] = value;
    }

    // Set identity matrix at every grid point
    void setIdentity()
    {
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
				for (int k = 0; k < depth; ++k)
					for (int l = 0; l < dim; ++l)
						for (int m = 0; m < dim; ++m)
							data[i][j][k][l][m] 	= (l == m) ? 1 : 0;
    }

    // Deallocate the array
    void deallocate()
    {
        if (data)
        {
            for (int i = 0; i < rows; ++i)
            {
                for (int j = 0; j < cols; ++j)
                {
					for (int k = 0; k < depth; ++k)
					{
						for (int l = 0; l < dim; ++l)
						{
							delete[] data[i][j][k][l];
						}
						delete[] data[i][j][k];
					}
					delete[] data[i][j];
                }
                delete[] data[i];
            }
            delete[] data;
            data = nullptr;
            rows = cols = depth = dim = 0;
            std::cout << "Deallocated Array4D" << std::endl;
        }
    }

    // Access method: returns pointer to [dim][dim] matrix at (i, j, k) grid index
    T** at( int i, int j, int k )
    {
        if (i < 0 || i >= rows || j < 0 || j >= cols || k < 0 || k >= depth)
            throw std::out_of_range("Index out of range");
        return data[i][j][k];
    }

    // Const access method
    const T* const* at( int i, int j, int k ) const
    {
        if (i < 0 || i >= rows || j < 0 || j >= cols || k < 0 || k >= depth)
            throw std::out_of_range("Index out of range");
        return data[i][j][k];
    }
};