package lib;

abstract public class TachyonResult {
    protected long nativePtr;

    public TachyonResult(long nativePtr) {
        this.nativePtr = nativePtr;
    }

    public void delete() {
        nativeDeletePtr(nativePtr);
    }
    private static native void nativeDeletePtr(long nativePtr);
}
