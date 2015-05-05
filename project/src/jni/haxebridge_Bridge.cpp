#include "haxebridge_Bridge.h"
#include "haxebridge.h"

    // Keep track of JNI env and method to let java receive messages
static JNIEnv *s_haxebridge_jni_env = NULL;

value s_haxebridge_jni_local_listener(const char *channel, const char *message) {
        // Retrieve env
    JNIEnv *env = s_haxebridge_jni_env;

        // Ensure the env is available (if not, that means java is not listening to anything)
    if (env == NULL) return alloc_null();

    jclass cls = env->FindClass("haxebridge/Bridge");
    jmethodID method = env->GetStaticMethodID(cls, "receiveFromHaxe", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");

        // Call java method
    jstring channel_jstr = env->NewStringUTF(channel);
    jstring message_jstr = env->NewStringUTF(message);
    jstring result_jstr = (jstring) env->CallStaticObjectMethod(cls, method, channel_jstr, message_jstr);
    env->DeleteLocalRef(channel_jstr);
    env->DeleteLocalRef(message_jstr);

        // Compute and return result for haxe side
    if (result_jstr == NULL) return alloc_null();
    const char *result_str = env->GetStringUTFChars(result_jstr, 0);
    value result = alloc_string(result_str);
    env->ReleaseStringUTFChars(result_jstr, result_str);
    return result;
}

/*
 * Class:     haxebridge_Bridge
 * Method:    sendMessage
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_haxebridge_Bridge_sendMessage(JNIEnv *env, jclass cls, jstring channel, jstring message) {
        // Get C strings
    const char *channel_str = env->GetStringUTFChars(channel, 0);
    const char *message_str = env->GetStringUTFChars(message, 0);
        // Call C++ send function
    const char *result = haxebridge::native_send(channel_str, message_str);
        // Release strings
    env->ReleaseStringUTFChars(channel, channel_str);
    env->ReleaseStringUTFChars(message, message_str);
        // Return result
    if (result == NULL) return NULL;
    return env->NewStringUTF(result);
}

/*
 * Class:     haxebridge_Bridge
 * Method:    initNativeListener
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_haxebridge_Bridge_initNativeListener(JNIEnv *env, jclass cls) {
        // Keep JNIEnv* instance
    s_haxebridge_jni_env = env;

        // Map the local listener on C++ side
    haxebridge::native_listen(&s_haxebridge_jni_local_listener);
}
