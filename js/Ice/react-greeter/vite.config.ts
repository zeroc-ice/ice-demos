import { defineConfig } from "vite";
import react from "@vitejs/plugin-react";

// https://vitejs.dev/config/
export default defineConfig({
    plugins: [react()],
    define: {
        global: "globalThis",
    },
    resolve: {
        alias: {
            process: "process/browser",
            buffer: "buffer",
            crypto: "crypto-browserify",
            stream: "stream-browserify",
            util: "util",
        },
    },
});
