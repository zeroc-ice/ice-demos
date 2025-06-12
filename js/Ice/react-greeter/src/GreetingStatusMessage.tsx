// Copyright (c) ZeroC, Inc.

import React from "react";

// Represent the status of a greeting request.
export interface GreetingStatus {
    // True if the greeting request is currently being processed otherwise false.
    loading: boolean;
    // An error message if the greeting request failed, otherwise null.
    error: string | null;
    // The greeting received from the server if the request was successful, otherwise null.
    greeting: string | null;
}

// The StatusMessage component displays the status of the greeting request,
// showing either a loading message, an error, or the greeting received from the server.
const GreetingStatusMessage: React.FC<GreetingStatus> = ({ loading, error, greeting }) => {
    if (loading) {
        return <div className="response loading">Sending greeting request...</div>;
    } else if (error !== null) {
        return <div className="response error">{error}</div>;
    } else if (greeting !== null) {
        return (
            <div className="response success">
                <strong>Server Response:</strong> {greeting}
            </div>
        );
    } else {
        // If no greeting is available, we return null to render nothing.
        return null;
    }
};

export default GreetingStatusMessage;
