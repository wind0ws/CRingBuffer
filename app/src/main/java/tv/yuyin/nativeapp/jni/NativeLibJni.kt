package tv.yuyin.nativeapp.jni

class NativeLibJni {

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    companion object {

        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }

        @JvmStatic
        external fun stringBytesFromJni(content: String):ByteArray

        @JvmStatic
        external fun setLoggable(isLoggable: Boolean)

        @JvmStatic
        external fun testRingMsgQueue();
    }

}