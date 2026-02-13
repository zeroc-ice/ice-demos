import { defineConfig } from "vite";
import react from "@vitejs/plugin-react";
import slice2js from "@zeroc/slice2js/unplugin/vite";

// https://vitejs.dev/config/
export default defineConfig({
    plugins: [
        react(),
        slice2js({
            inputs: ["src/Greeter.ice"],
            args: ["--typescript"],
            outputDir: "src",
        }),
    ],
});
