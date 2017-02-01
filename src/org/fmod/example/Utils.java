package org.fmod.example;
/**
 * Created by 張鵬輝 on 2017/2/1.
 * Email: 1344670918@qq.com
 */
public class Utils {
	//音效类型
		public static final int MODE_NORMAL=0;
		public static final int MODE_LUOLI=1;
		public static final int MODE_DASHU=2;
		public static final int MODE_JINGSONG=3;
		public static final int MODE_GAOGUAI=4;
		public static final int MODE_KONGLING=5;
	/**
	 * 音效处理传2个值
	 * @param path 音频文件路径
	 * @param type 音频文件类型
	 */
		public native static void fix(String path,int type);
		static{
				System.loadLibrary("fmodL");
				System.loadLibrary("fmod");
				System.loadLibrary("sound");
		}
}
