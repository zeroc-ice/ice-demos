package com.example.iceandroidgreeter

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
import com.zeroc.Ice.Communicator
import com.zeroc.Ice.Util
import com.example.visitorcenter.GreeterPrx
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.future.await
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

class MainActivity : AppCompatActivity() {
    private var communicator: Communicator? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        // Optional visual enhancement for edge-to-edge layout (can be removed if unused)
        enableEdgeToEdge()
        setContentView(R.layout.activity_main)

        // Apply padding for system bars to avoid layout overlap
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main)) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }

        // UI references
        val ipInput = findViewById<EditText>(R.id.ipAddressInput)
        val nameInput = findViewById<EditText>(R.id.nameInput)
        val greetButton = findViewById<Button>(R.id.greetButton)
        val greetResponse = findViewById<TextView>(R.id.greetResponse)

        lifecycleScope.launch(Dispatchers.IO) {
            try {
                communicator = Util.initialize()
            } catch (e: Exception) {
                withContext(Dispatchers.Main) {
                    Toast.makeText(this@MainActivity, "Failed to initialize Ice: ${e.message}", Toast.LENGTH_LONG).show()
                    greetButton.isEnabled = false
                }
            }
        }

        greetButton.setOnClickListener {
            val ip = ipInput.text.toString().trim()
            val name = nameInput.text.toString().trim()

            // Disable the button to prevent repeated clicks during the request
            greetButton.isEnabled = false

            // Launch a coroutine tied to the Activity lifecycle
            lifecycleScope.launch {
                try {
                    // Create proxy to the remote Greeter object
                    val greeter = GreeterPrx.createProxy(communicator, "greeter:tcp -h $ip -p 4061")

                    // Call greetAsync on a background thread and suspend until response
                    val greeting = withContext(Dispatchers.IO) {
                        greeter.greetAsync(name).await()
                    }

                    // Display the result on the UI thread
                    greetResponse.text = greeting

                } catch (e: Exception) {
                    // Show the error message on the UI
                    Toast.makeText(this@MainActivity, "Failed to send greet request: ${e.message}", Toast.LENGTH_LONG).show()
                } finally {
                    // Re-enable the button after the request completes
                    greetButton.isEnabled = true
                }
            }
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        // Cleanly shut down the communicator on a background thread
        communicator?.let {
            lifecycleScope.launch(Dispatchers.IO) {
                it.destroy()
            }
        }
    }
}
