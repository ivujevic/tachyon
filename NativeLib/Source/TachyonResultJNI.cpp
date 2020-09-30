#include "TachyonResultJNI.hpp"

#include "jnipp.h"

#define JNI_PREFIX lib_TachyonResult

#include "jnimacros.h"


JNIEXPORT void JNICALL JNI_METHOD(nativeDeletePtr(JNIEnv* env, jclass, jlong resultPtr)) {
    jnipp::Env::Scope scope(env);

    auto result(reinterpret_cast<TachyonResultJNI*>(resultPtr));
    if (!result) return;

    delete result;
}

#undef JNI_PREFIX

#define JNI_PREFIX lib_TachyonProteineResult

JNIEXPORT jlongArray JNICALL JNI_METHOD(nativeGetEntries(JNIEnv* env, jclass, jlong resultPtr)) {
    jnipp::Env::Scope scope(env);

    auto result(reinterpret_cast<TachyonResultJNI*>(resultPtr));

    const auto& proteineResult(std::get<tachyon::TachyonResult::ProteineResult>(result->result_.entries_));


    jnipp::Array<jlong> retArray(jnipp::Array<jlong>::create(proteineResult.size()).steal());

    for (int i = 0; i < (int) proteineResult.size(); ++i) {
        retArray.set(i, reinterpret_cast<jlong>(&proteineResult[i]));
    }

    return retArray.operator _jlongArray *();
}


#undef JNI_PREFIX

#define JNI_PREFIX lib_TachyonNucleotideResult

JNIEXPORT jlongArray  JNICALL JNI_METHOD(nativeGetEntries(JNIEnv* env, jclass, jlong resultPtr)) {

    jnipp::Env::Scope scope(env);

    auto result(reinterpret_cast<TachyonResultJNI*>(resultPtr));

    const auto& nucleotideResults(std::get<std::vector<tachyon::TachyonResult::NucleotideResult>>(result->result_.entries_));

    jnipp::Array<jlong> retArray(jnipp::Array<jlong>::create(nucleotideResults.size()).steal());

    for (int i = 0; i < (int) nucleotideResults.size(); ++i) {
        retArray.set(i, reinterpret_cast<jlong>(&nucleotideResults[i]));
    }

    return retArray.operator _jlongArray *();
}

#undef JNI_PREFIX
#define JNI_PREFIX lib_TachyonNucleotideResultEntry

JNIEXPORT jlong  JNICALL JNI_METHOD(nativeGetQuery(JNIEnv* env, jclass, jlong resultPtr)) {

    jnipp::Env::Scope scope(env);

    auto result(reinterpret_cast<tachyon::TachyonResult::NucleotideResult*>(resultPtr));

    return reinterpret_cast<jlong>(result->nucleotideQuery_.get());
}

JNIEXPORT jlongArray JNICALL JNI_METHOD(nativeGetEntries(JNIEnv* env, jclass, jlong resultPtr)) {

    jnipp::Env::Scope scope(env);

    auto result(reinterpret_cast<tachyon::TachyonResult::NucleotideResult*>(resultPtr));

    jnipp::Array<jlong> retArray(jnipp::Array<jlong>::create(result->entries_.size()).steal());
    for (int i = 0; i < (int) result->entries_.size(); ++i) {
        retArray.set(i, reinterpret_cast<jlong>(&result->entries_[i]));
    }

    return retArray.operator _jlongArray *();
}
