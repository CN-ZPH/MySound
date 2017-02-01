package org.fmod.example;

import java.io.File;
import java.io.InputStream;

import org.fmod.FMOD;

import com.zph.mysound.R;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.widget.Toast;
/**
 * Created by 張鵬輝 on 2017/2/1.
 * Email: 1344670918@qq.com
 */

public class SoundActivity extends Activity{
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		FMOD.init(this);
		setContentView(R.layout.activity_main);
	}
	public void mFix(View v) {
		//音频路径可以去获取麦克风的数据，我这里写死了
		  String path = "file:///android_asset/zph.wav";
		switch (v.getId()) {
		case R.id.btn_normal:
			Utils.fix(path, Utils.MODE_NORMAL);
			break;
		case R.id.btn_luoli:
			Utils.fix(path, Utils.MODE_LUOLI);
			break;
		case R.id.btn_dashu:
			Utils.fix(path, Utils.MODE_DASHU);
			break;
		case R.id.btn_jingsong:
			Utils.fix(path, Utils.MODE_JINGSONG);
			break;
		case R.id.btn_gaoguai:
			Utils.fix(path, Utils.MODE_GAOGUAI);
			break;
		case R.id.btn_kongling:
			Utils.fix(path, Utils.MODE_KONGLING);
			break;
		default:
			break;
		}
	}

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		FMOD.close();
	}
}
