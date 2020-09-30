package lib;

public class FastaFileElem {

    private final long nativePtr;

    private Integer id = null;
    private String name = null;
    private String sequence = null;

    public FastaFileElem(long nativePtr) {
        this.nativePtr = nativePtr;
    }

    public int getId() {
        if (id != null) return id;
        return nativeGetId(nativePtr);
    }

    public String getName() {
        if (name != null) return name;
        return nativeGetName(nativePtr);
    }

    public String getSequence() {
        if (sequence != null) return sequence;
        return nativeGetSequence(nativePtr);
    }

    private static native int nativeGetId(long nativePtr);
    private static native String nativeGetName(long nativePtr);
    private static native String nativeGetSequence(long nativePtr);
}
