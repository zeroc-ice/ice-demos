import esbuild from "esbuild";
import slice2js from "@zeroc/slice2js/unplugin/esbuild";

await esbuild.build({
    entryPoints: ["sensor.ts"],
    bundle: true,
    packages: "external",
    platform: "node",
    format: "esm",
    target: "node22",
    outfile: "sensor.js",
    sourcemap: true,
    plugins: [
        slice2js({
            inputs: ["WeatherStation.ice"],
            args: ["--typescript"],
            outputDir: ".",
        }),
    ],
});
