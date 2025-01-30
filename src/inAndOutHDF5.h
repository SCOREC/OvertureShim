#include <string>


int sendToHDF5(     std::string   filename,
                    int           *spacedim,
                    int           **interior_box,
                    int           **domain_box,
                    double        ***xy,
                    int           **mask );

int getFromHDF5(    const char*   fileName, 
                    int           *numOfComponentGrids, 
                    //int           *dim, 
                    int           **interior_box, 
                    int           **domain_box, 
                    double        ***xy, 
                    //int           **mask, 
                    int           **desc );