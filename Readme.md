# Tachyon

1. [Results](#results)
    - [Sensitivity](#sensitivity)
        - [BLASTP](#BLASTP)
        - [BLASTX](#BLASTX)
    - [Speed](#Speed)
2. [Building](#Building)
3. [Examples](#Examples)
  
## Results
We have compared Tachyon's performance to [DIAMOND](http://www.diamondsearch.org/index.php) 
and [BLAST](https://blast.ncbi.nlm.nih.gov/Blast.cgi). As a reference we used NCBI NR (123GB),
and have used three query sets (HumDiv, HumVar, Escherichia). 

### Sensitivity

#### BLASTP
To measure sensitivity we used SW# as reference. The following tables shows sensitivity:

**HumDiv**: 

| RANK | Tachyon | Tachyon sensitive | Diamond | Diamond sensitive | Diamond more sensitive | BLAST  |
| ---- | ------- | ----------------- | ------- | ----------------- | ---------------------- | ------ |
| 1    | 100.00% | 100.00%           | 70.79%  | 70.79%            | 70.79%                 | 76.51% |
| 5    | 100.00% | 100.00%           | 86.92%  | 87.05%            | 87.05%                 | 90.79% |
| 10   | 100.00% | 100.00%           | 89.37%  | 89.75%            | 89.75%                 | 94.10% |
| 25   | 99.99%  | 100.00%           | 91.61%  | 92.09%            | 92.09%                 | 96.24% |
| 50   | 99.63%  | 99.94%            | 92.60%  | 93.12%            | 93.12%                 | 96.21% |
| 75   | 98.82%  | 99.84%            | 93.26%  | 93.79%            | 93.79%                 | 96.25% |
| 100  | 98.12%  | 99.62%            | 94.12%  | 94.57%            | 94.57%                 | 96.56% |
| 150  | 96.50%  | 99.12%            | 95.37%  | 95.90%            | 95.90%                 | 96.66% |
| 200  | 94.50%  | 98.26%            | 96.05%  | 96.68%            | 96.68%                 | 96.40% |
| 300  | 88.49%  | 94.84%            | 95.91%  | 96.73%            | 96.73%                 | 95.68% |
| 400  | 81.63%  | 90.55%            | 95.37%  | 96.25%            | 96.25%                 | 94.72% |
| 500  | 75.15%  | 86.57%            | 95.15%  | 96.06%            | 96.06%                 | 94.13% |

**HumVar**:

| RANK | Tachyon | Tachyon sensitive | Diamond | Diamond sensitive | Diamond more sensitive | BLAST  |
| ---- | ------- | ----------------- | ------- | ----------------- | ---------------------- | ------ |
| 1    | 99.74%  | 99.94%            | 79.56%  | 79.56%            | 89.22%                 | 83.38% |
| 5    | 99.69%  | 99.95%            | 89.12%  | 89.22%            | 91.11%                 | 92.74% |
| 10   | 99.59%  | 99.91%            | 90.99%  | 91.11%            | 93.33%                 | 94.68% |
| 25   | 99.07%  | 99.62%            | 93.14%  | 93.33%            | 93.02%                 | 96.32% |
| 50   | 96.99%  | 98.50%            | 92.76%  | 93.02%            | 93.22%                 | 95.80% |
| 75   | 94.70%  | 97.43%            | 92.91%  | 93.21%            | 94.16%                 | 95.74% |
| 100  | 92.78%  | 96.50%            | 93.83%  | 94.15%            | 95.70%                 | 95.94% |
| 150  | 89.24%  | 94.70%            | 95.27%  | 95.70%            | 95.90%                 | 96.02% |
| 200  | 85.17%  | 92.19%            | 95.33%  | 95.89%            | 95.68%                 | 95.47% |
| 300  | 76.52%  | 86.15%            | 94.74%  | 95.66%            | 95.20%                 | 94.43% |
| 400  | 68.67%  | 80.44%            | 93.89%  | 95.18%            | 94.51%                 | 93.52% |
| 500  | 61.81%  | 75.09%            | 92.77%  | 94.47%            |                        | 92.58% |


**Escherichia**:

| RANK | Tachyon | Tachyon sensitive | Diamond | Diamond sensitive | Diamond more sensitive | BLAST  |
| ---- | ------- | ----------------- | ------- | ----------------- | ---------------------- | ------ |
| 1    | 99.88%  | 99.92%            | 40.40%  | 40.42%            | 40.42%                 | 52.85% |
| 5    | 99.45%  | 99.41%            | 51.63%  | 51.97%            | 51.97%                 | 58.64% |
| 10   | 98.94%  | 98.90%            | 58.18%  | 58.66%            | 58.66%                 | 64.52% |
| 25   | 98.11%  | 98.04%            | 67.81%  | 68.53%            | 68.53%                 | 73.65% |
| 50   | 97.18%  | 97.11%            | 75.43%  | 76.31%            | 76.31%                 | 81.22% |
| 75   | 96.44%  | 96.44%            | 79.67%  | 80.67%            | 80.67%                 | 85.47% |
| 100  | 95.81%  | 95.89%            | 82.41%  | 83.50%            | 83.50%                 | 88.13% |
| 150  | 94.78%  | 95.01%            | 85.76%  | 86.95%            | 86.95%                 | 91.34% |
| 200  | 93.92%  | 94.22%            | 87.73%  | 89.04%            | 89.04%                 | 93.33% |
| 300  | 92.31%  | 92.64%            | 89.74%  | 91.26%            | 91.26%                 | 95.34% |
| 400  | 90.64%  | 90.99%            | 90.50%  | 92.21%            | 92.21%                 | 96.27% |
| 500  | 89.03%  | 89.38%            | 90.84%  | 92.69%            | 92.69%                 | 96.79% |


#### BLASTX

To measure sensitivity of BLASTX mode we used 5K reads of Human genome and align it against NR database. For each
read we measure how many times this read is mapped to Human reference in first N (1, 5, 10) results.
From the results we can see that Tachyon in sensitive mode are better than Diamond for N < 5.

|N | Tachyon | Tachyon Sensitive | Diamond | BLAST |
|---| ------- | ---------------   | ------- | ----- |
|1  | 615     |           769     |  731    | 937   |
|5  | 1915    |           3076    |  3083   | 3719   |
|10 | 2945    |           5156    |  5740   | 6649   |


### Speed

#### BLASTP mode
To measure speed we have used same query sets as for sensitivity. Those tables show that tachyon is 
faster than Diamond, and Tachyon in sensitivity mode is also faster than Diamond in sensitivity mode.

|               | Tachyon      | Tachyon sensitive | Diamond      | Diamond sensitive | Diamond more sensitive |
| ----          | ------------ | ----------------- | ------------ | ----------------- | ---------------------- |
| HumDiv        | 197s         | 755.347s          | 629.359s     | 2596.23s          | 2578.82s               |
| HumVar        | 738.115s     | 1708.885s         | 830.195s     | 3530.22s          | 3588.32s               |
| Esherichia    | 435.262s     | 1179.694s         | 834.69s      | 3651.16s          | 3617.92s               |

## Building

To build run the following commands from the cloned project:

```bash
    mkdir -p build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release ..ma
    make
```

After running those commands, the executable `Tachyon` will
be into `build` directory.

### JNI BUILD

If you want to use this library with some Java app run `cmake` as :

`cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_NATIVE=ON ..` 

before `make`.


## Examples

### Astral Database
For this example we will use Astral database (https://scop.berkeley.edu/downloads/scopeseq-2.07/astral-scopedom-seqres-gd-sel-gs-bib-40-2.07.fa).
You can find it into `db` folder.

The first step is to create a indexes database. From the `build` folder run:

`./Tachyon makedb --in ../db/astral.fa --db ./indexes.bin`

After that searching can be performed:

`./Tachyon blastp --db indexes.bin --query ../db/queries.fa --out output`

### NR Database
If you want to use the NR database, first download it:
`curl 'https://ftp.ncbi.nlm.nih.gov/blast/db/FASTA/nr.gz' --output nr.gz`

Once you get `nr.gz` file extract it using `gzip -d nr.gz` command.

After that make indexes:

`./Tachyon makedb --in ${PATH_TO_FOLDER_OF_NR_DB}/nr --db ${PATH_TO_FOLDER_OF_NR_DB}/nr.indexes`

and search using the same query file as it was for Astral database:
`./Tachyon blastp --db ${PATH_TO_FOLDER_OF_NR_DB}/nr.indexes --query ../db/queries.fa --out nr_output` 

### BLASTX mode
Tachyon also suports mapping RNA reads against protein database.

`./Tachyon blastx --db ${PATH_TO_FOLDER_OF_NR_DB}/nr.indexes --query ../db/human_genome.fa --out blastx_output`
## Acknowledgments

This work has been developed under supervision of prof. Mile Šikić (collab with Sebastian Maurer-Stroh from  Bioinformatics Institute, A*STAR). 
I would like to acknowledge the contribution of [Robert Vaser](https://github.com/rvaser/) from
the [Laboratory for Bioinformatics and Computational Biology](http://complex.zesoi.fer.hr/index.php/en/) 
for his help on this project. Both of them are also members of [AI in Genomics, Genome Institute of Singapore, A*STAR](https://www.a-star.edu.sg/gis)
