# Benchmark of various 4d vector hashing methods

Building:

    $ mkdir build
    $ cd build
    $ cmake ..
    $ ./hash_bench

Example output:
```
---------------------------------------------------------------------
Benchmark                           Time             CPU   Iterations
---------------------------------------------------------------------
bench_quat<triple_cantor>    73973646 ns     73947120 ns           10
bench_quat<cantor_fp>        73260913 ns     73247640 ns           10
bench_quat<fp_cantor>        72884957 ns     72876006 ns            9
bench_quat<fp_4d>            73547650 ns     73470258 ns           10
bench_quat<primes_8digits>   70647377 ns     70632122 ns            9
bench_quat<primes_4digits>   73325485 ns     73292646 ns            9

