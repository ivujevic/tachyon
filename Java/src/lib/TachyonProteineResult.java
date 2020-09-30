package lib;

import java.util.ArrayList;

public class TachyonProteineResult extends TachyonResult {

    private ArrayList<TachyonResultEntry> entries = null;

    public TachyonProteineResult(long nativePtr) {
        super(nativePtr);
    }

    public ArrayList<TachyonResultEntry> getEntries() {
        if (entries != null) return entries;

        entries = new ArrayList<>();

        for (Long entry : nativeGetEntries(nativePtr)) {
            entries.add(new TachyonResultEntry(entry));
        }

        return entries;
    }

    private static native long[] nativeGetEntries(long nativePtr);
}
