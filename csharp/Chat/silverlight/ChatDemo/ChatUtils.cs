// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// This copy of Chat Demo is licensed to you under the terms described
// in the CHAT_DEMO_LICENSE file included in this distribution.
//
// **********************************************************************

using System;

namespace ChatDemo
{
    class ChatUtils
    {
        public static string formatTimestamp(long timestamp)
        {
            DateTime date = new DateTime(1970, 1, 1, 0, 0, 0, 0);
            date = date.AddMilliseconds(timestamp);
            string dateStr = date.ToLocalTime().ToString("T").Trim();
            int i = dateStr.IndexOf(" ");
            if(i != -1)
            {
                dateStr = dateStr.Substring(0, i);
            }
            return dateStr;
        }

        public static string formatUsername(string name)
        {
            try
            {
                return name.Substring(0, 1).ToUpper() + name.Substring(1, name.Length - 1).ToLower();
            }
            catch (ArgumentOutOfRangeException)
            { 
            }
            return name;
        }

        public static string stripHtml(string input)
        {
            var output = "";
            for(var cont = 0; cont < input.Length; cont++)
            {
                switch(input[cont])
                {
                    case '&':
                    {
                        output += "&amp;";
                        break;
                    }
        
                    case '"':
                    {
                        output += "&quot;";
                        break;
                    }
        
                    case '\'':
                    {
                        output += "&#39;";
                        break;
                    }
        
                    case '<':
                    {
                        output += "&lt;";
                        break;
                    }
        
                    case '>':
                    {
                        output += "&gt;";
                        break;
                    }
        
                    case '\r':
                    case '\n':
                    case '\v':
                    case '\f':
                    case '\t':
                    {
                        output += " ";
                        break;
                    }
        
                    default:
                    {
                        output += input[cont];
                        break;
                    }
                }
            }
            return output;
        }
    }
}
