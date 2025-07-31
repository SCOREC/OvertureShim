#include <string>

#include "Overture.h"  
#include "PlotStuff.h"
#include "display.h"
#include "ParallelUtility.h"

#include "Array4D.h"

#include <stdlib.h>


// Forward declaration
class HydeCompositeGrid;


int sendToHDF5(     std::string     		filename,
                    HydeCompositeGrid 		*hydeCompositeGrid,
                    Array4D<double> 		*xy );

int getFromHDF5(    const char     			*fileName, 
                    HydeCompositeGrid 		*hydeCompositeGrid,
                    Array4D<double> 		*xy );