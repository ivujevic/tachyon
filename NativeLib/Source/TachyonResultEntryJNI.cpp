#include "TachyonResultJNI.hpp"

#include "jnipp.h"

#define JNI_PREFIX lib_TachyonResultEntry

#include "jnimacros.h"


JNIEXPORT jlong JNICALL JNI_METHOD(nativeGetQuery(JNIEnv* env, jclass, jlong nativePtr)) {
    jnipp::Env::Scope scope(env);

    auto entry(reinterpret_cast<tachyon::TachyonResultEntry*>(nativePtr));

    return reinterpret_cast<jlong>(entry->query_.get());
}


JNIEXPORT jlongArray JNICALL JNI_METHOD(nativeGetAlignments(JNIEnv* env, jclass, jlong nativePtr)) {
    jnipp::Env::Scope scope(env);

    auto result(reinterpret_cast<tachyon::TachyonResultEntry*>(nativePtr));


    jnipp::Array<jlong> retArray(jnipp::Array<jlong>::create(result->alignments_.size()).steal());

    for (int i = 0; i < (int) result->alignments_.size(); ++i) {
        retArray.set(i, reinterpret_cast<jlong>(&result->alignments_[i]));
    }

    return retArray.operator _jlongArray *();
}


#undef JNI_PREFIX

#define JNI_PREFIX lib_Alignment

JNIEXPORT jint JNICALL JNI_METHOD(nativeGetScore(JNIEnv* env, jclass, jlong nativePtr)) {
    jnipp::Env::Scope scope(env);

    auto alignment(reinterpret_cast<tachyon::Alignment*>(nativePtr));

    return reinterpret_cast<jint>(alignment->score_);
}

JNIEXPORT jint JNICALL JNI_METHOD(nativeGetQBegin(JNIEnv* env, jclass, jlong nativePtr)) {
    jnipp::Env::Scope scope(env);

    auto alignment(reinterpret_cast<tachyon::Alignment*>(nativePtr));

    return reinterpret_cast<jint>((int) alignment->qBegin_);
}

JNIEXPORT jint JNICALL JNI_METHOD(nativeGetQEnd(JNIEnv* env, jclass, jlong nativePtr)) {
    jnipp::Env::Scope scope(env);

    auto alignment(reinterpret_cast<tachyon::Alignment*>(nativePtr));

    return reinterpret_cast<jint>((int) alignment->qEnd_);
}

JNIEXPORT jint JNICALL JNI_METHOD(nativeGetTBegin(JNIEnv* env, jclass, jlong nativePtr)) {
    jnipp::Env::Scope scope(env);

    auto alignment(reinterpret_cast<tachyon::Alignment*>(nativePtr));

    return reinterpret_cast<jint>((int) alignment->tBegin_);
}

JNIEXPORT jint JNICALL JNI_METHOD(nativeGetTEnd(JNIEnv* env, jclass, jlong nativePtr)) {
    jnipp::Env::Scope scope(env);

    auto alignment(reinterpret_cast<tachyon::Alignment*>(nativePtr));

    return reinterpret_cast<jint>((int) alignment->tEnd_);
}

JNIEXPORT jint JNICALL JNI_METHOD(nativeGetAlignLen(JNIEnv* env, jclass, jlong nativePtr)) {
    jnipp::Env::Scope scope(env);

    auto alignment(reinterpret_cast<tachyon::Alignment*>(nativePtr));

    return reinterpret_cast<jint>((int) alignment->alignLen_);
}

JNIEXPORT jdouble JNICALL JNI_METHOD(nativeGetEValue(JNIEnv* env, jclass, jlong nativePtr)) {
    jnipp::Env::Scope scope(env);

    auto alignment(reinterpret_cast<tachyon::Alignment*>(nativePtr));
    return (jdouble) alignment->eValue_;
}

JNIEXPORT jcharArray JNICALL JNI_METHOD(nativeGetAlignment(JNIEnv* env, jclass, jlong nativePtr)) {
    jnipp::Env::Scope scope(env);

    auto alignment(reinterpret_cast<tachyon::Alignment*>(nativePtr));


    jnipp::Array<jchar> retArray(jnipp::Array<jchar>::create(alignment->alignment_.size()).steal());
    for (int i = 0; i < (int) alignment->alignLen_; ++i) {
        retArray.set(i, (jchar) alignment->alignment_[i]);
    }

    return retArray.operator _jcharArray *();
}

JNIEXPORT jlong JNICALL JNI_METHOD(nativeGetTarget(JNIEnv* env, jclass, jlong nativePtr)) {
    jnipp::Env::Scope scope(env);

    auto alignment(reinterpret_cast<tachyon::Alignment*>(nativePtr));

    return reinterpret_cast<jlong>(alignment->target.get());
}

#undef JNI_PREFIX

#define JNI_PREFIX lib_FastaFileElem
JNIEXPORT jint JNICALL JNI_METHOD(nativeGetId(JNIEnv* env, jclass, jlong nativePtr)) {
    jnipp::Env::Scope scope(env);

    auto elem(reinterpret_cast<utils::FastaFileElem*>(nativePtr));

    return reinterpret_cast<jint>((int) elem->id_);
}

JNIEXPORT jstring JNICALL JNI_METHOD(nativeGetName(JNIEnv* env, jclass, jlong nativePtr)) {
    jnipp::Env::Scope scope(env);

    auto elem(reinterpret_cast<utils::FastaFileElem*>(nativePtr));
    if (!elem) printf("Greska\n");
    jnipp::String name(jnipp::String::create(elem->name_).steal());
    return name.operator _jstring *();
}

JNIEXPORT jstring JNICALL JNI_METHOD(nativeGetSequence(JNIEnv* env, jclass, jlong nativePtr)) {
    jnipp::Env::Scope scope(env);

    auto elem(reinterpret_cast<utils::FastaFileElem*>(nativePtr));

    jnipp::String seq(jnipp::String::create(elem->sequence_).steal());
    return seq.operator _jstring *();
}
