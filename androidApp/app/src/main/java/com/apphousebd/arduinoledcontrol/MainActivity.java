package com.apphousebd.arduinoledcontrol;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.support.annotation.NonNull;
import android.support.v4.widget.SwipeRefreshLayout;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.firebase.ui.auth.AuthUI;
import com.firebase.ui.auth.IdpResponse;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.messaging.FirebaseMessaging;
import com.google.firebase.messaging.FirebaseMessagingService;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.util.Arrays;
import java.util.concurrent.TimeUnit;

import dmax.dialog.SpotsDialog;
import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.FormBody;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;

public class MainActivity extends AppCompatActivity implements SwipeRefreshLayout.OnRefreshListener {
    private static final String TAG = "MainActivity";
    private static final int FIREBASE_SIGN_IN = 1010;

    SwipeRefreshLayout refreshLayout;

    private ImageView channelOneImg, channelTwoImg, channelThreeImg, channelFourImg;
    private TextView channelOneText, channelTwoText, channelThreeText, channelFourText;

    private ImageView imageView;
    private TextView mTextView;

    private String urlHeader = "http://";
    //    private String ipAddress = "192.168.0.102";
//    String port = "105";
    private String localIpAddress = "apphousebd.com";//"192.168.0.101";

    private OkHttpClient client;

    private FirebaseAuth mFirebaseAuth = FirebaseAuth.getInstance();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        if (mFirebaseAuth.getCurrentUser() == null) {
            finish();
        }
        else {
            FirebaseMessaging.getInstance().subscribeToTopic("smartBasha");
        }

        client = new OkHttpClient.Builder()
                .writeTimeout(5, TimeUnit.SECONDS)
                .connectTimeout(5, TimeUnit.SECONDS)
                .build();

        channelOneImg = findViewById(R.id.channel_status_icon_1);
        channelTwoImg = findViewById(R.id.channel_status_icon_2);
        channelThreeImg = findViewById(R.id.channel_status_icon_3);
        channelFourImg = findViewById(R.id.channel_status_icon_4);

        channelOneText = findViewById(R.id.channel_status_text_1);
        channelTwoText = findViewById(R.id.channel_status_text_2);
        channelThreeText = findViewById(R.id.channel_status_text_3);
        channelFourText = findViewById(R.id.channel_status_text_4);

        refreshLayout = findViewById(R.id.reload);
        refreshLayout.setOnRefreshListener(this);

