cd benchmarks/correctness/

rm -rf cachegrind.out.*
echo "=========== UNOPTIMIZED (Original) ===========\n\n" > ../results/benchmark${1}.txt
valgrind --tool=cachegrind ./hw2correct${1}_no_fplicm 2>> ../results/benchmark${1}.txt
echo "\n\n=========== UNOPTIMIZED (Profile) ===========\n\n" >> ../results/benchmark${1}.txt
valgrind --tool=cachegrind ./hw2correct${1}_prof 2>> ../results/benchmark${1}.txt
echo "\n\n=========== OPTIMIZED ===========\n\n" >> ../results/benchmark${1}.txt
valgrind --tool=cachegrind ./hw2correct${1}_fplicm 2>> ../results/benchmark${1}.txt
rm -rf cachegrind.out.*
