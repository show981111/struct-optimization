
cd build 
cmake ..
make 

cd .. 

cd ./benchmarks/correctness
echo "running benchmark$1"
sh run.sh benchmark$1

sh viz.sh benchmark$1 cfg
sh viz.sh benchmark$1.fplicm cfg 
