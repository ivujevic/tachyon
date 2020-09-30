import lib.Tachyon;

public class CreateIndexesDemo {
    static {
        System.loadLibrary("TachyonNativeLib");
    }

    public static void main(String[] args) {

        String outputPath = "astral.bin";
        Tachyon.createIndexes(args[0], outputPath, 8);
    }
}
