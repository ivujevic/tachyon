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

| RANK | Tachyon      | Tachyon sensitive | Diamond      | Diamond sensitive | Diamond more sensitive | BLAST        |
| ---- | ------------ | ----------------- | ------------ | ----------------- | ---------------------- | ------------ |
| 1    | 1            | 1                 | 0.7079365079 | 0.7079365079      | 0.7079365079           | 0.7650793651 |
| 5    | 1            | 1                 | 0.8692063492 | 0.8704761905      | 0.8704761905           | 0.9079365079 |
| 10   | 1            | 1                 | 0.8936507937 | 0.8974603175      | 0.8974603175           | 0.940952381  |
| 25   | 0.9998730159 | 1                 | 0.9160634921 | 0.9208888889      | 0.9208888889           | 0.9624126984 |
| 50   | 0.9962539683 | 0.9994285714      | 0.925968254  | 0.9312380952      | 0.9312380952           | 0.9620952381 |
| 75   | 0.9881904762 | 0.9983915344      | 0.9326137566 | 0.9379047619      | 0.9379047619           | 0.9624973545 |
| 100  | 0.9812063492 | 0.9961904762      | 0.9412380952 | 0.9456825397      | 0.9456825397           | 0.9655873016 |
| 150  | 0.965037037  | 0.9912380952      | 0.9536931217 | 0.959026455       | 0.959026455            | 0.9665820106 |
| 200  | 0.944968254  | 0.9825873016      | 0.9604920635 | 0.9667777778      | 0.9667936508           | 0.964        |
| 300  | 0.8849100529 | 0.9484021164      | 0.9590793651 | 0.9673015873      | 0.9673015873           | 0.9568253968 |
| 400  | 0.8163174603 | 0.9055238095      | 0.9536825397 | 0.9624920635      | 0.962515873            | 0.9471587302 |
| 500  | 0.7514539683 | 0.8656952381      | 0.9515238095 | 0.9606095238      | 0.9606349206           | 0.9412634921 |

**HumVar**:

| RANK | Tachyon      | Tachyon sensitive | Diamond      | Diamond sensitive | Diamond more sensitive | BLAST        |
| ---- | ------------ | ----------------- | ------------ | ----------------- | ---------------------- | ------------ |
| 1    | 0.9973529412 | 0.9994117647      | 0.7955882353 | 0.7955882353      | 0.8922352941           | 0.8338235294 |
| 5    | 0.9968823529 | 0.9995294118      | 0.8911764706 | 0.8922352941      | 0.9111470588           | 0.9274117647 |
| 10   | 0.9958529412 | 0.9991176471      | 0.9099117647 | 0.9111470588      | 0.9332941176           | 0.9467647059 |
| 25   | 0.9907058824 | 0.9962            | 0.9314352941 | 0.9333058824      | 0.9301777778           | 0.9632352941 |
| 50   | 0.9699228758 | 0.984996732       | 0.9275830065 | 0.9301777778      | 0.9321503268           | 0.9580496732 |
| 75   | 0.9470169935 | 0.974324183       | 0.9290810458 | 0.9321464052      | 0.9415627702           | 0.9573947712 |
| 100  | 0.9278059075 | 0.9650385872      | 0.9382740322 | 0.9415451232      | 0.9570259534           | 0.9593820261 |
| 150  | 0.8924045099 | 0.9469803269      | 0.9527205695 | 0.95699262        | 0.9590008514           | 0.9602023223 |
| 200  | 0.8516530211 | 0.9218971394      | 0.9532992748 | 0.9589361456      | 0.9567599151           | 0.9547463125 |
| 300  | 0.765197055  | 0.8615499342      | 0.9473639165 | 0.9566177582      | 0.9519984403           | 0.9443399635 |
| 400  | 0.6866559729 | 0.8044401371      | 0.938887671  | 0.9517594248      | 0.9450937047           | 0.9352449433 |
| 500  | 0.6180714631 | 0.7509081591      | 0.9276575041 | 0.9447399833      |                        | 0.9258430541 |

**Escherichia**:

| RANK | Tachyon      | Tachyon sensitive | Diamond      | Diamond sensitive | Diamond more sensitive | BLAST        |
| ---- | ------------ | ----------------- | ------------ | ----------------- | ---------------------- | ------------ |
| 1    | 0.9987910538 | 0.9991940359      | 0.4039895225 | 0.4041910135      | 0.4041910135           | 0.5285109813 |
| 5    | 0.9944757875 | 0.9941298946      | 0.5162636846 | 0.5196554503      | 0.5196554503           | 0.5864329371 |
| 10   | 0.9893872594 | 0.9890404069      | 0.5818408605 | 0.5865835756      | 0.5866238738           | 0.6452481698 |
| 25   | 0.9810855425 | 0.9803589347      | 0.6781235847 | 0.6853113107      | 0.6853113107           | 0.7365178991 |
| 50   | 0.9718008032 | 0.9711081922      | 0.7542631954 | 0.7630656859      | 0.7630656859           | 0.8121857221 |
| 75   | 0.9644497998 | 0.9644030966      | 0.7967447644 | 0.8066911937      | 0.8066965668           | 0.8546667153 |
| 100  | 0.9581171164 | 0.958893716       | 0.824112706  | 0.8349589186      | 0.8349609335           | 0.8813351043 |
| 150  | 0.9477829907 | 0.9500852069      | 0.8575569646 | 0.869530642       | 0.8695373584           | 0.913377829  |
| 200  | 0.9391907559 | 0.9421679808      | 0.8773346807 | 0.8903917996      | 0.8904080822           | 0.9332710138 |
| 300  | 0.9230609875 | 0.9264132166      | 0.897366818  | 0.9125687798      | 0.9125776745           | 0.9533841169 |
| 400  | 0.9064412612 | 0.9099363988      | 0.9049565433 | 0.9220504984      | 0.9220563387           | 0.9626964314 |
| 500  | 0.8902861568 | 0.8938464355      | 0.9083629204 | 0.926930224       | 0.9269398926           | 0.9678985774 |

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
