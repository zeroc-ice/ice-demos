// Copyright (c) ZeroC, Inc.

import React, { useState, useCallback, useEffect, useRef } from "react";
import { Ice } from "@zeroc/ice";
import { VisitorCenter } from "./Greeter.js";

import GreetingStatusMessage from "./GreetingStatusMessage.js";
import { GreetingStatus } from "./GreetingStatusMessage.js";

// Define the state of the GreeterApp component
interface GreeterAppState {
    // The name to greet, editable by the user.
    name: string;
    // The status of the greeting request.
    greetingStatus: GreetingStatus;
    // The error message set if communicator initialization fails, otherwise null.
    initializeError?: string | null;
}

function GreeterApp(): JSX.Element {
    // Initialize the state of the GreeterApp component.
    const [state, setState] = useState<GreeterAppState>({
        name: "React",
        greetingStatus: {
            greeting: null,
            loading: false,
            error: null,
        },
    });

    // Use a ref to hold Ice communicator, is initialized when the component mounts in the useEffect hook.
    const communicatorRef = useRef<Ice.Communicator | null>(null);
    // Use a ref to hold the Greeter proxy, which is created when the component mounts in the useEffect hook.
    const greeterRef = useRef<VisitorCenter.GreeterPrx | null>(null);

    // Initialize communicator and greeter proxy when component mounts
    useEffect(() => {
        try {
            // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
            communicatorRef.current = Ice.initialize();

            // Create a proxy to the Greeter object. If you run the server on a different computer, replace localhost
            // with the server's hostname or IP address
            greeterRef.current = new VisitorCenter.GreeterPrx(
                communicatorRef.current,
                "greeter:ws -h localhost -p 4061",
            );
        } catch (error) {
            // If an error occurs during initialization, update the state with the initialize error message.
            setState((prev) => ({
                ...prev,
                initializeError: `Failed to initialize Ice communicator: ${error}`,
            }));
        }

        // Return a cleanup function to destroy the communicator when the component unmounts.
        return () => {
            communicatorRef.current?.destroy().catch((err: Error) => {
                console.error("Error destroying Ice communicator:", err);
            });
        };
    }, []);

    // Callback to send a greeting request using the current name.
    // Recreated only when `state.name` changes.
    const sendGreeting = useCallback(async () => {
        // Reset the status before sending the greeting.
        setState((prev) => ({ ...prev, greetingStatus: { loading: true, error: null, greeting: null } }));

        try {
            console.assert(greeterRef.current !== null, "Greeter proxy should not be null");
            // Send the greeting request and wait for the response.
            const greeting = await greeterRef.current!.greet(state.name.trim());

            // Update the state with the received greeting, and reset the loading state.
            setState((prev) => ({ ...prev, greetingStatus: { loading: false, error: null, greeting: greeting } }));
        } catch (error) {
            // If an error occurs, update the state with the error message, and reset the loading state.
            setState((prev) => ({
                ...prev,
                greetingStatus: {
                    loading: false,
                    error: `Failed to send greeting: ${error instanceof Error ? error.message : "Unknown error"}`,
                    greeting: null,
                },
            }));
        }
    }, [state.name]);

    // Handle changes to the name input field.
    const handleNameChange = (event: React.ChangeEvent<HTMLInputElement>) => {
        setState((prev) => ({ ...prev, name: event.target.value }));
    };

    // Handle form submission to send the greeting.
    const handleSubmit = (event: React.FormEvent<HTMLFormElement>) => {
        event.preventDefault();
        sendGreeting();
    };

    // If initialization failed, render the error instead of the greeting form.
    if (state.initializeError) {
        return (
            <div className="container">
                <h1>Ice Greeter React Demo</h1>
                <div className="error">{state.initializeError}</div>
            </div>
        );
    } else {
        return (
            <div className="container">
                <h1>Ice Greeter React Demo</h1>
                <p>
                    This demo shows how to use Ice for JavaScript in a React application. Make sure you have an Ice
                    Config server running on <code>localhost:4061</code> with the <code>ws</code> transport enabled.
                </p>

                <form className="greeting-form" onSubmit={handleSubmit}>
                    <div className="form-group">
                        <label htmlFor="name">Name:</label>
                        <input
                            id="name"
                            type="text"
                            value={state.name}
                            onChange={handleNameChange}
                            placeholder="Enter your name"
                            disabled={state.greetingStatus.loading}
                        />
                    </div>

                    <button type="submit" disabled={state.greetingStatus.loading}>
                        {state.greetingStatus.loading ? "Sending..." : "Send Greeting"}
                    </button>
                </form>
                <GreetingStatusMessage {...state.greetingStatus} />
            </div>
        );
    }
}

export default GreeterApp;
