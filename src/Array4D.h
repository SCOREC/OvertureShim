#pragma once

#include <iostream>
#include <stdexcept>

template <typename T>
struct Array4D
{
    T**** data;
    int rows;
    int cols;

	int dim;

    // Constructor
    Array4D() : data(nullptr), rows(0), cols(0), dim(0) {}

    // Destructor
    ~Array4D()
    {
        deallocate();
    }

    // Allocate the array: [rows][cols][dim][dim]
    void allocate(int r, int c, int d)
    {
        rows 	= r;
        cols 	= c;
        dim 	= d;
        data 	= new T***[rows];
        for (int i = 0; i < rows; ++i)
        {
            data[i] = new T**[cols];
            for (int j = 0; j < cols; ++j)
            {
                data[i][j] = new T*[dim];
				
                for (int k = 0; k < dim; ++k)
                {
                    data[i][j][k] = new T[dim];
                }
            }
        }
    }

    // Fill all elements with a value
    void fill(const T& value)
    {
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                for (int k = 0; k < dim; ++k)
                    for (int l = 0; l < dim; ++l)
                        data[i][j][k][l] = value;
    }

    // Set identity matrix at every grid point
    void setIdentity()
    {
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                for (int k = 0; k < dim; ++k)
                    for (int l = 0; l < dim; ++l)
                        data[i][j][k][l] = (k == l) ? 1 : 0;
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
                    for (int k = 0; k < dim; ++k)
                    {
                        delete[] data[i][j][k];
                    }
                    delete[] data[i][j];
                }
                delete[] data[i];
            }
            delete[] data;
            data = nullptr;
            rows = cols = dim = 0;
            std::cout << "Deallocated Array4D" << std::endl;
        }
    }

    // Access operator: returns pointer to [M][dim][dim] block for row i
    T*** operator[](int index)
    {
        if (index < 0 || index >= rows)
            throw std::out_of_range("Index out of range");
        return data[index];
    }

    // Const access operator
    const T*** operator[](int index) const
    {
        if (index < 0 || index >= rows)
            throw std::out_of_range("Index out of range");
        return data[index];
    }
};