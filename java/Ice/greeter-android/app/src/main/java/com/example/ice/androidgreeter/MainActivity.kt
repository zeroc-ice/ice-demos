package com.example.ice.androidgreeter

import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import android.widget.TextView
import android.widget.Toast
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import androidx.lifecycle.lifecycleScope
import com.example.visitorcenter.GreeterPrx
import com.zeroc.Ice.Communicator
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.future.await
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

/**
 * Main activity for the Ice Greeter Android demo.
 *
 * Provides a simple UI to send a greeting request to a remote Ice server using user-provided IP and name inputs.
 */
class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        // UI initialization
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.activity_main)
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main)) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }

        // UI element references
        val ipInput = findViewById<EditText>(R.id.ipAddressInput)
        val nameInput = findViewById<EditText>(R.id.nameInput)
        val greetButton = findViewById<Button>(R.id.greetButton)
        val greetResponse = findViewById<TextView>(R.id.greetResponse)

        // Retrieve the Ice communicator from the application class
        val communicator = (application as App).getCommunicator()

        // Handle the greet button click
        greetButton.setOnClickListener {
            val ip = ipInput.text.toString().trim()
            val name = nameInput.text.toString().trim()

            // Disable the button to prevent repeated clicks during the request
            greetButton.isEnabled = false

            // Launch a coroutine tied to the activity's lifecycle
            lifecycleScope.launch {
                try {
                    // Create a proxy to the remote Greeter object.
                    // The proxy is recreated for each request to ensure the latest IP address is used.
                    val greeter = GreeterPrx.createProxy(communicator, "greeter:tcp -h $ip -p 4061")

                    // Send the greetAsync request on a background thread and wait for the response
                    val greeting = withContext(Dispatchers.IO) {
                        greeter.greetAsync(name).await()
                    }

                    // Display the result on the UI thread
                    greetResponse.text = greeting
                } catch (e: Exception) {
                    // Show the error message on the UI
                    Toast.makeText(
                        this@MainActivity,
                        "Failed to send greet request: ${e.cause?.message ?: e.message}",
                        Toast.LENGTH_LONG
                    ).show()
                } finally {
                    // Re-enable the button after the request completes
                    greetButton.isEnabled = true
                }
            }
        }
    }
}
