// Copyright (c) ZeroC, Inc.

/* eslint no-sync: "off" */
/* eslint no-process-env: "off" */
/* eslint no-process-exit: "off" */
/* eslint no-shadow: "off" */

import { deleteAsync } from "del";
import vinylPaths from "vinyl-paths";
import fs from "fs";
import gulp from "gulp";
import iceBuilder from "gulp-ice-builder";
import path from "path";
import pump from "pump";
import { fileURLToPath } from "url";
import tsc from "gulp-typescript";

const __dirname = path.dirname(fileURLToPath(import.meta.url));
const root = path.resolve(__dirname);

//
// Check ICE_HOME environment variable. If this is set and points to a source
// distribution then prefer it over default packages.
//
const iceHome =
    process.env.ICE_HOME && fs.existsSync(path.join(process.env.ICE_HOME, "js", "package.json"))
        ? process.env.ICE_HOME
        : undefined;

function parseArg(argv, key) {
    for (let i = 0; i < argv.length; ++i) {
        const e = argv[i];
        if (e == key) {
            return argv[i + 1];
        } else if (e.indexOf(key + "=") === 0) {
            return e.substr(key.length + 1);
        }
    }
}

const platform = parseArg(process.argv, "--cppPlatform") || process.env.CPP_PLATFORM;
const configuration = parseArg(process.argv, "--cppConfiguration") || process.env.CPP_CONFIGURATION;

function slice2js(options) {
    if (iceHome) {
        if (process.platform == "win32") {
            if (!platform || (platform != "Win32" && platform != "x64")) {
                console.log(
                    "Error: CPP_PLATFORM environment variable must be set to " +
                        "`Win32' or `x64', in order to locate slice2js.exe",
                );
                process.exit(1);
            }

            if (!configuration || (configuration != "Debug" && configuration != "Release")) {
                console.log(
                    "Error: CPP_CONFIGURATION environment variable must be set to " +
                        "`Debug' or `Release', in order to locate slice2js.exe",
                );
                process.exit(1);
            }
            options.iceToolsPath = path.join(iceHome, "cpp", "bin", platform, configuration);
        }
        options.iceHome = iceHome;
    }
    return iceBuilder(options);
}

//
// If ICE_HOME is set we install Ice for JavaScript module from ICE_HOME
// that is required for running JavaScript NodeJS demos.
//
if (iceHome) {
    //
    // BUGFIX: do not use npm.commands.install to install local packages
    // https://npm.community/t/npm-install-for-package-with-local-dependency-fails/754
    //
    gulp.task("npm", (cb) => {
        const moduleDir = `${root}/node_modules/ice`;

        function install(err) {
            if (err) {
                cb(err);
            } else {
                fs.symlink(`${iceHome}/js`, moduleDir, "dir", (err) => {
                    cb(err);
                });
            }
        }

        fs.stat(moduleDir, (err) => {
            if (err) {
                if (err.code == "ENOENT") {
                    install();
                } else {
                    cb(err);
                }
            } else {
                deleteAsync(moduleDir).then(() => install());
            }
        });
    });
} else {
    gulp.task("npm", (cb) => {
        cb();
    });
}

const demos = [
    "Glacier2/simpleChat",
    "Ice/bidir",
    "Ice/hello",
    "Ice/latency",
    "Ice/minimal",
    "Ice/throughput",
    "Manual/printer",
    "Manual/simpleFilesystem",
];

const demoTaskName = (demoName, taskName) => "demo_" + demoName.replace(/\//g, "_") + ":" + taskName;

for (const demo of demos) {
    gulp.task(demoTaskName(demo, "slice2js"), (cb) => {
        pump(
            [
                gulp.src(`${root}/${demo}/*.ice`),
                slice2js({
                    include: [demo],
                    args: ["--typescript"],
                    jsbundle: false,
                    tsbundle: false,
                }),
                gulp.dest(path.join(root, demo)),
            ],
            cb,
        );
    });

    gulp.task(demoTaskName(demo, "tscompile"), (cb) => {
        pump(
            [
                gulp.src(`${root}/${demo}/*.ts`),
                tsc({
                    lib: ["dom", "es2022"],
                    target: "es2022",
                    module: "es2022",
                    noImplicitAny: true,
                    moduleResolution: "node",
                }),
                gulp.dest(demo),
            ],
            cb,
        );
    });

    gulp.task(
        demoTaskName(demo, "build"),
        gulp.series(demoTaskName(demo, "slice2js"), demoTaskName(demo, "tscompile")),
    );

    gulp.task(demoTaskName(demo, "clean"), (cb) => {
        pump([gulp.src([`${demo}/**/*.js`, `${demo}/**/*.d.ts`]), vinylPaths(deleteAsync)], cb);
    });
}

gulp.task(
    "build",
    gulp.series(
        "npm",
        demos.map((name) => demoTaskName(name, "build")),
    ),
);
gulp.task("clean", gulp.series(demos.map((name) => demoTaskName(name, "clean"))));

gulp.task("default", gulp.series("build"));
