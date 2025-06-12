// Copyright (c) ZeroC, Inc.

import { StrictMode } from "react";
import { createRoot } from "react-dom/client";
import GreeterApp from "./GreeterApp.tsx";
import "./app.css";

// Render the GreeterApp component into the root element
createRoot(document.getElementById("root")!).render(
    <StrictMode>
        <GreeterApp />
    </StrictMode>,
);
