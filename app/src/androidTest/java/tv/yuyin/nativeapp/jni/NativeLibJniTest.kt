package tv.yuyin.nativeapp.jni

import android.support.test.runner.AndroidJUnit4
import org.junit.Test
import org.junit.runner.RunWith

/**
 * @name NativeApp
 * @class name：tv.yuyin.nativeapp.jni
 * @decribe describe
 * @author Threshold
 * @time 2019/5/7 14:51
 * @change
 */
@RunWith(AndroidJUnit4::class)
class NativeLibJniTest {

    @Test
    fun testRingMsgQueue(){
        NativeLibJni.setLoggable(true)
        NativeLibJni.testRingMsgQueue()
    }

}