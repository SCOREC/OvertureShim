
// Forward declaration
template <class T> class Array3D_V2;
template <class T> class Array4D;


int sendToTextFile(     const char*         	fileName,
                        int                 	numOfComponentGrids, 
                        int                 	dimension, 
                        Array3D_V2<int>        *grid_index_range, 
                        Array3D_V2<int>        *ext_index_range, 
                        Array3D_V2<int>        *bcs, 
                        Array4D<double>     	*xy, 
                        Array3D_V2<int>        *mask );

int getFromTextFile(    const char*         	fileName,
                        int                 	*numOfComponentGrids,  
                        int                 	*dimension, 
                        Array3D_V2<int>        *grid_index_range, 
                        Array3D_V2<int>        *ext_index_range,
                        Array3D_V2<int>        *bcs,  
                        Array4D<double>     	*xy, 
                        Array3D_V2<int>        *mask );