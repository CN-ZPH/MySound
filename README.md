# MySound
仿QQ语音变声功能实现
> 版权声明：转载必须注明本文转自张鹏辉的博客: http://blog.csdn.net/qingtiangg
<br>
  >大家好，这是我从业以来第一篇博客，给大家拜个晚年，祝大家鸡年大吉，幸福美满、事业有成。好了请容许我唠叨几万字。首先自我介绍下
我叫张鹏辉大家可以叫我道长，我是老司······哦不，我是新人请大家多多关照。 一直都是看别人的博客成长（复制、黏贴），总想自己写点什么，发出来，让大家去点评，给建议互相成长。这也是我的初衷，也就有了这个博客的诞生。。。在下目前从事移动开发所以目前以移动端内容来发布，记录一些日常开发中碰到的坑，和项目中一些模块拿出来开源互相学习（几万字略。。。我用意念给你们介绍完了，就当你们了解我了）

闲话不多说了开撸，如题今天请跟贫道一步步来实现球球语音变声功能
## 分析
首先我们来看下QQ语音变声效果，如下图所示：
  
![这里写图片描述](http://img.blog.csdn.net/20170201013154092?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUWluZ1RpYW5HRw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


点击发送语音在点击相应的生效发出去就变声了，没玩过的可以打开QQ试下，那么他们是怎么做到让声音变声呢！看官们接着往下看。

首先我们知道，录制人声到手机，这些音频数据是由二进制十六进制组成，如果我们自己去用算法处理这些数据这个难度是比较大的。

那么怎么办？？？庆幸吧我们Android开发中有个NDK的东西，简单来说就是我们在java层调用c/c++的一些强大的开源库，(这里不讲NDK基础如果想了解请谷歌一下)。

网上有很多开源的音频处理库。我不知道腾讯是用的那个，但我们今天用到的是FMOD音频引擎，如果你做过游戏开发相信你一定接触过，这里附上FMOD官网链接大家可以去简单的看下介绍，国内FMOD的中文资料几乎没有，所以还是建议你去官网了解，[点我进fmod官网](http://www.fmod.org/)，这里就不详细介绍了，他的主要功能就是对音频进行处理，比如你玩赛车类游戏你的引擎轰鸣声，会随着你的速度变化而变化，游戏中有多个视角，比如车内切换到车外，你听到的声音是不是不一样的，再比如我们当年玩的cs游戏，你附近的敌人在你不同角度开枪，你听到的声音也是不一样的，（！！！暴露年龄了。。。）

<br>
<br>
或许我们来个图更好理解：

![这里写图片描述](http://img.blog.csdn.net/20170201024843561?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUWluZ1RpYW5HRw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

>听到脚步声或枪声，就可以判断出敌人在你什么位置，不同的位置方位明显听到的声音不同

##开始

好的大体环境介绍完了，接下来我们看代码。


首先我们新建eclipse工程‘’mysound‘’（这里不要问我为什么不用Android Studio开发as现在多火啊！，因为ndk开发as还是有些问题的，有些坑我也没解决，我能怎么办？我也很绝望呀 。当然了以后我主要还是以as去写，如果今天的项目你能移植到as里面请联系我！）

首先我们导入一下FMOD的so动态库和一些需要用到的头文件。

![这里写图片描述](http://img.blog.csdn.net/20170201163033494?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUWluZ1RpYW5HRw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


>今天的所有代码和用到的库我都会放到github上，所以不用担心这些文件去哪找。


###1.接了下我们将工程转成C/C++工程 Add Native Support

 ![这里写图片描述](http://img.blog.csdn.net/20170201141626146?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUWluZ1RpYW5HRw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

之后会给你生成android.mk文件 我们来配置一下里面的文件，引入提供的预编译的so库把他们放到jni目录下，还有他自己的函数的实现也要引入看下面

```
LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)
LOCAL_MODULE := fmod
LOCAL_SRC_FILES := libfmod.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := fmodL
LOCAL_SRC_FILES := libfmodL.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := sound
LOCAL_SRC_FILES :=effect_fix.cpp
LOCAL_SHARED_LIBRARIES := fmod fmodL
LOCAL_LDLIBS := -llog
LOCAL_CPP_FEATURES :=exceptions
include $(BUILD_SHARED_LIBRARY)

```
>LOCAL_MODULE    := sound 自己的动态库名切记要和System.loadLibrary("sound");一致这坑我踩过  ，fmod fmodL这是2个预编译的库
>如果你引入的是官网提供的库可能会出现一些路径的问题改一下就好我的已经改了。

>划重点   FMOD 用到了标准模板库STL，说用这玩意会变慢？，但我没感觉出来
>安卓ndk里面已经有了一个最小的c++运行库，想用那么我们就需要另外的配置

怎么配置呢？看ndk提供的文档 在我们下载的ndk目录里就有解决方法APP_STL

![这里写图片描述](http://img.blog.csdn.net/20170201170459182?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUWluZ1RpYW5HRw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

 ![这里写图片描述](http://img.blog.csdn.net/20170201145442601?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUWluZ1RpYW5HRw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


###2.创建Application.mk 支持C++异常处理，标准模板块

里面就一句话  :
>APP_STL := gnustl_static


3.配置完我们来写java代码

布局我就不在这贴出来了几个按钮，看关键代码

我们先来个工具类Utils

```
public class Utils {
	    //音效类型 大叔萝莉什么的类型
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
		//加载动态库
		//要和你android.mk文件一致
		static{
				System.loadLibrary("fmodL");
				System.loadLibrary("fmod");
				System.loadLibrary("sound");
		}
}
```
###接下来我们生成头文件

![这里写图片描述](http://img.blog.csdn.net/20170201155913993?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUWluZ1RpYW5HRw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后到项目F5刷新一下src目录下会多出个.h的文件把它拖到jni目录下
继续在jni目录下创建一个c++源文件 supersound.cpp代码

```
#include "inc/fmod.hpp"
#include <stdlib.h>
#include <unistd.h>
#include  "org_fmod_example_Utils.h"

#include <android/log.h>
#define LOGI(FORMAT,...) __android_log_print(ANDROID_LOG_INFO,"zph",FORMAT,##__VA_ARGS__);
#define LOGE(FORMAT,...) __android_log_print(ANDROID_LOG_ERROR,"zph",FORMAT,##__VA_ARGS__);

#define MODE_NORMAL 0
#define MODE_LUOLI 1
#define MODE_DASHU 2
#define MODE_JINGSONG 3
#define MODE_GAOGUAI 4
#define MODE_KONGLING 5
using namespace FMOD;

JNIEXPORT void JNICALL Java_org_fmod_example_Utils_fix(JNIEnv *env,
		jclass jcls, jstring path_jstr, jint type) {
	//初始化
	System *system;
	Sound *sound;
	DSP *dsp;
	bool playing = true;
	Channel *channel; 
	float frequency = 0;
	System_Create(&system);
	system->init(32, FMOD_INIT_NORMAL, NULL);//32通道
	const char* path_cstr = env->GetStringUTFChars(path_jstr, NULL);
	try {

		//创建声音
		system->createSound(path_cstr, FMOD_DEFAULT, NULL, &sound);
		switch (type) {
		case MODE_NORMAL:
			//原生播放
			LOGI("%s", path_cstr);
			system->playSound(sound, 0, false, &channel);
			LOGI("%s", "fix normal");
			break;
		case MODE_LUOLI:
			//loli
			//DSP digital singal process
			//dsp->音效
			//dsp 提升或者降低音调的一种音效
			system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
			//设置音调的参数
			dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 8.0);
			//添加进到channel
			system->playSound(sound, 0, false, &channel);
			channel->addDSP(0, dsp);
			break;
		case MODE_DASHU:
			//大叔
			system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
			//设置音调的参数
			dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 0.8);
			//添加进到channel
			system->playSound(sound, 0, false, &channel);
			channel->addDSP(0, dsp);
			break;
		case MODE_JINGSONG:
			//惊悚
			system->createDSPByType(FMOD_DSP_TYPE_TREMOLO, &dsp);
			dsp->setParameterFloat(FMOD_DSP_TREMOLO_SKEW, 5);
			system->playSound(sound, 0, false, &channel);
			channel->addDSP(0, dsp);
			break;
		case MODE_GAOGUAI:
			//搞怪
			//提高说话的速度
			system->playSound(sound, 0, false, &channel);
			channel->getFrequency(&frequency);
			frequency = frequency * 2;
			channel->setFrequency(frequency);
			break;
		case MODE_KONGLING:
			//空灵
			system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp);
			dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY, 300);
			dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 20);
			system->playSound(sound, 0, false, &channel);
			channel->addDSP(0, dsp);
			break;
		default:
			break;
		}
	} catch (...) {
		LOGE("%s", "发生异常");
		goto end;
	}
	system->update();
	//释放资源
	//单位是微妙
	//每秒钟判断下是否是播放
	while (playing) {
		channel->isPlaying(&playing);
		usleep(1000 * 1000);
	}
	goto end;
	end: env->ReleaseStringUTFChars(path_jstr, path_cstr);
	sound->release();
	system->close();
	system->release();
}

```

举个例子比如在我们播放一段mp3的时候里面又人声，乐器声等，如果我想对其中一个钢琴的声音加一个特效


![这里写图片描述](http://img.blog.csdn.net/20170201172430201?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUWluZ1RpYW5HRw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

>那么怎么加呢！这就用到它了Channel *channel;   一般是32个通道修改完以后混音






>好了到这里关键代码已经写完，我们可以测试玩玩了.


##项目源码下载地址：

https://github.com/CN-ZPH/MySound.git
 觉得不错请点一个star蛤！
 有问题下面留言评论，我看到会回复。


