package com.apphousebd.arduinoledcontrol;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class WarningActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_warning);

        String channel = getIntent().getStringExtra("channel");

        TextView textView = findViewById(R.id.warningText);
        textView.setText("Looks like your light is turned on in " + channel + ". Please turn it off.");

        Button button = findViewById(R.id.warningOkBtn);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
            }
        });
    }
}
