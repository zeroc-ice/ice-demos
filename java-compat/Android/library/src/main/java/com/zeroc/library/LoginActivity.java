// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

package com.zeroc.library;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;

import com.zeroc.library.controller.LoginController;
import com.zeroc.library.controller.SessionController;

public class LoginActivity extends Activity
{

    private static final String USERNAME_KEY = "username";
    private static final String PASSWORD_KEY = "password";
    private static final String ERROR_TAG = "error";

    private Button _login;
    private EditText _username;
    private EditText _password;
    private SharedPreferences _prefs;
    private LoginController _loginController;
    private boolean _loginInProgress = false;

    private LoginController.Listener _listener = new LoginController.Listener()
    {
        public void onLoginInProgress()
        {
            _loginInProgress = true;
            setLoginState();
        }

        public void onLogin(SessionController sessionController)
        {
            if(isFinishing())
            {
                return;
            }

            LibraryApp app = (LibraryApp)getApplication();
            app.loggedIn(sessionController);
            startActivity(new Intent(LoginActivity.this, LibraryActivity.class));
        }

        public void onLoginError()
        {
            _loginInProgress = false;
            setLoginState();

            DialogFragment dialog = ErrorDialogFragment.newInstance(_loginController.getLoginError());
            dialog.show(getFragmentManager(), ERROR_TAG);
        }
    };

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

        LibraryApp app = (LibraryApp)getApplication();
        _loginController = app.login(username, password, _listener);
    }

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.login);

        _login = (Button) findViewById(R.id.login);
        _login.setOnClickListener(new android.view.View.OnClickListener()
        {
            public void onClick(android.view.View v)
            {
                login();
            }
        });
        _username = (EditText) findViewById(R.id.username);
        _username.addTextChangedListener(new TextWatcher()
        {
            @Override
            public void afterTextChanged(Editable s)
            {
                setLoginState();
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

        _password = (EditText) findViewById(R.id.password);
        _password.addTextChangedListener(new TextWatcher()
        {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after)
            {
            }

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count)
            {
            }

            @Override
            public void afterTextChanged(Editable s)
            {
                setLoginState();
            }
        });

        _prefs = getPreferences(MODE_PRIVATE);

        _prefs = getPreferences(MODE_PRIVATE);
        if(savedInstanceState == null)
        {
            _username.setText(_prefs.getString(USERNAME_KEY, ""));
            _password.setText(_prefs.getString(PASSWORD_KEY, ""));
        }
    }

    @Override
    protected void onResume()
    {
        super.onResume();
        LibraryApp app = (LibraryApp)getApplication();
        _loginInProgress = false;
        _loginController = app.getLoginController();
        if(_loginController != null)
        {
            _loginController.setLoginListener(_listener);
        }
        else
        {
            setLoginState();
        }
    }

    @Override
    protected void onStop()
    {
        super.onStop();
        if(_loginController != null)
        {
            _loginController.setLoginListener(null);
            _loginController = null;
        }
    }

    public static class InvalidHostDialogFragment extends DialogFragment
    {
        @Override
        public Dialog onCreateDialog(Bundle savedInstanceState)
        {
            AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
            builder.setTitle("Error")
                   .setMessage("The hostname is invalid");
            return builder.create();
        }
    }
    public static class ErrorDialogFragment extends DialogFragment
    {
        public static ErrorDialogFragment newInstance(String message)
        {
            ErrorDialogFragment frag = new ErrorDialogFragment();
            Bundle args = new Bundle();
            args.putString("message", message);
            frag.setArguments(args);
            return frag;
        }

        @Override
        public Dialog onCreateDialog(Bundle savedInstanceState)
        {
            AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
            builder .setTitle("Error")
                    .setMessage(getArguments().getString("message"))
                    .setCancelable(false)
                    .setPositiveButton("Ok", new DialogInterface.OnClickListener()
                    {
                        public void onClick(DialogInterface dialog, int whichButton)
                        {
                            ((LoginActivity) getActivity()).errorOk();
                        }
                    });
            return builder.create();
        }
    }

    private void errorOk()
    {
        // Clean up the login controller upon login failure.
        if(_loginController != null)
        {
            LibraryApp app = (LibraryApp)getApplication();
            app.loginFailure();
        }
    }
}
