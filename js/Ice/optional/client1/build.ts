import esbuild from "esbuild";
import slice2js from "@zeroc/slice2js/unplugin/esbuild";

await esbuild.build({
    entryPoints: ["client.ts"],
    bundle: true,
    packages: "external",
    platform: "node",
    format: "esm",
    target: "node22",
    outfile: "client.js",
    sourcemap: true,
    plugins: [
        slice2js({
            inputs: ["WeatherStation1.ice"],
            args: ["--typescript"],
            outputDir: ".",
        }),
    ],
});
