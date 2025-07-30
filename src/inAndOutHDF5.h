#include <string>

#include "Overture.h"  
#include "PlotStuff.h"
#include "display.h"
#include "ParallelUtility.h"

#include "Array4D.h"
#include "Array3D.h"

#include <stdlib.h>


int sendToHDF5(     std::string     filename,
                    int             numOfComponentGrids, 
                    int             numberOfDimensions,
                    Array3D<int>    *grid_index_range, 
                    Array3D<int>    *ext_index_range,
                  	Array3D<int>    *bcs, 
                    Array4D<double> *xy, 
                    Array3D<int>    *desc );

int getFromHDF5(    const char*     filename, 
                    int             *numOfComponentGrids, 
                    int             *numberOfDimensions,
                    Array3D<int>    *grid_index_range, 
                    Array3D<int>    *ext_index_range,
                  	Array3D<int>    *bcs, 
                    Array4D<double> *xy, 
                    Array3D<int>    *desc );