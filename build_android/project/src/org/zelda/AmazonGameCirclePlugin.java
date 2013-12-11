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

package org.zelda;

import java.util.EnumSet;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

import com.amazon.ags.api.AmazonGamesCallback;
import com.amazon.ags.api.AmazonGamesClient;
import com.amazon.ags.api.AmazonGamesFeature;
import com.amazon.ags.api.AmazonGamesStatus;

public class AmazonGameCirclePlugin extends Plugin {
	public AmazonGameCirclePlugin(Activity activity) {
		super(activity);
	}

	//reference to the agsClient	
	AmazonGamesClient agsClient = null;
	 
	AmazonGamesCallback callback;
	 
	//list of features the game uses
	EnumSet<AmazonGamesFeature> myGameFeatures;
		
	@Override
	protected void onPauseImpl() {
	    if (agsClient != null) {
	        AmazonGamesClient.release();
	    }
	}

	@Override
	protected void onResumeImpl() {
		//mActivity.runOnUiThread(new Runnable() {
		//	public void run() {
	    AmazonGamesClient.initialize(mActivity, callback, myGameFeatures);
		//	}});
	}
	
	@Override
	protected void onCreateImpl() {
	    callback = new AmazonGamesCallback() {
	        @Override
	        public void onServiceNotReady(AmazonGamesStatus status) {
	            //unable to use service
	        	Log.e("AmazonGamesCircle", "Unable to use GameCircle (" + status.toString() + ")");
	        	AmazonGamesClient.shutdown();
	        }
	        @Override
	        public void onServiceReady(AmazonGamesClient amazonGamesClient) {
	            agsClient = amazonGamesClient;
	        	Log.i("AmazonGamesCircle", "Ready to use GameCircle");
	        	//Toast.makeText(mActivity, "Ready to use GameCircle", Toast.LENGTH_LONG).show();
	            //ready to use GameCircle
	        }
	    };
	    //myGameFeatures = EnumSet.of(AmazonGamesFeature.Achievements, AmazonGamesFeature.Leaderboards);
	    myGameFeatures = AmazonGamesFeature.all();
	}

	@Override
	protected void onDestroyImpl() {
        AmazonGamesClient.shutdown();
	}
	@Override
	protected void onStopImpl() {
		AmazonGamesClient.shutdown();
	}

}
