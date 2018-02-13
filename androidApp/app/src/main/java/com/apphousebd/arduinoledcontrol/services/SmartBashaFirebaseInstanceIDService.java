package com.apphousebd.arduinoledcontrol.services;

import android.app.Service;

import com.google.firebase.iid.FirebaseInstanceIdService;

/**
 * Created by Asif Imtiaz Shaafi
 * Email: a15shaafi.209@gmail.com
 * Date: February, 2018
 * Copyright (c) 2018, AppHouseBD. All rights reserved.
 */

public class SmartBashaFirebaseInstanceIDService extends FirebaseInstanceIdService {
    @Override
    public void onTokenRefresh() {
        super.onTokenRefresh();
    }
}
