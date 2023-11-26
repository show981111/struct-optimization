cd benchmarks/correctness/

rm -rf cachegrind.out.*
echo "=========== UNOPTIMIZED ==========="
valgrind --tool=cachegrind ./hw2correct${1}_no_fplicm
echo "=========== OPTIMIZED ==========="
valgrind --tool=cachegrind ./hw2correct${1}_fplicm
