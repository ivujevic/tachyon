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

For this example we will use Astral database (https://scop.berkeley.edu/downloads/scopeseq-2.07/astral-scopedom-seqres-gd-sel-gs-bib-40-2.07.fa).
You can find it into `db` folder.

The first step is to create a indexes database. From the `build` folder run:

`./Tachyon makedb --in ../db/astral.fa --db ./indexes.bin`

After that searching can be performed:

`./Tachyon blastp --db indexes.bin --query ../db/queries.fa --out output`



