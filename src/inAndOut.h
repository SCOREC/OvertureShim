
#include "Array3D.h"
#include "Array4D.h"


int sendToTextFile(     const char*         fileName,
                        int                 numOfComponentGrids, 
                        int                 dimension, 
                        Array3D<int>        *grid_index_range, 
                        Array3D<int>        *ext_index_range, 
                        Array3D<int>        *bcs, 
                        Array4D<double>     *xy, 
                        Array3D<int>        *mask );

int getFromTextFile(    const char*         fileName,
                        int                 *numOfComponentGrids,  
                        int                 *dimension, 
                        Array3D<int>        *grid_index_range, 
                        Array3D<int>        *ext_index_range,
                        Array3D<int>        *bcs,  
                        Array4D<double>     *xy, 
                        Array3D<int>        *mask );