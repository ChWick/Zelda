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


import android.widget.Toast;

import com.google.android.gms.ads.AdRequest;

public class AdListener extends com.google.android.gms.ads.AdListener {
	private NativeActivity mContext;
	
    public AdListener(NativeActivity context) {
        this.mContext = context;
    }

    @Override
    public void onAdLoaded() {
        //Toast.makeText(mContext, "onAdLoaded()", Toast.LENGTH_SHORT).show();
    	if (mContext.showAdWhenLoaded()) {
    		mContext.getInterstitialAd().show();
    	}
    }

    @Override
    public void onAdFailedToLoad(int errorCode) {
        String errorReason = "";
        switch(errorCode) {
            case AdRequest.ERROR_CODE_INTERNAL_ERROR:
                errorReason = "Internal error";
                break;
            case AdRequest.ERROR_CODE_INVALID_REQUEST:
                errorReason = "Invalid request";
                break;
            case AdRequest.ERROR_CODE_NETWORK_ERROR:
                errorReason = "Network Error";
                break;
            case AdRequest.ERROR_CODE_NO_FILL:
                errorReason = "No fill";
                break;
        }
	// ad blocker, etc...
        //Toast.makeText(mContext, String.format("onAdFailedToLoad(%s)", errorReason),
	//      Toast.LENGTH_SHORT).show();
        mContext.setAdClosed(true);
    }

    @Override
    public void onAdOpened() {
        //Toast.makeText(mContext, "onAdOpened()", Toast.LENGTH_SHORT).show();
    }

    @Override
    public void onAdClosed() {
        //Toast.makeText(mContext, "onAdClosed()", Toast.LENGTH_SHORT).show();
    	mContext.closeAd();
    }

    @Override
    public void onAdLeftApplication() {
        //Toast.makeText(mContext, "onAdLeftApplication()", Toast.LENGTH_SHORT).show();
    }
}
