
cd build 
cmake ..
make 

cd .. 

cd ./benchmarks/performance
echo "running hw2perf$1"
sh run.sh hw2perf$1

sh viz.sh hw2perf$1 cfg
sh viz.sh hw2perf$1.fplicm cfg 