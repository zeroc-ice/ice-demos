import React from "react";

interface StatusMessageProps {
    loading: boolean;
    error: string;
    greeting: string;
}

const StatusMessage: React.FC<StatusMessageProps> = ({ loading, error, greeting }) => {
    if (loading) {
        return <div className="response loading">Sending greeting request...</div>;
    }

    if (error) {
        return <div className="response error">{error}</div>;
    }

    if (greeting) {
        return (
            <div className="response success">
                <strong>Server Response:</strong> {greeting}
            </div>
        );
    }

    return null;
};

export default StatusMessage;
