# Tachyon

## Building

To build run the following commands from the cloned project:

```bash
    mkdir -p build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release ..
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
`./Tachyon blastp --db ${PATH_TO_FOLDER_OF_NR_DB}/nr.indexes ../db/queries.fa --out nr_output` 


## Acknowledgments

This work has been developed under supervision of prof. Mile Šikić (collab with Sebastian Maurer-Stroh from  Bioinformatics Institute, A*STAR). 
I would like to acknowledge the contribution of [Robert Vaser](https://github.com/rvaser/) from
the [Laboratory for Bioinformatics and Computational Biology](http://complex.zesoi.fer.hr/index.php/en/) 
for his help on this project. Both of them are also members of [AI in Genomics, Genome Institute of Singapore, A*STAR](https://www.a-star.edu.sg/gis)
