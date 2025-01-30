

int sendToTextFile(     const char*   fileName, 
                        int           dim, 
                        int           **interior_box, 
                        int           **domain_box, 
                        double        ***xy, 
                        int           **mask );

int getFromTextFile(    const char*   fileName, 
                        int           *dim, 
                        int           **interior_box, 
                        int           **domain_box, 
                        double        ***xy, 
                        int           **mask );