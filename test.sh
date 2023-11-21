
cd build 
cmake ..
make 

cd .. 

cd ./benchmarks/correctness
echo "running hw2correct$1"
sh run.sh hw2correct$1

sh viz.sh hw2correct$1 cfg
sh viz.sh hw2correct$1.fplicm cfg 
