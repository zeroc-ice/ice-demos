//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

package com.zeroc.mtalk;

import java.lang.ref.WeakReference;
import java.util.LinkedList;

import android.app.ActionBar;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import com.zeroc.mtalk.service.MTalkService;
import com.zeroc.mtalk.service.Service;

public class MainActivity extends Activity
{
    private static final int MAX_MESSAGE_SIZE = 1024;

    private static final int REQUEST_CONNECT = 1;

    private EditText _text;
    private EditText _name;
    private ArrayAdapter<String> _adapter;
    private Service _service;
    private Intent _serviceIntent;
    private LinkedList<String> _strings = new LinkedList<String>();

    //
    // We use a static class and a weak reference to avoid lint warnings about leaks.
    //
    private static class HandlerI extends Handler
    {
        HandlerI(WeakReference<MainActivity> activity)
        {
            _activity = activity;
        }

        @Override
        public void handleMessage(Message m)
        {
            switch(m.what)
            {
                case Service.MESSAGE_RECEIVED:
                case Service.MESSAGE_SENT:
                case Service.MESSAGE_LOG:
                {
                    _activity.get().addMessage(m.getData().getString(Service.KEY_TEXT));
                    break;
                }
                case Service.MESSAGE_TOAST:
                {
                    Toast.makeText(_activity.get(), m.getData().getString(Service.KEY_TEXT), Toast.LENGTH_SHORT).show();
                    break;
                }
            }
        }

        private WeakReference<MainActivity> _activity;
    }

    final private ServiceConnection _connection = new ServiceConnection()
    {
        public void onServiceConnected(ComponentName name, IBinder service)
        {
            //
            // This is called when the connection with the service has been
            // established, giving us the service object we can use to
            // interact with the service. Because we have bound to a explicit
            // service that we know is running in our own process, we can
            // cast its IBinder to a concrete class and directly access it.
            //
            serviceConnected(((com.zeroc.mtalk.service.MTalkService.LocalBinder)service).getService());
        }

        public void onServiceDisconnected(ComponentName name)
        {
            serviceDisconnected();
        }
    };

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        //
        // Listen for network status updates from the service.
        //
        IntentFilter filter = new IntentFilter();
        filter.addAction(Intents.ACTION_NETWORK_UP);
        filter.addAction(Intents.ACTION_NETWORK_DOWN);
        filter.addAction(Intents.ACTION_PEER_STATUS);
        this.registerReceiver(_receiver, filter);

        //
        // Start the MTalkService.
        //
        _serviceIntent = new Intent(MainActivity.this, MTalkService.class);
        startService(_serviceIntent);

        //
        // Obtain the user's name.
        //
        _name = new EditText(this);
        _name.setText(Build.MODEL);
        _name.setSelectAllOnFocus(true);
        _name.setSingleLine(true);

