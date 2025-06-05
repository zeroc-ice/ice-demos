import React, { useState, useCallback, useEffect, useRef } from "react";
import { Ice } from "@zeroc/ice";
import { VisitorCenter } from "./Greeter.js";

interface AppState {
    name: string;
    greeting: string;
    loading: boolean;
    error: string;
}

function App(): JSX.Element {
    const [state, setState] = useState<AppState>({
        name: "React",
        greeting: "",
        loading: false,
        error: "",
    });

    // Refs to hold the communicator and greeter proxy
    const communicatorRef = useRef<Ice.Communicator | null>(null);
    const greeterRef = useRef<VisitorCenter.GreeterPrx | null>(null);

    // Initialize communicator and greeter proxy when component mounts
    useEffect(() => {
        try {
            // Create an Ice communicator
            const communicator = Ice.initialize();
            communicatorRef.current = communicator;

            // Create a proxy to the Greeter object. If you run the server on a different computer, replace localhost
            // with the server's hostname or IP address
            const greeter = new VisitorCenter.GreeterPrx(communicator, "greeter:ws -h localhost -p 4061");
            greeterRef.current = greeter;
        } catch (error) {
            console.error("Error initializing Ice:", error);
            setState((prev) => ({
                ...prev,
                error: `Failed to initialize Ice: ${error instanceof Error ? error.message : "Unknown error"}`,
            }));
        }

        return () => {
            const cleanup = async () => {
                try {
                    if (communicatorRef.current) {
                        await communicatorRef.current.destroy();
                    }
                } catch (err) {
                    console.warn("Failed to destroy communicator:", err);
                }
            };
            // Kick off the async cleanup, but don't return the promise
            cleanup();
        };
    }, []);

    const sendGreeting = useCallback(async () => {
        if (!state.name.trim()) {
            setState((prev) => ({ ...prev, error: "Please enter a name" }));
            return;
        }

        if (!greeterRef.current) {
            setState((prev) => ({ ...prev, error: "Ice connection not initialized" }));
            return;
        }

        setState((prev) => ({ ...prev, loading: true, error: "", greeting: "" }));

        try {
            // Send the greeting request using the existing greeter proxy
            const greeting = await greeterRef.current.greet(state.name.trim());

            setState((prev) => ({ ...prev, greeting, loading: false }));
        } catch (error) {
            console.error("Error sending greeting:", error);
            setState((prev) => ({
                ...prev,
                error: `Failed to send greeting: ${error instanceof Error ? error.message : "Unknown error"}`,
                loading: false,
            }));
        }
    }, [state.name]);

    const handleNameChange = (event: React.ChangeEvent<HTMLInputElement>) => {
        setState((prev) => ({ ...prev, name: event.target.value }));
    };

    const handleSubmit = (event: React.FormEvent<HTMLFormElement>) => {
        event.preventDefault();
        sendGreeting();
    };

    return (
        <div className="container">
            <h1>Ice Greeter React Demo</h1>
            <p>
            This demo shows how to use Ice for JavaScript in a React application. Make sure you have an Ice Config server running on 
            <code>localhost:4061</code> with the <code>ws</code> transport enabled.
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
                        disabled={state.loading}
                    />
                </div>

                <button type="submit" disabled={state.loading}>
                    {state.loading ? "Sending..." : "Send Greeting"}
                </button>
            </form>

            {state.loading && <div className="response loading">Sending greeting request...</div>}

            {state.error && <div className="response error">{state.error}</div>}

            {state.greeting && (
                <div className="response success">
                    <strong>Server Response:</strong> {state.greeting}
                </div>
            )}
        </div>
    );
}

export default App;
