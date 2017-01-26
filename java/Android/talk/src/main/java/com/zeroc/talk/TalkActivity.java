// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

package com.zeroc.talk;

import java.lang.ref.WeakReference;
import java.util.LinkedList;
import java.util.List;

import android.app.ActionBar;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.inputmethod.EditorInfo;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import com.zeroc.talk.service.TalkService;
import com.zeroc.talk.service.Service;

public class TalkActivity extends Activity
{
    private static final int MAX_MESSAGE_SIZE = 1024;

    private static final int REQUEST_CONNECT_DEVICE = 1;
    private static final int REQUEST_CONNECT_DEVICE_SSL = 2;
    private static final int REQUEST_ENABLE_BT = 3;

    private EditText _text;
    private ArrayAdapter<String> _adapter;
    private Service _service;
    private Intent _serviceIntent;
    private LinkedList<String> _strings = new LinkedList<String>();

    //
    // We use a static class and a weak reference to avoid lint warnings about leaks.
    //
    private static class HandlerI extends Handler
    {
        HandlerI(WeakReference<TalkActivity> activity)
        {
            _activity = activity;
        }

        @Override
        public void handleMessage(Message m)
        {
            switch(m.what)
            {
            case Service.MESSAGE_STATE_CHANGE:
                switch(m.arg1)
                {
                case Service.STATE_NOT_CONNECTED:
                    _activity.get().setStatus(R.string.not_connected);
                    break;
                case Service.STATE_CONNECTING:
                    _activity.get().setStatus(R.string.connecting);
                    break;
                case Service.STATE_CONNECTED:
                    _activity.get().setStatus(R.string.connected);
                    break;
                }
                break;
            case Service.MESSAGE_RECEIVED:
            case Service.MESSAGE_SENT:
            case Service.MESSAGE_LOG:
                _activity.get().addMessage(m.getData().getString(Service.KEY_TEXT));
                break;
            case Service.MESSAGE_TOAST:
                Toast.makeText(_activity.get(), m.getData().getString(Service.KEY_TEXT), Toast.LENGTH_SHORT).show();
                break;
            }
        }

        private WeakReference<TalkActivity> _activity;
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
            serviceConnected(((com.zeroc.talk.service.TalkService.LocalBinder)service).getService());
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
        // Start the TalkService.
        //
        _serviceIntent = new Intent(TalkActivity.this, TalkService.class);
        startService(_serviceIntent);
    }

    @Override
    public void onStart()
    {
        super.onStart();

        //
        // Enable Bluetooth if necessary.
        //
        if(!BluetoothAdapter.getDefaultAdapter().isEnabled())
        {
            Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableIntent, REQUEST_ENABLE_BT);
        }
        else if(_text == null)
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
            Intent intent = new Intent(this, DeviceActivity.class);
            startActivityForResult(intent, REQUEST_CONNECT_DEVICE);
            return true;
        }

        case R.id.connectSSL:
        {
            Intent intent = new Intent(this, DeviceActivity.class);
            startActivityForResult(intent, REQUEST_CONNECT_DEVICE_SSL);
            return true;
        }

        case R.id.disconnect:
        {
            disconnect();
            return true;
        }

        case R.id.discoverable:
        {
            if(BluetoothAdapter.getDefaultAdapter().getScanMode() !=
                BluetoothAdapter.SCAN_MODE_CONNECTABLE_DISCOVERABLE)
            {
                Intent intent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
                intent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 300);
                startActivity(intent);
            }
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
        case REQUEST_CONNECT_DEVICE:
            if(res == Activity.RESULT_OK)
            {
                connect(data, false);
            }
            break;
        case REQUEST_CONNECT_DEVICE_SSL:
            if(res == Activity.RESULT_OK)
            {
                connect(data, true);
            }
            break;
        case REQUEST_ENABLE_BT:
            if(res == Activity.RESULT_OK)
            {
                if(_text == null)
                {
                    setup();
                }
            }
            else
            {
                Toast.makeText(this, R.string.no_bluetooth, Toast.LENGTH_SHORT).show();
                finish();
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

        _text.requestFocus();

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
        // Bluetooth is already enabled so we can initialize the service.
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
        _text.setText("");

        _service.send(t);
    }

    private void connect(Intent data, boolean ssl)
    {
        //
        // Extract the address and name of the peer from the result intent returned by DeviceActivity.
        //
        String address = data.getExtras().getString(DeviceActivity.EXTRA_DEVICE_ADDRESS);
        String name = data.getExtras().getString(DeviceActivity.EXTRA_DEVICE_NAME);
        _service.connect(address, name, ssl);
    }

    private void disconnect()
    {
        _service.disconnect();
    }

    synchronized private void serviceConnected(Service service)
    {
        _service = service;
        _service.setHandler(new HandlerI(new WeakReference<TalkActivity>(this)));
        if(BluetoothAdapter.getDefaultAdapter().isEnabled())
        {
            _service.initialize();
        }
    }

    synchronized private void serviceDisconnected()
    {
        _service = null;
    }
}
