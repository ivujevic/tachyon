package lib;

public class Tachyon {

    private Long nativePtr;

    private Tachyon(Long nativePtr) {
        this.nativePtr = nativePtr;
    }

    public TachyonNucleotideResult searchX() {
        return new TachyonNucleotideResult(nativeSearchX());
    }

    public TachyonProteineResult searchP(String fasta, int numberOfThreads) {
        return new TachyonProteineResult(nativeSearchP(nativePtr, fasta, numberOfThreads));
    }

    public static void createIndexes(String inputPath, String outputPath, int numberOfThreads) {
        nativeCreateIndexes(inputPath, outputPath, numberOfThreads);
    }
    public static Tachyon initialize(String databasePath) {
        return new Tachyon(nativeInitialize(databasePath));
    }

    public void delete() {
        nativeDelete(this.nativePtr);
    }

    private static native void nativeCreateIndexes(String inputPath, String outputPath, int numberOfThreads);

    private static native long nativeInitialize(String databasePath);
    private static native long nativeSearchP(long ptr, String fasta, int numberOfThreads);
    private static native long nativeSearchX();
    private static native void nativeDelete(long nativePtr);

}
