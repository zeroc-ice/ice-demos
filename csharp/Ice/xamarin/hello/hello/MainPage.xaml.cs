//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

using System;
using System.IO;
using System.Reflection;
using System.Security.Cryptography.X509Certificates;
using Xamarin.Forms;

namespace hello
{
    public partial class MainPage : ContentPage
    {
        const string TWOWAY = "Twoway";
        const string TWOWAY_SECURE = "Twoway Secure";
        const string ONEWAY = "Oneway";
        const string ONEWAY_BATCH = "Oneway Batch";
        const string ONEWAY_SECURE = "Oneway Secure";
        const string ONEWAY_SECURE_BATCH = "Oneway Secure Batch";
        const string DATAGRAM = "Datagram";
        const string DATAGRAM_BATCH = "Datagram Batch";

        public MainPage(string defaultHost)
        {
            InitializeComponent();
            Mode.Items.Add(TWOWAY);
            Mode.Items.Add(TWOWAY_SECURE);
            Mode.Items.Add(ONEWAY);
            Mode.Items.Add(ONEWAY_BATCH);
            Mode.Items.Add(ONEWAY_SECURE);
            Mode.Items.Add(ONEWAY_SECURE_BATCH);
            Mode.Items.Add(DATAGRAM);
            Mode.Items.Add(DATAGRAM_BATCH);
            Mode.SelectedIndex = 0;
            Hostname.Text = defaultHost;
            Hostname.TextChanged += Hostname_TextChanged;
            Mode.SelectedIndexChanged += Mode_SelectedIndexChanged;
            Timeout.ValueChanged += Timeout_ValueChanged;

            Hello.Clicked += Hello_Clicked;
            Shutdown.Clicked += Shutdown_Clicked;
            Flush.Clicked += Flush_Clicked;

            IceSSL.Util.registerIceSSL(true);
            var initData = new Ice.InitializationData();
            initData.properties = Ice.Util.createProperties();
            initData.properties.setProperty("Ice.InitPlugins", "0");
            initData.dispatcher = (Action action, Ice.Connection connection) =>
                {
                    Device.BeginInvokeOnMainThread(action);
                };
            _communicator = Ice.Util.initialize(initData);

            var plugin = (IceSSL.Plugin)_communicator.getPluginManager().getPlugin("IceSSL");
            plugin.setCertificates(loadCertificate("client.p12", "password"));
            plugin.setCACertificates(loadCertificate("cacert.der"));
            _communicator.getPluginManager().initializePlugins();
        }

        private void Timeout_ValueChanged(object sender, ValueChangedEventArgs e)
        {
            _helloPrx = null;
        }

        public X509Certificate2Collection loadCertificate(string name, string password = "")
        {
            var assembly = IntrospectionExtensions.GetTypeInfo(typeof(MainPage)).Assembly;
            var stream = assembly.GetManifestResourceStream(string.Format("hello.{0}", name));
            var memoryStream = new MemoryStream();
            stream.CopyTo(memoryStream);
            var cert = new X509Certificate2Collection();
            if(string.IsNullOrEmpty(password))
            {
                cert.Import(memoryStream.ToArray());
            }
            else
            {
                cert.Import(memoryStream.ToArray(), password, X509KeyStorageFlags.UserKeySet);
            }
            return cert;
        }

        private void Hostname_TextChanged(object sender, TextChangedEventArgs e)
        {
            if(string.IsNullOrEmpty(Hostname.Text))
            {
                Hello.IsEnabled = false;
                Shutdown.IsEnabled = false;
                Flush.IsEnabled = false;
                Status.Text = "No hostname";
            }
            else
            {
                Hello.IsEnabled = true;
                Shutdown.IsEnabled = true;
                Flush.IsEnabled = false;
            }
            _helloPrx = null;

        }

        private void Mode_SelectedIndexChanged(object sender, EventArgs e)
        {
            Flush.IsEnabled = false;
            _helloPrx = null;
        }

        private bool deliveryModeIsBatch()
        {
            return Mode.SelectedItem.ToString().Equals(ONEWAY_BATCH) ||
                Mode.SelectedItem.ToString().Equals(ONEWAY_SECURE_BATCH) ||
                Mode.SelectedItem.ToString().Equals(DATAGRAM_BATCH);
        }

