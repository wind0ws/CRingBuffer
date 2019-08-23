package tv.yuyin.nativeapp.jni

class NativeTestsJni {

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    companion object {

        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("tests")
        }

        @JvmStatic
        external fun stringBytesFromJni(content: String):ByteArray

        @JvmStatic
        external fun setLoggable(isLoggable: Boolean)

        @JvmStatic
        external fun testRingMsgQueue()

        @JvmStatic
        external fun testQueueHandler()

        @JvmStatic
        external fun testSemaphore()

        @JvmStatic
        external fun testSimpleQueue()

        @JvmStatic
        external fun testRingBuf()
    }

}