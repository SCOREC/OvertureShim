#include <string>

#include "Overture.h"  
#include "PlotStuff.h"
#include "display.h"
#include "ParallelUtility.h"
#include <stdlib.h>


int sendToHDF5(     std::string   filename,
                    int           *spacedim,
                    int           **interior_box,
                    int           **domain_box,
                    double        ***xy,
                    int           **mask );

int getFromHDF5(    aString       fileName, 
                    int           *numOfComponentGrids, 
                    int           *numberOfDimensions,
                    //int           *dim, 
                    int           ***interior_box, 
                    int           ***domain_box, 
                    double        ****xy, 
                    //int           **mask, 
                    int           ***desc );