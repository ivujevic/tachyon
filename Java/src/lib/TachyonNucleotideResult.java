package lib;

import java.util.ArrayList;

public class TachyonNucleotideResult extends TachyonResult {

    private ArrayList<TachyonNucleotideResultEntry> entries = null;

    public TachyonNucleotideResult(long nativePtr) {
        super(nativePtr);
    }

    public ArrayList<TachyonNucleotideResultEntry> getEntries() {
        if (entries != null) return entries;

        entries = new ArrayList<>();

        for(Long entry: nativeGetEntries(this.nativePtr)) {
            entries.add(new TachyonNucleotideResultEntry(entry));
        }
        return entries;
    }

    private static native long[] nativeGetEntries(long nativePtr);
}
