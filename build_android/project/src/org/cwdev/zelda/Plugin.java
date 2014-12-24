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

import android.app.Activity;
import android.os.Bundle;

public abstract class Plugin {
	protected Activity mActivity;
	private boolean mStarted;
	
	public Plugin(Activity activity) {
		mActivity = activity;
		mStarted = false;
	}
	
	public boolean isStarted() {return mStarted;}
	public void start() {
		mStarted = true;
		onCreate();
		onResume();
	}
	
	public void onCreate() {
		if (mStarted) {
			onCreateImpl();
		}
	}
	public void onDestroy() {
		if (mStarted) {
			onDestroyImpl();
		}
	}
	public void onPause() {
		if (mStarted) {
			onPauseImpl();
		}
	}
	public void onResume() {
		if (mStarted) {
			onResumeImpl();
		}
	}
	public void onStop() {
		if (mStarted) {
			onStopImpl();
		}
	}
	
	protected abstract void onCreateImpl();
	protected abstract void onDestroyImpl();
	protected abstract void onPauseImpl();
	protected abstract void onResumeImpl();
	protected abstract void onStopImpl();
}
