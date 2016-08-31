// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

package com.zeroc.chat;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.ComponentName;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.IBinder;
import android.text.Editable;
import android.text.TextWatcher;
import android.widget.Button;
import android.widget.EditText;

import com.zeroc.chat.service.ChatService;
import com.zeroc.chat.service.Service;
import com.zeroc.chat.service.SessionListener;

public class LoginActivity extends Activity
{
    private static final String USERNAME_KEY = "username";
    private static final String PASSWORD_KEY = "password";
    public static final String LOGIN_ERROR_TAG = "loginerror";

    private Button _login;
    private EditText _username;
    private EditText _password;
    private SharedPreferences _prefs;

    private boolean _loginInProgress = false;
    private Service _service;
    private Intent _chatServiceIntent;

    public static class LoginErrorDialogFragment extends DialogFragment
    {
        public static LoginErrorDialogFragment newInstance(String message)
        {
            LoginErrorDialogFragment frag = new LoginErrorDialogFragment();
            Bundle args = new Bundle();
            args.putString("message", message);
            frag.setArguments(args);
            return frag;
        }

        @Override
        public Dialog onCreateDialog(Bundle savedInstanceState)
        {
            AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
            builder.setMessage(getArguments().getString("message"))
                   .setPositiveButton("Ok", new DialogInterface.OnClickListener()
                   {
                       public void onClick(DialogInterface dialog, int whichButton)
                       {
                           ((LoginActivity) getActivity()).loginErrorOk();
                       }
                   });
            return builder.create();
        }
    }

    public static class InvalidHostDialogFragment extends DialogFragment
    {
        @Override
        public Dialog onCreateDialog(Bundle savedInstanceState)
        {
            AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
            builder.setTitle("Error")
                   .setMessage("The hostname is invalid.")
                   .setPositiveButton("Ok", new DialogInterface.OnClickListener()
                   {
                       public void onClick(DialogInterface dialog, int whichButton)
                       {
                       }
                   });
            return builder.create();
        }
    }

    private SessionListener _listener = new SessionListener()
    {
        public void onLoginInProgress()
        {
            _loginInProgress = true;
            setLoginState();
        }

        public void onConnectConfirm()
        {
        }

        public void onLogin()
        {
            startActivity(new Intent(LoginActivity.this, ChatActivity.class));
        }

        public void onLoginError()
        {
            setLoginState();
            DialogFragment dialog = LoginErrorDialogFragment.newInstance(
                (_service != null) ? _service.getLoginError() : "");
            dialog.show(getFragmentManager(), LOGIN_ERROR_TAG);
        }
    };

    final private ServiceConnection _connection = new ServiceConnection()
    {
        public void onServiceConnected(ComponentName name, IBinder service)
        {
            // This is called when the connection with the service has been
            // established, giving us the service object we can use to
            // interact with the service. Because we have bound to a explicit
            // service that we know is running in our own process, we can
            // cast its IBinder to a concrete class and directly access it.
            setLoginState();

            _service = ((com.zeroc.chat.service.ChatService.LocalBinder)service).getService();
            _service.setSessionListener(_listener);
        }

        public void onServiceDisconnected(ComponentName name)
        {
        }
    };

    private void loginErrorOk()
    {
        _loginInProgress = false;
        setLoginState();
    }

    private void setLoginState()
    {
        if(_loginInProgress)
        {
            _login.setEnabled(false);
        }
        else
        {
            String username = _username.getText().toString().trim();
            _login.setEnabled(username.length() > 0);
        }
    }

    private void login()
    {
        final String username = _username.getText().toString().trim();
        final String password = _password.getText().toString().trim();

        // Update preferences.
        SharedPreferences.Editor edit = _prefs.edit();
        if(!_prefs.getString(USERNAME_KEY, "").equals(username))
        {
            edit.putString(USERNAME_KEY, username);
        }
        if(!_prefs.getString(PASSWORD_KEY, "").equals(password))
        {
            edit.putString(PASSWORD_KEY, password);
        }
        edit.apply();

        _login.setEnabled(false);

        // Kick off the login process. The activity is notified of changes
        // in the login process through calls to the SessionListener.
        _service.login(username, password);
    }

    @Override
    protected void onResume()
    {
        super.onResume();

        bindService(_chatServiceIntent, _connection, BIND_AUTO_CREATE);
    }

    @Override
    public void onStop()
    {
        super.onStop();
        unbindService(_connection);

        if(_service != null)
        {
            _service.setSessionListener(null);
            _service = null;
        }
    }

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.login);

        _login = (Button)findViewById(R.id.login);
        _login.setOnClickListener(new android.view.View.OnClickListener()
        {
            public void onClick(android.view.View v)
            {
                login();
            }
        });
        _login.setEnabled(false);

        _username = (EditText)findViewById(R.id.username);
        _username.addTextChangedListener(new TextWatcher()
        {
            public void afterTextChanged(Editable s)
            {
                setLoginState();
            }

            public void beforeTextChanged(CharSequence s, int start, int count, int after)
            {
            }

            public void onTextChanged(CharSequence s, int start, int count, int after)
            {
            }
        });
        _password = (EditText)findViewById(R.id.password);

        _prefs = getPreferences(MODE_PRIVATE);

        if(savedInstanceState == null)
        {
            _username.setText(_prefs.getString(USERNAME_KEY, ""));
            _password.setText(_prefs.getString(PASSWORD_KEY, ""));
        }

        // Start the ChatService.
        _chatServiceIntent = new Intent(LoginActivity.this, ChatService.class);
        startService(_chatServiceIntent);
    }
}