        new Handler().postDelayed(new Runnable() {
            @Override
            public void run() {

                updateChannelStates();
            }
        }, 500);
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_logout) {
            AuthUI.getInstance()
                    .signOut(this)
                    .addOnCompleteListener(new OnCompleteListener<Void>() {
                        public void onComplete(@NonNull Task<Void> task) {
                            // user is now signed out
                            finish();
                        }
                    });
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    public void lightControl(View v) {

        final android.app.AlertDialog dialog = new SpotsDialog(MainActivity.this, R.style.CustomSpotDialogUpdating);
        dialog.show();

        final String channelId = v.getContentDescription().toString();

        switch (channelId) {
            case "channel_1":
                imageView = channelOneImg;
                mTextView = findViewById(R.id.channel_status_text_1);
                break;
            case "channel_2":
                imageView = channelTwoImg;
                mTextView = findViewById(R.id.channel_status_text_2);
                break;
            case "channel_3":
                imageView = channelThreeImg;
                mTextView = findViewById(R.id.channel_status_text_3);
                break;
            case "channel_4":
                imageView = channelFourImg;
                mTextView = findViewById(R.id.channel_status_text_4);
                break;
            default:
                imageView = channelOneImg;
                mTextView = findViewById(R.id.channel_status_text_1);
                break;
        }

        String command = imageView.getTag().toString();

//        String url = urlHeader + ipAddress + ":" + port + "/" + channelId + "/" + command;

        String localUrl = urlHeader + localIpAddress + "/wifi/api/updateChannelStates.php";

        Log.d(TAG, "lightControl: local url: " + localUrl);

//        Log.d(TAG, "lightControl: url: " + url);

        RequestBody formBody = new FormBody.Builder()
                .add("submit", "submit")
                .add("channel", channelId)
                .add("state", command)
                .build();

        final Request request = new Request.Builder()
                .url(localUrl)
                .post(formBody)
                .build();

        client.newCall(request).enqueue(new Callback() {
            @Override
            public void onFailure(@NonNull Call call, @NonNull final IOException e) {
                Log.w(TAG, "onFailure: " + e.getLocalizedMessage(), e);
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        Toast.makeText(MainActivity.this, "Failed!", Toast.LENGTH_SHORT).show();
                        Log.w(TAG, "run: failed! " + e.getLocalizedMessage(), e);
                        if (dialog.isShowing())
                            dialog.dismiss();
                    }
                });
            }

            @Override
            public void onResponse(@NonNull Call call, @NonNull final Response response) throws IOException {
                if (response.isSuccessful()) {
                    final String result = response.body().string();
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
//                                Toast.makeText(MainActivity.this, "Result: " + result, Toast.LENGTH_SHORT).show();
                            Log.d(TAG, "run: result: " + result);
                            if (result.contains("success")) {
                                if (result.contains("on")) {
                                    imageView.setImageResource(R.drawable.on);
                                    imageView.setTag("led=off");
                                    mTextView.setText(channelId + " is on");
                                } else {
                                    imageView.setImageResource(R.drawable.off);
                                    imageView.setTag("led=on");
                                    mTextView.setText(channelId + " is off");
                                }
                            } else {
                                Toast.makeText(MainActivity.this, "failed!!", Toast.LENGTH_SHORT).show();
                                Log.d(TAG, "run: failed");
                            }

                            if (dialog.isShowing())
                                dialog.dismiss();
                        }
                    });
                } else {
                    Log.d(TAG, "onResponse: " + response.networkResponse().message());
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            Toast.makeText(MainActivity.this, "unsuccessful & failed!!", Toast.LENGTH_SHORT).show();
                            Log.d(TAG, "run: response unsuccessful");
                            if (dialog.isShowing())
                                dialog.dismiss();
                        }
                    });
                }
            }
        });
    }

    public void updateChannelStates() {
        final android.app.AlertDialog dialog = new SpotsDialog(MainActivity.this, R.style.CustomSpotDialogUpdating);
        dialog.show();

        Request request = new Request.Builder()
                .url(urlHeader + localIpAddress + "/wifi/api/getChannelStates.php")
                .build();

        client.newCall(request).enqueue(
                new Callback() {
                    @Override
                    public void onFailure(@NonNull Call call, @NonNull final IOException e) {
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                if (dialog.isShowing())
                                    dialog.dismiss();

                                Log.e(TAG, "run: error updating: " + e.getLocalizedMessage(), e);
                            }
                        });
                    }

                    @Override
                    public void onResponse(@NonNull Call call, @NonNull Response response) throws IOException {
                        if (response.isSuccessful()) {
                            String result = response.body().string();

                            Log.d(TAG, "onResponse: result: " + result);

                            final String jsonString = result.substring(
                                    result.indexOf("{"), result.lastIndexOf("}") + 1
                            );

                            Log.d(TAG, "onResponse: json: " + jsonString);

                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    try {
                                        JSONObject object = new JSONObject(jsonString);

                                        updateChannelUI(object, "channel_1",
                                                channelOneImg, channelOneText);
                                        updateChannelUI(object, "channel_2",
                                                channelTwoImg, channelTwoText);
                                        updateChannelUI(object, "channel_3",
                                                channelThreeImg, channelThreeText);
                                        updateChannelUI(object, "channel_4",
                                                channelFourImg, channelFourText);

                                        if (dialog.isShowing())
                                            dialog.dismiss();

                                    } catch (final JSONException e) {
                                        e.printStackTrace();
                                        Log.d(TAG, "onResponse: json parsing failed");
                                        if (dialog.isShowing())
                                            dialog.dismiss();

                                        Log.e(TAG, "run: error updating: " + e.getLocalizedMessage(), e);

                                    }
                                }
                            });

                        } else {
                            Log.d(TAG, "onResponse: response unsuccessful");
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    if (dialog.isShowing())
                                        dialog.dismiss();
                                    Toast.makeText(MainActivity.this, "Response unsuccessful", Toast.LENGTH_SHORT).show();
                                }
                            });
                        }
                    }
                }
        );
    }

    public void updateChannelUI(JSONObject object, String channel,
                                ImageView channelImg, TextView channelText) throws JSONException {

        String state = "led=";
        String channelState = object.getString(channel);

        if (channelState.contains("on")) {
            channelImg.setTag(state + "off");
            channelImg.setImageResource(R.drawable.on);
            channelText.setText(channel + " is on");

        } else {
            channelImg.setTag(state + "on");
            channelImg.setImageResource(R.drawable.off);
            channelText.setText(channel + " is off");
        }
    }

    @Override
    public void onRefresh() {

        updateChannelStates();
    }
//
//    public void getNewIP(View v) {
//        TextInputLayout layout = new TextInputLayout(MainActivity.this);
//
//        final EditText editText = new EditText(MainActivity.this);
//        LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(
//                LinearLayout.LayoutParams.MATCH_PARENT,
//                LinearLayout.LayoutParams.WRAP_CONTENT
//        );
//
//        params.setMargins(4, 4, 4, 4);
//
//        layout.setLayoutParams(params);
//
//        editText.setHint("IP address");
//        editText.setLayoutParams(params);
//        editText.setInputType(InputType.TYPE_CLASS_TEXT);
//
//        layout.addView(editText);
//
//        new AlertDialog.Builder(MainActivity.this)
//                .setTitle("Enter new IP address")
//                .setView(layout)
//                .setPositiveButton("Ok", new DialogInterface.OnClickListener() {
//                    @Override
//                    public void onClick(DialogInterface dialog, int which) {
//                        if (!TextUtils.isEmpty(editText.getText().toString())) {
//                            ipAddress = editText.getText().toString();
//                        }
//                    }
//                })
//                .create()
//                .show();
//    }
}
