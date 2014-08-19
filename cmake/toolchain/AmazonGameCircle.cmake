if (PROJECT_USE_AMAZON_GAME_CIRCLE) 
  set(PROJECT_AMAZON_GAME_CIRCLE_ANDROID_MK 
"include $(CLEAR_VARS)
LOCAL_MODULE := AmazonGamesJni
LOCAL_SRC_FILES := libs/libAmazonGamesJni.so
include $(PREBUILT_SHARED_LIBRARY)")

  set(PROJECT_AMAZON_GAME_CIRCLE_LIBRARY "AmazonGamesJni")

  set(PROJECT_AMAZON_GAME_CIRCLE_ACTIVITY
 "<activity android:name=\"com.amazon.ags.html5.overlay.GameCircleUserInterface\"
	      android:theme=\"\@style/GCOverlay\" android:hardwareAccelerated=\"false\"></activity>
    <activity
	android:name=\"com.amazon.identity.auth.device.authorization.AuthorizationActivity\"
	android:theme=\"\@android:style/Theme.NoDisplay\"
	android:allowTaskReparenting=\"true\"
	android:launchMode=\"singleTask\">
      <intent-filter>
	<action android:name=\"android.intent.action.VIEW\" />
	<category android:name=\"android.intent.category.DEFAULT\" />
	<category android:name=\"android.intent.category.BROWSABLE\" />
	<data android:host=\"org.mencus.game\" android:scheme=\"amzn\" />
      </intent-filter>
    </activity>
    <activity android:name=\"com.amazon.ags.html5.overlay.GameCircleAlertUserInterface\"
	      android:theme=\"\@style/GCAlert\" android:hardwareAccelerated=\"false\"></activity>
    <receiver
	android:name=\"com.amazon.identity.auth.device.authorization.PackageIntentReceiver\"
	android:enabled=\"true\">
      <intent-filter>
	<action android:name=\"android.intent.action.PACKAGE_INSTALL\" />
	<action android:name=\"android.intent.action.PACKAGE_ADDED\" />
	<data android:scheme=\"package\" />
      </intent-filter>
    </receiver>"
)
endif()