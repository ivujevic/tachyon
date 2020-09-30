package lib;

import java.util.ArrayList;
import java.util.Vector;

public class TachyonResultEntry {

    private long nativePtr;

    private FastaFileElem query = null;
    private ArrayList<Alignment> alignments = null;

    public TachyonResultEntry(long nativePtr) {
        this.nativePtr = nativePtr;
    }

    public FastaFileElem getQuery() {
        if (query != null) return query;
        return new FastaFileElem(nativeGetQuery(nativePtr));
    }

    public ArrayList<Alignment> getAlignments() {
        if (alignments != null) return alignments;

        alignments = new ArrayList<>();
        for (Long a : nativeGetAlignments(nativePtr)) {
            alignments.add(new Alignment(a));
        }
        return alignments;
    }

    private static native long nativeGetQuery(long nativePtr);
    private static native long[] nativeGetAlignments(long nativePtr);
}
