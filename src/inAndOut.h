
#include "Array3D.h"
#include "Array4D.h"


int sendToTextFile(     const char*         fileName,
                        int                 numOfComponentGrids, 
                        int                 dimension, 
                        Array3D<int>        *interior_box, 
                        Array3D<int>        *domain_box, 
                        Array4D<double>     *xy, 
                        Array3D<int>        *mask );

int getFromTextFile(    const char*         fileName,
                        int                 numOfComponentGrids,  
                        int                 dimension, 
                        Array3D<int>        *interior_box, 
                        Array3D<int>        *domain_box, 
                        Array4D<double>     *xy, 
                        Array3D<int>        *mask );