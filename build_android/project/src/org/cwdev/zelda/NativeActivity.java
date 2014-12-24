/*****************************************************************************
 * Copyright 2014 Christoph Wick
 *
 * This file is part of Mencus.
 *
 * Mencus is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Mencus is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Mencus. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

package org.cwdev.zelda;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Locale;

import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.InterstitialAd;

import android.app.Activity;
import android.app.Dialog;
import android.os.Bundle;
import android.util.Log;
import android.widget.PopupWindow;
import android.widget.TextView;

public class NativeActivity extends android.app.NativeActivity {
	List<Plugin> mPlugins = new ArrayList<Plugin>();
	
	PopupWindow popUp;

	NativeActivity _activity;
	private InterstitialAd mInterstitial;
	private Runnable mThread;
	private boolean mAddClosed = false;
	private boolean mShowAdWhenLoaded = true;
	private Dialog mLoadDialog = null;
	
	InterstitialAd getInterstitialAd() {return mInterstitial;}
	void setAdClosed(boolean addClosed) {mAddClosed = addClosed;}
	boolean showAdWhenLoaded() {return mShowAdWhenLoaded;}

    static {
    	if (GlobalBuildDefines.PROJECT_USE_AMAZON_GAME_CIRCLE) {
    		System.loadLibrary("AmazonGamesJni");
    	}
    	//System.loadLibrary("Mencus");
    }
    
	@Override
	public void onCreate(Bundle savedInstanceState) {
		_activity = this;
		super.onCreate(savedInstanceState);

		
		mPlugins.add(new AmazonGameCirclePlugin(this));
		for (Iterator<Plugin> i = mPlugins.iterator(); i.hasNext();) {
			i.next().onCreate();
		}
        //Toast.makeText(this, "Loading, please wait...", Toast.LENGTH_LONG).show();
		//showAdPopup();
        //Toast.makeText(this, "popup done", Toast.LENGTH_SHORT).show();

        mLoadDialog = new Dialog(this, android.R.style.Theme_Black_NoTitleBar_Fullscreen) {
        	@Override
        	public void onStop() {
        		Log.i("Ogre", "onStop dialog");
        		_activity.mLoadDialog = null;
        	}
        };
        mLoadDialog.setContentView(R.layout.activity_zelda);
        mLoadDialog.show();
	}
	public void onStop() {
		super.onStop();

		for (Iterator<Plugin> i = mPlugins.iterator(); i.hasNext();) {
			i.next().onStop();
		}
		mLoadDialog = null;
	}
	public void onPause() {
		super.onPause();
		mLoadDialog = null;
		
		for (Iterator<Plugin> i = mPlugins.iterator(); i.hasNext();) {
			i.next().onPause();
		}
	}
	public void onResume() {
		super.onResume();
		
		for (Iterator<Plugin> i = mPlugins.iterator(); i.hasNext();) {
			i.next().onResume();
		}
	}
	public void onDestroy() {		
		super.onDestroy();
		mLoadDialog = null;
	}
	
	public void startPlugins() {
		for (Iterator<Plugin> i = mPlugins.iterator(); i.hasNext();) {
			i.next().start();
		}
	}

	// Our popup window, you will call it from your C/C++ code later
	public void showAdPopup() {
		_activity.runOnUiThread(new Runnable() {
			public void run() {
				mAddClosed = false;
				if (mInterstitial != null && mInterstitial.isLoaded()) {
					mInterstitial.show();
				}
				else {
					closeAd();
					loadAd(true);
				}
			}
		});
	}
	
	public boolean adPopupClosed() {
		return mAddClosed;
	}
	
	public void closeAd() {
		mShowAdWhenLoaded = false;
		mInterstitial = null;
		mThread = null;
		mAddClosed = true;
	}
	
	public void preloadAd() {
		loadAd(false);
	}
	private void loadAd(boolean showWhenLoaded) {
		mAddClosed = false;
		mShowAdWhenLoaded = showWhenLoaded;
		mThread = new Runnable()  {
			 @Override
			 public void run()  {
			    //Toast.makeText(_activity, "Loading, please wait...", Toast.LENGTH_LONG).show();
			    mInterstitial = new InterstitialAd(_activity);
		        mInterstitial.setAdUnitId("ca-app-pub-1316406434207336/9422601805");
		        mInterstitial.setAdListener(new AdListener(_activity));
		        mInterstitial.loadAd(new AdRequest.Builder()
	        		//.addTestDevice(AdRequest.DEVICE_ID_EMULATOR)
	        		//.addTestDevice("5379191936D78128252BAEC2CA476429")
	        		.addTestDevice("5379191936D78128252BAEC2CA476429")
	        		.build());
			 }
		 };
		 _activity.runOnUiThread(mThread);
	}
	
	public void setLoadText(String text) {
		if (mLoadDialog == null) {return;}
		TextView tv = (TextView)mLoadDialog.findViewById(R.id.zelda_load_text);
		if (tv == null) {return;}
		tv.setText(text);
	}
	public void closeLoadDialog() {
		if (mLoadDialog == null) {return;}
		mLoadDialog.dismiss();
		mLoadDialog = null;
	}
	public String getLanguage() {
		return Locale.getDefault().getLanguage();
	}
}

