// Copyright (c) ZeroC, Inc.

package com.zeroc.hello;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.view.WindowManager;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.Spinner;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.DialogFragment;

import com.zeroc.Ice.LocalException;

import java.util.concurrent.ExecutorService;

public class HelloWorld extends AppCompatActivity
{
    public static class ErrorDialogFragment extends DialogFragment
    {
        public static ErrorDialogFragment newInstance(String message, boolean fatal)
        {
            ErrorDialogFragment frag = new ErrorDialogFragment();
            Bundle args = new Bundle();
            args.putString("message", message);
            args.putBoolean("fatal", fatal);
            frag.setArguments(args);
            return frag;
        }

        @NonNull
        @Override
        public Dialog onCreateDialog(Bundle savedInstanceState)
        {
            AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
            builder.setTitle("Error")
                   .setMessage(requireArguments().getString("message"))
                   .setPositiveButton("Ok", (dialog, whichButton) -> {
                       if(requireArguments().getBoolean("fatal"))
                       {
                           requireActivity().finish();
                       }
                   });
            return builder.create();
        }
    }

    // These two arrays match.
    private final static DeliveryMode[] DELIVERY_MODES =
    {
        DeliveryMode.TWOWAY,
        DeliveryMode.TWOWAY_SECURE,
        DeliveryMode.ONEWAY,
        DeliveryMode.ONEWAY_BATCH,
        DeliveryMode.ONEWAY_SECURE,
        DeliveryMode.ONEWAY_SECURE_BATCH,
        DeliveryMode.DATAGRAM,
        DeliveryMode.DATAGRAM_BATCH,
    };

    private final static String[] DELIVERY_MODE_DESC = new String[]
    {
            "Twoway",
            "Twoway Secure",
            "Oneway",
            "Oneway Batch",
            "Oneway Secure",
            "Oneway Secure Batch",
            "Datagram",
            "Datagram Batch"
    };

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        final Button sayHelloButton = findViewById(R.id.sayHello);
        final SeekBar delaySeekBar = findViewById(R.id.delay);
        final ProgressBar activityProgressBar = findViewById(R.id.activity);
        final Button shutdownButton = findViewById(R.id.shutdown);
        final EditText hostEditText = findViewById(R.id.host);
        final CheckBox useDiscoveryCheckBox = findViewById(R.id.useDiscovery);
        final Button flushButton = findViewById(R.id.flush);
        final TextView statusTextView = findViewById(R.id.status);
        final Spinner modeSpinner = findViewById(R.id.mode);
        final TextView delayTextView = findViewById(R.id.delayView);
        final TextView timeoutTextView = findViewById(R.id.timeoutView);
        final SeekBar timeoutSeekBar = findViewById(R.id.timeout);
        final SharedPreferences prefs = getPreferences(MODE_PRIVATE);

        _app = (HelloApp)getApplication();
        _executor = java.util.concurrent.Executors.newSingleThreadExecutor();

        useDiscoveryCheckBox.setOnClickListener(v -> {
            final boolean checked = ((CheckBox)v).isChecked();

            _app.setUseDiscovery(checked);

            if(checked)
            {
                hostEditText.setEnabled(false);
                hostEditText.setText("");
                sayHelloButton.setEnabled(true);
                shutdownButton.setEnabled(true);
            }
            else
            {
                hostEditText.setEnabled(true);
                hostEditText.setText(prefs.getString(HOSTNAME_KEY, DEFAULT_HOST));
            }
        });

        sayHelloButton.setOnClickListener(v -> {
            if(_app.getDeliveryMode().isBatch())
            {
                flushButton.setEnabled(true);
                _app.sayHello(delaySeekBar.getProgress());
                statusTextView.setText(R.string.queued_hello_request);

            }
            else
            {
                int delay = delaySeekBar.getProgress();
                // sayHelloAsync is called on the executor to avoid IO operations on the main thread.
                _executor.execute(() -> {
                    _app.sayHelloAsync(delay);
                });
            }
        });

        shutdownButton.setOnClickListener(v -> {
            if(_app.getDeliveryMode().isBatch())
            {
                flushButton.setEnabled(true);
                _app.shutdown();
                statusTextView.setText(R.string.queued_shutdown_request);
            }
            else
            {
                // shutdownAsync is called on the executor to avoid IO operations on the main thread.
                _executor.execute(() -> {
                    _app.shutdownAsync();
                });
            }
        });

