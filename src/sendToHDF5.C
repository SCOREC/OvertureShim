#include <string>
#ifdef OLD_HEADER_FILENAME
#include <iostream.h>
#else
#include <iostream>
#endif

#include "H5Cpp.h"
#include "sendToHDF5.h"

#ifndef H5_NO_NAMESPACE
using namespace H5;
#endif


int sendToHDF5(   std::string   filename,
                  int           spacedim,
                  int           *interior_box,
                  int           *domain_box,
                  double        *xy,
                  int           *mask )
{
  try
  {

    H5File* file = new H5File( filename, H5F_ACC_TRUNC );
    Group group( file->createGroup("/grid1") );

    // Create dataspace for dataset in file.
    DataSet *dataset;

    // Set interior box ///////////////////////////////////////////////////////
    hsize_t     boxsize[] = {2, 2};
    DataSpace   boxspace( 2, boxsize );

    dataset = new DataSet( 
                            group.createDataSet(  "interior_box", 
                                                  PredType::NATIVE_INT, 
                                                  boxspace ) 
                         );
    dataset ->      write(  interior_box, 
                            PredType::NATIVE_INT );

    delete dataset;


    // Set dimensions ///////////////////////////////////////////////////////
    hsize_t     dimsize[] = {1};
    DataSpace   fspace( 1, dimsize );

    dataset = new DataSet( 
                            group.createDataSet(  "dim", 
                                                  PredType::NATIVE_INT, 
                                                  fspace ) 
                         );
    dataset ->      write(  &spacedim, 
                            PredType::NATIVE_INT );

    delete dataset;


    // Set domain box ///////////////////////////////////////////////////////
    dataset = new DataSet( 
                            group.createDataSet(  "domain_box", 
                                                  PredType::NATIVE_INT, 
                                                  boxspace ) 
                         );
    dataset ->      write(  domain_box, 
                            PredType::NATIVE_INT );

    delete dataset;


    // Set xy ///////////////////////////////////////////////////////
    int nx = *(domain_box + 2*1 + 0) - *(domain_box + 2*0 + 0);
    int ny = *(domain_box + 2*1 + 1) - *(domain_box + 2*0 + 1);
    
    hsize_t   gridsize[] = {  static_cast<hsize_t>(nx + 1), 
                              static_cast<hsize_t>(ny + 1), 
                              2 };
    DataSpace xyspace(  spacedim + 1, 
                        gridsize );
    
    dataset = new DataSet( 
                            group.createDataSet(  "xy", 
                                                  PredType::NATIVE_DOUBLE, 
                                                  xyspace ) 
                         );
    dataset ->      write(  xy, 
                            PredType::NATIVE_DOUBLE );

    delete dataset;


    // Set mask ///////////////////////////////////////////////////////
    DataSpace   maskspace(2, gridsize);

    dataset = new DataSet( 
                            group.createDataSet(  "mask", 
                                                  PredType::NATIVE_INT, 
                                                  maskspace ) 
                         );
    dataset ->      write(  mask, 
                            PredType::NATIVE_INT );

    delete dataset;


    file->close();
  } 

   // catch failure caused by the DataSet operations
  catch( DataSetIException error )
  {
    std::cout << error.getDetailMsg() << std::endl;
    std::cout << "dataset exception" << std::endl;
    error.printErrorStack();
    return -1;
  }

  // catch failure caused by the DataSpace operations
  catch( DataSpaceIException error )
  {
    std::cout << error.getDetailMsg() << std::endl;
    std::cout << "dataspace exception" << std::endl;
    error.printErrorStack();
    return -1;
  }

  return 0;
}



int getFromHDF5(  std::string   filename,
                  int           spacedim,
                  int           *interior_box,
                  int           *domain_box,
                  double        *xy,
                  int           *mask )
{
  
}