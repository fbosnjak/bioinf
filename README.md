How to use the Navarro algorithm code:

Make sure you have g++ compiler installed on your computer before using this algorithm.

Position yourself in the bioinf/algo/ folder.
Build the code in bioinf/algo/algo.cpp by writing:
g++ -o algo algo.cpp

It should build the exe file correctly which can be used by simply writing:
algo.exe -parameter1 value1 -parameter2 value2

Possible parameter flags:
-g : location of a file with a graph in .gfa format is expected (ex. -g C:\Users\Fabijan\Documents\bioinf\data\examples\ex00\g.gfa)
-p : location of a file with patterns in .fastq format is expected (ex. -p C:\Users\Fabijan\Documents\bioinf\data\examples\ex00\p.fastq)

Output:
For output you should recieve data about each node in the decomposed graph (original graph is chopped into nodes of segment size 1) in the format:
x_y Z num,
where x_y is the name of the node made up of

- x: the original node name that the current node was a part of
- y: the position of this node in the original node segment
- Z: one letter signifying data that the node holds from the original segment (y-th code in segment x)
- num: the distance of the nearest suffix of the graph ending in x_y from the pattern

Apart from that, the output contains information about time spent calculating.
