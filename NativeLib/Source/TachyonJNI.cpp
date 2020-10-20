#include "TachyonJNI.hpp"
#include "TachyonResultJNI.hpp"

#define JNI_PREFIX lib_Tachyon

#include "jnimacros.h"
#include "../../TachyonLib/Source/Tachyon.hpp"


JNIEXPORT void JNICALL JNI_METHOD(nativeCreateIndexes(JNIEnv* env, jclass, jstring inputPath, jstring outputPath, jint numberOfThreads)) {

    jnipp::Env::Scope score(env);

    jnipp::Ref<jnipp::String> input(inputPath);
    jnipp::Ref<jnipp::String> output(outputPath);

    IndexingOptions options;
    options.numberOfThreads = numberOfThreads;
    tachyon::Database::makeIndexes(input.std_str().c_str(), output.std_str().c_str(), options);
}

JNIEXPORT jlong JNICALL JNI_METHOD(nativeInitialize(JNIEnv * env, jclass, jstring jPath)) {
    jnipp::Env::Scope score(env);

    const auto path = jnipp::Ref<jnipp::String>(jPath);

    return reinterpret_cast<jlong>(new tachyon::Tachyon{ tachyon::Database::loadDatabase(path.std_str().c_str()) });
}

JNIEXPORT jlong JNICALL JNI_METHOD(nativeSearchP(JNIEnv* env, jclass, jlong tachyonPtr, jstring jFasta, jint numberOfThreads)) {

    jnipp::Env::Scope scope(env);

    jnipp::Ref<jnipp::String> fasta(jFasta);

    std::vector<tachyon::ProteineQuery> queries;

    for(const auto& it : utils::createEntriesFromFastaString(fasta)) {
        queries.push_back(it);
    }

    auto tachyon(reinterpret_cast<tachyon::Tachyon*>(tachyonPtr));

    return reinterpret_cast<jlong>(new TachyonResultJNI{tachyon->search({ std::move(queries) }, numberOfThreads)});
}

JNIEXPORT void JNICALL JNI_METHOD(nativeDelete(JNIEnv* env, jclass, jlong tachyonPtr)) {

    jnipp::Env::Scope scope(env);

    auto tachyon(reinterpret_cast<tachyon::Tachyon*>(tachyonPtr));

    delete tachyon;
}
