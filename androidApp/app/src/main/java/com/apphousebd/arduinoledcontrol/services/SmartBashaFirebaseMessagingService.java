package com.apphousebd.arduinoledcontrol.services;

import android.app.Service;
import android.util.Log;

import com.apphousebd.arduinoledcontrol.WarningNotification;
import com.google.firebase.messaging.FirebaseMessagingService;
import com.google.firebase.messaging.RemoteMessage;

import java.util.Map;

/**
 * Created by Asif Imtiaz Shaafi
 * Email: a15shaafi.209@gmail.com
 * Date: February, 2018
 * Copyright (c) 2018, AppHouseBD. All rights reserved.
 */

public class SmartBashaFirebaseMessagingService extends FirebaseMessagingService {

    private static final String TAG = "SmartBashaFirebase";

    @Override
    public void onMessageReceived(RemoteMessage remoteMessage) {
        if (remoteMessage.getData() != null)
        {
            Map<String , String > data = remoteMessage.getData();
            Log.d(TAG, "onMessageReceived: " + data);
            WarningNotification.notify(getBaseContext(), data.get("channel"));
        }
    }
}
