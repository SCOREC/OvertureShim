export MOTIF=/usr/motif

###############################################
## Required vars for overture build ##########
export APlusPlus=$HOME/dependencies/A++/install
export PPlusPlus=$HOME/dependencies/P++/install

## OpenGL set to Mesa 7.0.2 for ogen to function
#export OpenGL=/usr/opengl
export OpenGL=$HOME/Mesa-7.0.2

#export HDF=/usr/hdf5
export HDF=$HOME/hdf5-1.8.15-macPatch

export Overture=$HOME/overture

export XLIBS=/lib/x86_64-linux-gnu
#export XLIBS=/usr

export LAPACK=/lib/x86_64-linux-gnu
#export LAPACK=/usr
###############################################
###############################################


#########################################
## Relevant build locations #############
export OvertureBuild=$HOME/overture
export OgenBuild=${Overture}-working-ogen
export shimBuild=$HOME/overture-hyde-shim/src
#########################################
#########################################


#export GL_LIBS="-lGLU -lGL -lOSMesa"
export GL_LIBS="-lGLU -lGL -lGLw -lOpenGL"

export LD_LIBRARY_PATH=$OpenGL/lib64:$MOTIF/lib:/usr/lib/x86_64-linux-gnu:$LD_LIBRARY_PATH
export INCLUDE_DIRS=$OpenGL/include:$MOTIF/include:$INCLUDE_DIRS

#export LD_LIBRARY_PATH=$MOTIF/lib:/usr/lib/x86_64-linux-gnu:$LD_LIBRARY_PATH
#export INCLUDE_DIRS=$MOTIF/include:$INCLUDE_DIRS

export PATH=$PATH:.

#################################################################
## Some useful alias commands ###################################
# alias ogen="/home/overture/run-ogen.sh"
# alias gridShimIn="/home/overture/run-shim.sh"
#################################################################
#################################################################