        hostEditText.addTextChangedListener(new TextWatcher()
        {
            @Override
            public void afterTextChanged(Editable s)
            {
                if(!useDiscoveryCheckBox.isChecked())
                {
                    String host = hostEditText.getText().toString().trim();
                    if(host.isEmpty())
                    {
                        sayHelloButton.setEnabled(false);
                        shutdownButton.setEnabled(false);
                    }
                    else
                    {
                        sayHelloButton.setEnabled(true);
                        shutdownButton.setEnabled(true);
                    }

                    _app.setHost(host);

                    // Change the preferences if necessary.
                    if(!prefs.getString(HOSTNAME_KEY, DEFAULT_HOST).equals(host))
                    {
                        SharedPreferences.Editor edit = prefs.edit();
                        edit.putString(HOSTNAME_KEY, host);
                        edit.apply();
                    }
                }
            }

            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after)
            {
            }

            @Override
            public void onTextChanged(CharSequence s, int start, int count, int after)
            {
            }
        });

        flushButton.setOnClickListener(v -> {
            // flush is called on the executor to avoid IO operations on the main thread.
            _executor.execute(() -> _app.flush());
            flushButton.setEnabled(false);
            statusTextView.setText(R.string.flushed_batch_requests);
        });

        ArrayAdapter<String> modeAdapter =
                new ArrayAdapter<>(this, android.R.layout.simple_spinner_item, DELIVERY_MODE_DESC);
        modeSpinner.setAdapter(modeAdapter);
        modeSpinner.setOnItemSelectedListener(new android.widget.AdapterView.OnItemSelectedListener()
        {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id)
            {
                _app.setDeliveryMode(DELIVERY_MODES[(int) id]);
            }

            @Override
            public void onNothingSelected(AdapterView<?> arg0)
            {
            }
        });
        modeSpinner.setSelection(0);
        _app.setDeliveryMode(DELIVERY_MODES[(int)modeSpinner.getSelectedItemId()]);

        delaySeekBar.setOnSeekBarChangeListener(new OnSeekBarChangeListener()
        {
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromThumb)
            {
                delayTextView.setText(String.format(java.util.Locale.getDefault(), "%.1f", progress / 1000.0));
            }

            public void onStartTrackingTouch(SeekBar seekBar)
            {
            }

            public void onStopTrackingTouch(SeekBar seekBar)
            {
            }
        });

        timeoutSeekBar.setOnSeekBarChangeListener(new OnSeekBarChangeListener()
        {
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromThumb)
            {
                timeoutTextView.setText(String.format(java.util.Locale.getDefault(), "%.1f", progress / 1000.0));
                _app.setTimeout(progress);
            }

            public void onStartTrackingTouch(SeekBar seekBar)
            {
            }

            public void onStopTrackingTouch(SeekBar seekBar)
            {
            }
        });

        activityProgressBar.setVisibility(View.GONE);

        // Setup the defaults.
        hostEditText.setText(prefs.getString(HOSTNAME_KEY, DEFAULT_HOST));
        flushButton.setEnabled(false);

        statusTextView.setText(R.string.ready);

        _handler = new Handler(Looper.getMainLooper()) {
            @Override
            public void handleMessage(Message m)
            {
                switch(m.what)
                {
                    case HelloApp.MSG_WAIT:
                    {
                        getWindow().setFlags(WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE,
                                WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE);
                        findViewById(R.id.progressbar).setVisibility(View.VISIBLE);
                        break;
                    }

                    case HelloApp.MSG_READY:
                    {
                        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE);
                        findViewById(R.id.progressbar).setVisibility(View.INVISIBLE);

                        HelloApp.MessageReady ready = (HelloApp.MessageReady)m.obj;
                        if(ready.ex != null)
                        {
                            LocalException ex = ready.ex;
                            DialogFragment dialog = ErrorDialogFragment.newInstance(ex.toString(), true);
                            dialog.show(getSupportFragmentManager(), ERROR_TAG);
                        }
                        statusTextView.setText(R.string.ready);
                        break;
                    }

                    case HelloApp.MSG_EXCEPTION:
                    {
                        statusTextView.setText(R.string.ready);
                        activityProgressBar.setVisibility(View.GONE);

                        LocalException ex = (LocalException)m.obj;
                        DialogFragment dialog = ErrorDialogFragment.newInstance(ex.toString(), false);
                        dialog.show(getSupportFragmentManager(), ERROR_TAG);
                        break;
                    }

                    case HelloApp.MSG_RESPONSE:
                    {
                        activityProgressBar.setVisibility(View.GONE);
                        statusTextView.setText(R.string.ready);
                        break;
                    }
                    case HelloApp.MSG_SENT:
                    {
                        activityProgressBar.setVisibility(View.VISIBLE);
                        statusTextView.setText(R.string.waiting_response);
                        break;
                    }

                    case HelloApp.MSG_SENDING:
                    {
                        activityProgressBar.setVisibility(View.VISIBLE);
                        statusTextView.setText(R.string.sending_request);
                        break;
                    }
                }
            }
        };
    }

    @Override
    protected void onResume()
    {
        super.onResume();
        _app.setHandler(_handler);
    }

    @Override
    protected void onRestoreInstanceState(Bundle savedInstanceState)
    {
        super.onRestoreInstanceState(savedInstanceState);

        final CheckBox useDiscoveryCheckBox = findViewById(R.id.useDiscovery);
        final EditText hostEditText = findViewById(R.id.host);
        final SeekBar delaySeekBar = findViewById(R.id.delay);
        final Button sayHelloButton = findViewById(R.id.sayHello);
        final Button shutdownButton = findViewById(R.id.shutdown);
        final Button flushButton = findViewById(R.id.flush);
        final Spinner modeSpinner = findViewById(R.id.mode);
        final SeekBar timeoutSeekBar = findViewById(R.id.timeout);
        final TextView statusTextView = findViewById(R.id.status);
        final ProgressBar activityProgressBar = findViewById(R.id.activity);

        modeSpinner.setSelection(savedInstanceState.getInt(BUNDLE_KEY_MODE));
        flushButton.setEnabled(savedInstanceState.getBoolean(BUNDLE_KEY_FLUSH_ENABLED));
        delaySeekBar.setProgress(savedInstanceState.getInt(BUNDLE_KEY_DELAY));
        timeoutSeekBar.setProgress(savedInstanceState.getInt(BUNDLE_KEY_TIMEOUT));
        statusTextView.setText(savedInstanceState.getString(BUNDLE_KEY_STATUS));
        activityProgressBar.setVisibility(
                savedInstanceState.getBoolean(BUNDLE_KEY_PROGRESS) ? View.VISIBLE : View.GONE);

        if(useDiscoveryCheckBox.isChecked())
        {
            hostEditText.setEnabled(false);
            hostEditText.setText("");
            sayHelloButton.setEnabled(true);
            shutdownButton.setEnabled(true);
        }
    }

    @Override
    protected void onSaveInstanceState(@NonNull Bundle outState)
    {
        super.onSaveInstanceState(outState);

        final SeekBar delaySeekBar = findViewById(R.id.delay);
        final Button flushButton = findViewById(R.id.flush);
        final Spinner modeSpinner = findViewById(R.id.mode);
        final SeekBar timeoutSeekBar = findViewById(R.id.timeout);
        final TextView statusTextView = findViewById(R.id.status);
        final ProgressBar activityProgressBar = findViewById(R.id.activity);

        outState.putInt(BUNDLE_KEY_MODE, (int)modeSpinner.getSelectedItemId());
        outState.putInt(BUNDLE_KEY_DELAY, delaySeekBar.getProgress());
        outState.putInt(BUNDLE_KEY_TIMEOUT, timeoutSeekBar.getProgress());
        outState.putBoolean(BUNDLE_KEY_FLUSH_ENABLED, flushButton.isEnabled());
        outState.putString(BUNDLE_KEY_STATUS, statusTextView.getText().toString());
        outState.putBoolean(BUNDLE_KEY_PROGRESS, activityProgressBar.getVisibility() == View.VISIBLE);

        // Clear the application handler. We don't want any further messages while
        // in the background.
        _app.setHandler(null);
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();
        _app.setHandler(null);
    }

    public static final String ERROR_TAG = "error";

    private static final String DEFAULT_HOST = "10.0.2.2";
    private static final String HOSTNAME_KEY = "host";

    private static final String BUNDLE_KEY_PROGRESS = "zeroc:progress";
    private static final String BUNDLE_KEY_STATUS = "zeroc:status";
    private static final String BUNDLE_KEY_MODE = "zeroc:mode";
    private static final String BUNDLE_KEY_TIMEOUT = "zeroc:timeout";
    private static final String BUNDLE_KEY_DELAY = "zeroc:delay";
    private static final String BUNDLE_KEY_FLUSH_ENABLED = "zeroc:flush";

    private HelloApp _app;
    private ExecutorService _executor;
    private Handler _handler;
}
