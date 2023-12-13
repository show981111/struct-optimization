# Struct Data layout optimization

## Running a benchamrk

Create Visual diagram of IR and run benchmark.

```sh
sh test.sh 3 # number of benchmark. Currently have 10 benchmarks 
```

## Get Cache statistics

Cache statistics are gathered using **Valgrind.** In order to get the result correctly, install the Valgrind first.

### Get the result of benchmarks

After running `test.sh` script, run `compare.sh` script.

```sh
sh compare.sh 3 # number of benchmark. Currently have 10 benchmarks 
```

The reuslt will be stored in `/benchmarks/results` directory.
