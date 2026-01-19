//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import del from "del";
import fs from "fs";
import gulp from "gulp";
import iceBuilder from "gulp-ice-builder";
import path from "path";
import paths from "vinyl-paths";
import pump from "pump";
import tsc from "gulp-typescript";
import { fileURLToPath } from "url";

const __dirname = path.dirname(fileURLToPath(import.meta.url));
const root = __dirname;

function slice2js(options)
{
    const opts = options || {};
    return iceBuilder(opts);
}

const demos = [
    "Glacier2/simpleChat",
    "Ice/bidir",
    "Ice/hello",
    "Ice/latency",
    "Ice/minimal",
    "Ice/throughput",
    "Manual/printer",
    "Manual/simpleFilesystem"
];

const demoTaskName = name => "demo_" + name.replace(/\//g, "_");

const demoSliceJsTask = name => demoTaskName(name) + ":slice2js";
const demoTsCompileTask = name => demoTaskName(name) + ":tscompile";

const demoBuildTask = name => demoTaskName(name) + ":build";
const demoCleanTask = name => demoTaskName(name) + ":clean";

for(const demo of demos)
{
    gulp.task(demoSliceJsTask(demo),
        cb =>
        {
            pump([
                gulp.src(path.join(root, demo, "*.ice")),
                slice2js(
                    {
                        include: [demo],
                        args: ["--typescript"],
                        jsbundleFormat: "cjs"
                    }),
                gulp.dest(path.join(root, demo))
            ], cb);
        });

    gulp.task(demoTsCompileTask(demo),
              cb =>
              {
                  pump([
                      gulp.src(path.join(root, demo, "*.ts")),
                      tsc(
                          {
                              lib: ["es2017"],
                              target: "es2017",
                              module: "commonjs",
                              noImplicitAny: true,
                              noUnusedLocals: true
                          }),
                      gulp.dest(demo)
                  ], cb);
              });

    gulp.task(demoBuildTask(demo), gulp.series(demoSliceJsTask(demo), demoTsCompileTask(demo)));

    gulp.task(demoCleanTask(demo),
              cb =>
              {
                  pump([gulp.src([path.join(demo, "**/*.js"),
                                  path.join(demo, "**/*.d.ts"),
                                  path.join(demo, "**/*.map")]), paths(del)], cb);
              });
}

gulp.task("build", gulp.series(demos.map(demoBuildTask)));
gulp.task("clean", gulp.series(demos.map(demoCleanTask)));

gulp.task("default", gulp.series("build"));
