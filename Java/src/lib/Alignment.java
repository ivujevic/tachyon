package lib;

public class Alignment {

    private long nativePtr;

    private Integer score = null;
    private FastaFileElem target = null;
    private Integer qBegin = null;
    private Integer qEnd = null;
    private Integer tBegin = null;
    private Integer tEnd = null;
    private Integer alignLen = null;
    private char[] alignment = null;
    private Double eValue = null;

    public Alignment(long nativePtr) {
        this.nativePtr = nativePtr;
    }

    public int getScore() {
        if (score != null) return score;
        return nativeGetScore(nativePtr);
    }
    public FastaFileElem getTarget() {
        if (target != null) return target;
        return new FastaFileElem(nativeGetTarget(nativePtr));
    }
    public int getQBegin() {
        if (qBegin != null) return qBegin;
        return nativeGetQBegin(nativePtr);
    }
    public int getQEnd() {
        if (qEnd != null) return qEnd;
        return nativeGetQEnd(nativePtr);
    }
    public int getTBegin() {
        if (tBegin != null) return tBegin;
        return nativeGetTBegin(nativePtr);
    }
    public int getTEnd() {
        if (tEnd != null) return tEnd;
        return nativeGetTEnd(nativePtr);
    }

    public int getAlignLen() {
        if (alignLen != null) return alignLen;
        return nativeGetAlignLen(nativePtr);
    }

    public char[] getAlignment() {
        if (alignment != null) return alignment;
        return nativeGetAlignment(nativePtr);
    }

    public double getEValue() {
        if (eValue != null) return eValue;
        return nativeGetEValue(nativePtr);
    }
    private static native int nativeGetScore(long nativePtr);
    private static native long nativeGetTarget(long nativePtr);
    private static native int nativeGetQBegin(long nativePtr);
    private static native int nativeGetQEnd(long nativePtr);
    private static native int nativeGetTBegin(long nativePtr);
    private static native int nativeGetTEnd(long nativePtr);
    private static native int nativeGetAlignLen(long nativePtr);
    private static native char[] nativeGetAlignment(long nativePtr);
    private static native double nativeGetEValue(long nativePtr);
}
