Bindings
=====
http://code.google.com/p/cgal-bindings/

Building the bindings
----

git clone https://code.google.com/p/cgal-bindings/  
cd cgal-bindings  
mkdir build  
cd build  
cmake -DCGAL_DIR=~/dev/cgal/CGAL-4.1/build/ -DBUILD_PYTHON=OFF -DJAVA_OUTDIR_PREFIX=../examples/java ..  
make

Swig
----
http://www.swig.org/tutorial.html
