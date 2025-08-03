#pragma once


#include <iostream>  // For debugging
#include <stdexcept>
#include <algorithm> // For std::sort and std::unique


template <typename T>
struct Array1D 
{
    T* data;
    int size;

    // Constructor
    Array1D() : data( nullptr ), size( 0 ) {}

    // Destructor
    ~Array1D() 
    {
        deallocate();
    }

    // Method to allocate the array and set size
    void allocate( int s ) 
    {
        if ( s <= 0 ) 
        {
            std::cerr << "Invalid size for allocation: " << s << std::endl;
            return;
        }
        size = s;
        data = new T[size];
        if ( data == nullptr ) 
        {
            std::cerr << "Failed to allocate memory for size: " << size << std::endl;
        } else 
        {
            std::cout << "Allocated Array1D of size " << size << std::endl; // Debugging
        }
    }


    // Method to deallocate the array
    void deallocate() 
    {
        if ( data ) 
        {
            delete[] data;
            data = nullptr;
            size = 0;
            std::cout << "Deallocated Array1D" << std::endl; // Debugging
        }
    }


    // Access operator
    T& operator[]( int index ) 
    {
        if ( index < 0 || index >= size ) 
        {
            throw std::out_of_range( "Index out of range" );
        }
        return data[index];
    }


    // Const access operator
    const T& operator[]( int index ) const 
    {
        if ( index < 0 || index >= size ) 
        {
            throw std::out_of_range( "Index out of range" );
        }
        return data[index];
    }


    // Method to extract unique elements
    Array1D<T>* extractUnique() const 
    {
        // Check if the original data is valid
        if ( data == nullptr || size <= 0 ) 
        {
            std::cerr << "Invalid data or size in extractUnique" << std::endl;
            return nullptr;
        }

        // Create a copy of the data
        Array1D<T>                  tempArray;
        tempArray.allocate( size );

        // Debugging statements
        std::cout << "Original data size: " << size << std::endl;
        std::cout << "Temp array allocated with size: " << tempArray.size << std::endl;

        // Ensure tempArray.data is not null
        if ( tempArray.data == nullptr ) 
        {
            std::cerr << "Failed to allocate tempArray.data" << std::endl;
            return nullptr;
        }

        // Copy data to tempArray
        std::copy(  data, 
                    data + size, 
                    tempArray.data );

        // Sort the copy
        std::sort(  tempArray.data, 
                    tempArray.data + size );

        // Remove duplicates
        T*                  uniqueEnd               = std::unique(  tempArray.data, 
                                                                    tempArray.data + size );
        int                 uniqueSize              = uniqueEnd - tempArray.data;

        // Create the unique array on the heap
        Array1D<T>*         uniqueArray             = new Array1D<T>(  );
        uniqueArray -> allocate( uniqueSize );

        // Ensure uniqueArray.data is not null
        if ( uniqueArray -> data == nullptr ) 
        {
            std::cerr << "Failed to allocate uniqueArray.data" << std::endl;
            tempArray.deallocate();
            delete      uniqueArray;
            return nullptr;
        }

        // Copy unique data to uniqueArray
        std::copy(  tempArray.data, 
                    uniqueEnd, 
                    uniqueArray -> data );

        // Deallocate the temporary array
        tempArray.deallocate();

        return uniqueArray;
    }
};
