//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

package com.zeroc.mtalk;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.ConnectivityManager;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;

import java.util.Comparator;

public class ConnectActivity extends Activity
{
    public static String EXTRA_NAME = "peer_name";
    public static String EXTRA_PROXY = "peer_proxy";

    private ArrayAdapter<PeerInfo> _peers;

    //
    // PeerInfo encapsulates the details about each peer that we discover.
    //
    static class PeerInfo
    {
        PeerInfo(String name, String proxy)
        {
            _name = name;
            _proxy = proxy;
        }

        //
        // Constructor for placeholder text (e.g., "No peers found")
        //
        PeerInfo(String text)
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

            return _name;
        }

        @Override
        public boolean equals(Object o)
        {
            if(!(o instanceof PeerInfo) || _text != null)
            {
                return false;
            }

            PeerInfo pi = (PeerInfo)o;
            return _name.equals(pi._name);
        }

        String getName()
        {
            return _name;
        }

        String getProxy()
        {
            return _proxy;
        }

        void setProxy(String proxy)
        {
            _proxy = proxy;
        }

        boolean isValid()
        {
            return _text == null;
        }

        private String _name;
        private String _proxy;
        private String _text;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.connect);

        _peers = new ArrayAdapter<PeerInfo>(this, R.layout.peer_name);

        ListView list = (ListView)findViewById(R.id.peers);
        list.setAdapter(_peers);
        list.setOnItemClickListener(_peerListener);

        //
        // Listen for intents.
        //
        IntentFilter filter = new IntentFilter();
        filter.addAction(Intents.ACTION_FOUND_PEER);
        filter.addAction(Intents.ACTION_PEER_STATUS);
        this.registerReceiver(_receiver, filter);

        //
        // Add an entry indicating that there are no known peers.
        //
        _peers.add(new PeerInfo(getResources().getText(R.string.none_found).toString()));
    }

    @Override
    protected void onResume()
    {
        super.onResume();
        sendBroadcast(new Intent(Intents.ACTION_START_MULTICAST));
    }

    @Override
    protected void onPause()
    {
        super.onPause();
        sendBroadcast(new Intent(Intents.ACTION_STOP_MULTICAST));
    }

    @Override
    protected void onStop()
    {
        super.onStop();
        sendBroadcast(new Intent(Intents.ACTION_STOP_MULTICAST));
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();
        sendBroadcast(new Intent(Intents.ACTION_STOP_MULTICAST));
        this.unregisterReceiver(_receiver);
    }

    private synchronized void discoveredPeer(String name, String proxy)
    {
        _peers.setNotifyOnChange(false);

        boolean changed = false;

        //
        // Remove the placeholder message (if any).
        //
        if(_peers.getCount() == 1)
        {
            PeerInfo info = _peers.getItem(0);
            if(!info.isValid())
            {
                _peers.clear();
                changed = true;
            }
        }

        //
        // Update the list to add the peer if necessary.
        //
        PeerInfo info = new PeerInfo(name, proxy);
        int pos = _peers.getPosition(info);
        if(pos >= 0)
        {
            info = _peers.getItem(pos);
            if(!info.getProxy().equals(proxy))
            {
                info.setProxy(proxy);
            }
        }
        else
        {
            _peers.add(info);
            changed = true;
        }

        if(changed)
        {
            _peers.sort(new Comparator<PeerInfo>()
                {
                    @Override
                    public int compare(PeerInfo p1, PeerInfo p2)
                    {
                        return p1.getName().compareTo(p2.getName());
                    }
                });
            _peers.notifyDataSetChanged();
        }
    }

    private AdapterView.OnItemClickListener _peerListener = new AdapterView.OnItemClickListener()
    {
        public void onItemClick(AdapterView<?> av, View v, int pos, long id)
        {
            PeerInfo info = _peers.getItem(pos);

            if(!info.isValid())
            {
                return;
            }

            //
            // The name and proxy of the selected peer become the result of this activity.
            //
            Intent intent = new Intent();
            intent.putExtra(EXTRA_NAME, info.getName());
            intent.putExtra(EXTRA_PROXY, info.getProxy());

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

            if(Intents.ACTION_FOUND_PEER.equals(action))
            {
                String name = intent.getStringExtra(Intents.EXTRA_PEER_NAME);
                String proxy = intent.getStringExtra(Intents.EXTRA_PEER_PROXY);
                discoveredPeer(name, proxy);
            }
            else if(Intents.ACTION_PEER_STATUS.equals(action))
            {
                String status = intent.getStringExtra(Intents.EXTRA_PEER_STATUS);
                if(Intents.PEER_CONNECTED.equals(status))
                {
                    finish();
                }
            }
        }
    };
}
