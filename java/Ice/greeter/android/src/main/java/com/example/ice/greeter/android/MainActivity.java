// Copyright (c) ZeroC, Inc.

package com.example.ice.greeter.android;

import android.os.Bundle;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;
import com.example.visitorcenter.GreeterPrx;
import com.zeroc.Ice.Communicator;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;

public class MainActivity extends AppCompatActivity {

    private EditText editTextIpAddress;
    private EditText editTextName;
    private Button buttonGreet;
    private TextView textViewResponse;
    private Communicator communicator;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Get Ice communicator from application
        GreeterApplication app = (GreeterApplication) getApplication();
        communicator = app.getCommunicator();

        // Initialize UI components
        editTextIpAddress = findViewById(R.id.editTextIpAddress);
        editTextName = findViewById(R.id.editTextName);
        buttonGreet = findViewById(R.id.buttonGreet);
        textViewResponse = findViewById(R.id.textViewResponse);

        // Set up button click listener
        buttonGreet.setOnClickListener(v -> sendGreeting());
    }

    private void sendGreeting() {
        String ipAddress = editTextIpAddress.getText().toString().trim();
        String name = editTextName.getText().toString().trim();

        if (ipAddress.isEmpty()) {
            Toast.makeText(this, "Please enter an IP address", Toast.LENGTH_SHORT).show();
            return;
        }

        if (name.isEmpty()) {
            Toast.makeText(this, "Please enter a name", Toast.LENGTH_SHORT).show();
            return;
        }

        // Disable button during request
        buttonGreet.setEnabled(false);
        textViewResponse.setText("Sending greeting request...");

        try {
            // Create proxy with the specified IP address
            String proxyString = "greeter:tcp -h " + ipAddress + " -p 4061";
            GreeterPrx greeter = GreeterPrx.createProxy(communicator, proxyString);

            // Send async greeting request
            CompletableFuture<String> futureGreeting = greeter.greetAsync(name);

            // Handle the response asynchronously
            futureGreeting.whenComplete((greeting, throwable) -> {
                runOnUiThread(() -> {
                    buttonGreet.setEnabled(true);
                    if (throwable != null) {
                        // Handle error
                        String errorMessage = "Error: " + throwable.getMessage();
                        textViewResponse.setText(errorMessage);
                        Toast.makeText(MainActivity.this, getString(R.string.error_request), Toast.LENGTH_LONG).show();
                    } else {
                        // Display successful response
                        textViewResponse.setText(greeting);
                    }
                });
            });

        } catch (Exception e) {
            // Handle proxy creation error
            buttonGreet.setEnabled(true);
            String errorMessage = "Connection error: " + e.getMessage();
            textViewResponse.setText(errorMessage);
            Toast.makeText(this, getString(R.string.error_connection), Toast.LENGTH_LONG).show();
        }
    }
}