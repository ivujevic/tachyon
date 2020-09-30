package lib;

public class TachyonNucleotideResultEntry {
    private long nativePtr;

    private FastaFileElem query = null;
    private TachyonResultEntry[] entries = null;

    public TachyonNucleotideResultEntry(long nativePtr) {
        this.nativePtr = nativePtr;
    }

    public FastaFileElem getQuery() {
        if (query != null) return query;
        return new FastaFileElem(nativeGetQuery(this.nativePtr));
    }

    public TachyonResultEntry[] getEntries() {
        if (entries != null) return entries;

        entries = new TachyonResultEntry[6];
        int i = 0;
        for (Long entry: nativeGetEntries(this.nativePtr)) {
            entries[i] = new TachyonResultEntry(entry);
        }

        return entries;
    }

    private static native long nativeGetQuery(long nativePtr);
    private static native long[] nativeGetEntries(long nativePtr);

}
