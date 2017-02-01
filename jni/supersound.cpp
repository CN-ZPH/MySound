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
	system->init(32, FMOD_INIT_NORMAL, NULL);
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
