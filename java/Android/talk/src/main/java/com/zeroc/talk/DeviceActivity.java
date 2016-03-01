// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

package com.zeroc.talk;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;

import java.util.Set;

public class DeviceActivity extends Activity
{
    public static String EXTRA_DEVICE_ADDRESS = "device_address";
    public static String EXTRA_DEVICE_NAME = "device_name";

    private BluetoothAdapter _adapter;
    private ArrayAdapter<DeviceInfo> _devices;
    private Button _scanButton;

    //
    // DeviceInfo encapsulates the details about each Bluetooth device that we discover.
    //
    static class DeviceInfo
    {
        DeviceInfo(BluetoothDevice device)
        {
            _device = device;
            _name = device.getName();
            _address = device.getAddress();
        }

        //
        // Constructor for placeholder text (e.g., "No devices found")
        //
        DeviceInfo(String text)
        {
            _text = text;
        }

        @Override
        public String toString()
        {
            if(_text != null)
            {
                return _text;
            }

            StringBuffer s = new StringBuffer();
            s.append(_name != null ? _name : "Unknown device");
            if(_device.getBondState() == BluetoothDevice.BOND_BONDED)
            {
                s.append(" (paired)");
            }
            s.append("\n");
            s.append(_address);
            return s.toString();
        }

        @Override
        public boolean equals(Object o)
        {
            if(!(o instanceof DeviceInfo) || _text != null)
            {
                return false;
            }

            DeviceInfo di = (DeviceInfo)o;
            return getAddress().equals(di.getAddress());
        }

        boolean hasDevice()
        {
            return _device != null;
        }

        String getAddress()
        {
            return _address;
        }

        String getName()
        {
            return _name;
        }

        private BluetoothDevice _device;
        private String _name;
        private String _address;
        private String _text;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        requestWindowFeature(Window.FEATURE_INDETERMINATE_PROGRESS);
        setContentView(R.layout.device);

        _scanButton = (Button)findViewById(R.id.scan);
        _scanButton.setOnClickListener(
            new View.OnClickListener()
            {
                public void onClick(View v)
                {
                    runDiscovery();
                    v.setEnabled(false);
                }
            });

        _devices = new ArrayAdapter<DeviceInfo>(this, R.layout.device_name);

        ListView list = (ListView)findViewById(R.id.devices);
        list.setAdapter(_devices);
        list.setOnItemClickListener(_deviceListener);

        //
        // Listen for discovery of new Bluetooth devices.
        //
        IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
        this.registerReceiver(_receiver, filter);

        //
        // Listen for discovery completion.
        //
        filter = new IntentFilter(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
        this.registerReceiver(_receiver, filter);

        _adapter = BluetoothAdapter.getDefaultAdapter();

        //
        // Add the current list of paired devices.
        //
        Set<BluetoothDevice> paired = _adapter.getBondedDevices();
        if(paired.size() > 0)
        {
            for(BluetoothDevice device : paired)
            {
                _devices.add(new DeviceInfo(device));
            }
        }
        else
        {
            //
            // Add an entry indicating that there are no paired devices.
            //
            _devices.add(new DeviceInfo(getResources().getText(R.string.none_paired).toString()));
        }
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();

        if(_adapter != null)
        {
            _adapter.cancelDiscovery();
        }

        this.unregisterReceiver(_receiver);
    }

    private void runDiscovery()
    {
        setProgressBarIndeterminateVisibility(true);
        setTitle(R.string.scanning);

        if(_adapter.isDiscovering())
        {
            _adapter.cancelDiscovery();
        }

        _adapter.startDiscovery();
    }

    private AdapterView.OnItemClickListener _deviceListener = new AdapterView.OnItemClickListener()
    {
        public void onItemClick(AdapterView<?> av, View v, int pos, long id)
        {
            DeviceInfo info = _devices.getItem(pos);

            if(!info.hasDevice())
            {
                return;
            }

            _adapter.cancelDiscovery();

            //
            // The address of the selected device becomes the result of this activity.
            //
            Intent intent = new Intent();
            intent.putExtra(EXTRA_DEVICE_ADDRESS, info.getAddress());

            //
            // Also return the device's name (if any).
            //
            String name = info.getName();
            if(name != null)
            {
                intent.putExtra(EXTRA_DEVICE_NAME, name);
            }

            setResult(Activity.RESULT_OK, intent);

            //
            // We're done.
            //
            finish();
        }
    };

    private final BroadcastReceiver _receiver = new BroadcastReceiver()
    {
        @Override
        public void onReceive(Context context, Intent intent)
        {
            String action = intent.getAction();

            if(BluetoothDevice.ACTION_FOUND.equals(action))
            {
                //
                // Remove the placeholder message (if any).
                //
                if(_devices.getCount() == 1)
                {
                    DeviceInfo info = _devices.getItem(0);
                    if(!info.hasDevice())
                    {
                        _devices.clear();
                    }
                }

                //
                // Add a device if it's not already in the list.
                //
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                DeviceInfo info = new DeviceInfo(device);
                int pos = _devices.getPosition(info);
                if(pos < 0)
                {
                    _devices.add(new DeviceInfo(device));
                }
                else
                {
                    //
                    // The device is already present but we force a refresh in case its state
                    // has changed.
                    //
                    _devices.notifyDataSetChanged();
                }
            }
            else if(BluetoothAdapter.ACTION_DISCOVERY_FINISHED.equals(action))
            {
                setProgressBarIndeterminateVisibility(false);
                setTitle(R.string.choose_peer);

                if(_devices.getCount() == 1)
                {
                    DeviceInfo info = _devices.getItem(0);
                    if(!info.hasDevice())
                    {
                        _devices.clear();
                    }
                }

                //
                // Add a placeholder entry.
                //
                if(_devices.getCount() == 0)
                {
                    _devices.add(new DeviceInfo(getResources().getText(R.string.none_found).toString()));
                }

                _scanButton.setEnabled(true);
            }
        }
    };
}