        new AlertDialog.Builder(this)
            .setTitle(R.string.name_title)
            .setMessage(R.string.enter_name)
            .setView(_name)
            .setPositiveButton(R.string.name_ok, new DialogInterface.OnClickListener()
                {
                    public void onClick(DialogInterface dialog, int whichButton)
                    {
                        setName();
                    }
                }
            )
            .show();
    }

    @Override
    public void onStart()
    {
        super.onStart();

        if(_text == null)
        {
            //
            // Only need to call setup() once.
            //
            setup();
        }
    }

    @Override
    public void onResume()
    {
        super.onResume();
        bindService(_serviceIntent, _connection, BIND_AUTO_CREATE);
    }

    @Override
    public void onStop()
    {
        super.onStop();
        unbindService(_connection);
    }

    @Override
    public void onDestroy()
    {
        super.onDestroy();
        this.unregisterReceiver(_receiver);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu)
    {
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item)
    {
        switch (item.getItemId())
        {
        case R.id.connect:
        {
            //
            // Ignore if WiFi is disabled.
            //
            if(!_service.isNetworkAvailable())
            {
                Toast.makeText(this, "WiFi is not enabled", Toast.LENGTH_SHORT).show();
                return true;
            }

            Intent intent = new Intent(this, ConnectActivity.class);
            startActivityForResult(intent, REQUEST_CONNECT);
            return true;
        }

        case R.id.disconnect:
        {
            disconnect();
            return true;
        }

        case R.id.clear:
        {
            _adapter.clear();
            return true;
        }
        }

        return false;
    }

    @Override
    protected void onActivityResult(int req, int res, Intent data)
    {
        switch(req)
        {
        case REQUEST_CONNECT:
            if(res == Activity.RESULT_OK)
            {
                //
                // Extract the name and proxy of the peer from the result intent returned by ConnectActivity.
                //
                String name = data.getExtras().getString(ConnectActivity.EXTRA_NAME);
                String proxy = data.getExtras().getString(ConnectActivity.EXTRA_PROXY);
                _service.connect(name, proxy);
            }
            break;
        }
    }

    synchronized private void setup()
    {
        _text = (EditText)findViewById(R.id.text);

        ListView transcript = (ListView)findViewById(R.id.list);
        _adapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, _strings);
        transcript.setAdapter(_adapter);

        _text.setOnEditorActionListener(new TextView.OnEditorActionListener()
        {
            @Override
            public boolean onEditorAction(TextView v, int actionId, KeyEvent event)
            {
                if(actionId == EditorInfo.IME_ACTION_DONE)
                {
                    sendText();
                    return true;
                }
                return false;
            }
        });

        Button sendButton = (Button)findViewById(R.id.send);
        sendButton.setOnClickListener(new View.OnClickListener()
        {
            public void onClick(View v)
            {
                sendText();
            }
        });

        setStatus(R.string.not_connected);

        //
        // Initialize the service.
        //
        if(_service != null)
        {
            _service.initialize();
        }
    }

    private void setStatus(int res)
    {
        ActionBar b = getActionBar();
        if(b != null)
        {
            b.setSubtitle(res);
        }
    }

    private void addMessage(String message)
    {
        if(_strings.size() > 200)
        {
            _strings.removeFirst();
        }
        _strings.add(message);
        _adapter.notifyDataSetChanged();
    }

    private void sendText()
    {
        String t = _text.getText().toString().trim();
        if(t.length() == 0)
        {
            return;
        }

        if(t.length() > MAX_MESSAGE_SIZE)
        {
            final String msg = "Message length exceeded, maximum length is " + MAX_MESSAGE_SIZE + " characters.";
            Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
            return;
        }

        //
        // Ignore the Send button if WiFi is disabled.
        //
        if(!_service.isNetworkAvailable())
        {
            Toast.makeText(this, "WiFi is not enabled", Toast.LENGTH_SHORT).show();
            return;
        }

        if(_service.send(t))
        {
            _text.setText("");
        }
    }

    private void disconnect()
    {
        _service.disconnect();
    }

    synchronized private void serviceConnected(Service service)
    {
        _service = service;
        _service.setHandler(new HandlerI(new WeakReference<MainActivity>(this)));
        _service.initialize();
        setName();
    }

    synchronized private void serviceDisconnected()
    {
        _service = null;
    }

    synchronized private void setName()
    {
        if(_service != null)
        {
            String text = _name.getText().toString().trim();
            if(text.length() > 0)
            {
                _service.setName(text);
            }
        }
    }

    private final BroadcastReceiver _receiver = new BroadcastReceiver()
    {
        @Override
        public void onReceive(Context context, Intent intent)
        {
            String action = intent.getAction();

            if(Intents.ACTION_NETWORK_UP.equals(action))
            {
                Toast.makeText(MainActivity.this, "Network is up", Toast.LENGTH_SHORT).show();
            }
            else if(Intents.ACTION_NETWORK_DOWN.equals(action))
            {
                Toast.makeText(MainActivity.this, "Network is down", Toast.LENGTH_SHORT).show();
            }
            else if(Intents.ACTION_PEER_STATUS.equals(action))
            {
                String status = intent.getStringExtra(Intents.EXTRA_PEER_STATUS);
                if(Intents.PEER_NOT_CONNECTED.equals(status))
                {
                    MainActivity.this.setStatus(R.string.not_connected);
                }
                else if(Intents.PEER_CONNECTING.equals(status))
                {
                    MainActivity.this.setStatus(R.string.connecting);
                }
                else
                {
                    MainActivity.this.setStatus(R.string.connected);
                }
            }
        }
    };
}
