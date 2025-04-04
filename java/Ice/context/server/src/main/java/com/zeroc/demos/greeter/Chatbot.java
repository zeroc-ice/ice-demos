// Copyright (c) ZeroC, Inc.

package com.zeroc.demos.greeter;

import com.zeroc.demos.VisitorCenter.Greeter;
import com.zeroc.Ice.Current;

/**
 * Chatbot is an Ice servant that implements Slice interface Greeter.
 */
class Chatbot implements Greeter
{
    // Implements the abstract method 'greet' from the Greeter interface generated by the Slice compiler.
    // This variant is the synchronous implementation.
    public String greet(String name, Current current) {
        // We retrieve the value for the language entry in the context.
        String language = current.ctx.getOrDefault("language", "");
        System.out.println("Dispatching greet request { name = '" + name + "', language = '" + language + "' }");

        // Return a greeting in the requested language.
        return switch(language)
        {
            case "de" -> "Hallo " + name + ", wie geht's?";
            case "es" -> "¡Hola " + name + "!";
            case "fr" -> "Bonjour " + name + ", comment vas-tu ?";
            default -> "Hello, " + name + "!";
        };
    }
}
