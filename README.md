# Navarro's algorithm for approximate string matching

Implementation of Navarro's algorithm for approximate string matching [(paper)](https://www.sciencedirect.com/science/article/pii/S0304397599003333).

Student project for [bioinformatics course](https://www.fer.unizg.hr/en/course/enbio1) at FER, Zagreb.

## Usage

```sh
g++ -std=c++17 -O3 -o bioinf algo/algo.cpp
./bioinf -g data/set01/tmp/ref10000_linear.gfa -p data/set01/tmp/ref10000_simulatedreads.fastq
```

## Testing data

Testing data is generated using process described in [this repository](https://github.com/maickrau/GraphAligner/tree/PaperExperiments/WabiExperimentSnake).
