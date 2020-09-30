#pragma once

#undef JNIEXPORT
#define JNIEXPORT extern "C" __attribute__((visibility("default")))

#ifndef JNI_PREFIX
#error "Please define JNI_PREFIX as the package name for the methods"
#endif

#define JNI_METHOD_INTERNAL1(prefix, name) Java_##prefix##_##name
#define JNI_METHOD_INTERNAL2(prefix, name) JNI_METHOD_INTERNAL1(prefix, name)
#define JNI_METHOD(name) JNI_METHOD_INTERNAL2(JNI_PREFIX, name)

#define JNI_PARAMS JNIEnv* env, jclass jclazz, jlong nativePtr
#define JNI_STATIC_PARAMS JNIEnv* env, jclass jclazz

template<typename T>
inline T& jlong_as(jlong ptr) {
    auto object = reinterpret_cast<T*>(ptr);
    return *object;
}
