# Navarro's algorithm for approximate string matching

Implementation of Navarro's algorithm for approximate string matching [(paper)](https://www.sciencedirect.com/science/article/pii/S0304397599003333).

Student project for [bioinformatics course](https://www.fer.unizg.hr/en/course/enbio1) at FER, Zagreb.

## Usage

Make sure you have g++ compiler installed on your computer before using this algorithm.

```sh
g++ -std=c++17 -O3 -o bioinf algo/algo.cpp
./bioinf -g data/set01/tmp/ref10000_linear.gfa -p data/set01/tmp/ref10000_simulatedreads.fastq
```

Possible parameter flags:
-g : location of a file with a graph in .gfa format is expected (ex. -g C:\Users\Fabijan\Documents\bioinf\data\examples\ex00\g.gfa)
-p : location of a file with patterns in .fastq format is expected (ex. -p C:\Users\Fabijan\Documents\bioinf\data\examples\ex00\p.fastq)

## Testing data

Testing data is generated using process described in [this repository](https://github.com/maickrau/GraphAligner/tree/PaperExperiments/WabiExperimentSnake).

## Output

For output you should recieve data about each node in the decomposed graph (original graph is chopped into nodes of segment size 1) in the format:
x_y Z num,
where x_y is the name of the node made up of

- x: the original node name that the current node was a part of
- y: the position of this node in the original node segment
- Z: one letter signifying data that the node holds from the original segment (y-th code in segment x)
- num: the distance of the nearest suffix of the graph ending in x_y from the pattern

Apart from that, the output contains information about time spent calculating.
