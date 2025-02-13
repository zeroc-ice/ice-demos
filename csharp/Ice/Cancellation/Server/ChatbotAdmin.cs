// Copyright (c) ZeroC, Inc.

using VisitorCenter;

namespace Server;

/// <summary>ChatbotAdmin is an Ice servant that implements Slice interface GreeterAdmin.</summary>
internal class ChatbotAdmin : GreeterAdminDisp_
{
    private readonly Chatbot _chatbot;

    /// <inheritdoc/>
    public override void Pause(Ice.Current current) => _chatbot.Pause();

    /// <inheritdoc/>
    public override void Resume(Ice.Current current) => _chatbot.Resume();

    /// <summary>Constructs a ChatbotAdmin servant.</summary>
    /// <param name="chatbot"></param>The chatbot servant being administered by this instance.<summary>
    internal ChatbotAdmin(Chatbot chatbot) => _chatbot = chatbot;
}