        private Ice.ObjectPrx deliveryModeApply(Ice.ObjectPrx prx)
        {
            if(Mode.SelectedItem.ToString().Equals(TWOWAY))
            {
                prx = prx.ice_twoway();
            }
            else if(Mode.SelectedItem.ToString().Equals(TWOWAY_SECURE))
            {
                prx = prx.ice_twoway().ice_secure(true);
            }
            else if(Mode.SelectedItem.ToString().Equals(ONEWAY))
            {
                prx = prx.ice_oneway();
            }
            else if(Mode.SelectedItem.ToString().Equals(ONEWAY_BATCH))
            {
                prx = prx.ice_batchOneway();
            }
            else if(Mode.SelectedItem.ToString().Equals(ONEWAY_SECURE))
            {
                prx = prx.ice_oneway().ice_secure(true);
            }
            else if(Mode.SelectedItem.ToString().Equals(ONEWAY_SECURE_BATCH))
            {
                prx = prx.ice_batchOneway().ice_secure(true);
            }
            else if(Mode.SelectedItem.ToString().Equals(DATAGRAM))
            {
                prx = prx.ice_datagram();
            }
            else if(Mode.SelectedItem.ToString().Equals(DATAGRAM_BATCH))
            {
                prx = prx.ice_batchDatagram();
            }

            return prx;
        }

        void updateProxy()
        {
            if(_communicator == null)
            {
                return;
            }
            var host = Hostname.Text.Trim();

            var s = "hello:tcp -h " + host + " -p 10000:ssl -h " + host + " -p 10001:udp -h " + host + " -p 10000";
            var prx = _communicator.stringToProxy(s);
            prx = deliveryModeApply(prx);
            int timeout = (int)Timeout.Value;
            if(timeout != 0)
            {
                prx = prx.ice_invocationTimeout(timeout);
            }

            //
            // The batch requests associated to the proxy are lost when we
            // update the proxy.
            //
            Flush.IsEnabled = false;

            _helloPrx = Demo.HelloPrxHelper.uncheckedCast(prx);
        }

        private async void Flush_Clicked(object sender, EventArgs e)
        {
            if(_helloPrx == null)
            {
                return;
            }
            try
            {
                await _helloPrx.ice_flushBatchRequestsAsync();
                Flush.IsEnabled = false;
                Status.Text = "Flushed batch requests";
            }
            catch(Exception ex)
            {
                handleException(ex);
            }
        }

        private async void Shutdown_Clicked(object sender, EventArgs e)
        {
            if(_helloPrx == null)
            {
                updateProxy();
            }

            int delay = (int)Delay.Value;

            try
            {
                if(!deliveryModeIsBatch())
                {
                    Status.Text = "Sending request";
                    await _helloPrx.shutdownAsync();
                    Status.Text = "Ready";
                }
                else
                {
                    Flush.IsEnabled = true;
                    _helloPrx.shutdown();
                    Status.Text = "Queued shutdown request";
                }
            }
            catch(Exception ex)
            {
                handleException(ex);
            }
        }

        private async void Hello_Clicked(object sender, EventArgs e)
        {
            if(_helloPrx == null)
            {
                updateProxy();
            }

            int delay = (int)Delay.Value;
            try
            {
                if(!deliveryModeIsBatch())
                {
                    Status.Text = "Sending request";
                    bool haveResponse = false;
                    await _helloPrx.sayHelloAsync(delay, progress: new Progress<bool>((value) =>
                            {
                                if(!haveResponse)
                                {
                                    if(Mode.SelectedItem.Equals(TWOWAY) || Mode.SelectedItem.Equals(TWOWAY_SECURE))
                                    {
                                        Status.Text = "Waiting for response";
                                    }
                                    else
                                    {
                                        Status.Text = "Ready";
                                    }
                                }
                            }));
                    haveResponse = true;
                    Status.Text = "Ready";
                }
                else
                {
                    Flush.IsEnabled = true;
                    _helloPrx.sayHello(delay);
                    Status.Text = "Queued sayHello request";
                }
            }
            catch(Exception ex)
            {
                handleException(ex);
            }
        }

        private void handleException(Exception ex)
        {
            Status.Text = ex.GetType().ToString();
        }

        private Ice.Communicator _communicator;
        private Demo.HelloPrx _helloPrx;
    }
}
