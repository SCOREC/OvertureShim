#include <string>

#include "Overture.h"  
#include "PlotStuff.h"
#include "display.h"
#include "ParallelUtility.h"


#include <stdlib.h>


// Forward declaration
class HydeCompositeGrid;


int sendToHDF5(     std::string     		filename,
                    HydeCompositeGrid 		*hydeCompositeGrid );

int getFromHDF5(    const char     			*fileName, 
                    HydeCompositeGrid 		*hydeCompositeGrid );