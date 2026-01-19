//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import concat from "gulp-concat";
import fs from "fs";
import gulp from "gulp";
import gzip from "gulp-gzip";
import iceBuilder from "gulp-ice-builder";
import newer from "gulp-newer";
import open from "open";
import path from "path";
import pump from "pump";
import terser from "gulp-terser";
import { fileURLToPath } from "url";
import httpServer from "./bin/HttpServer.mjs";

const __dirname = path.dirname(fileURLToPath(import.meta.url));
const fsp = fs.promises;

const root = __dirname;

function slice2js(options)
{
    const opts = options || {};
    return iceBuilder({
        args: opts.args || [],
        include: opts.include || [],
        dest: opts.dest
    });
}

const demos =
{
    "Glacier2/simpleChat":
    {
        srcs: [
            "node_modules/ice/lib/Ice.min.js",
            "node_modules/ice/lib/Glacier2.min.js",
            "Glacier2/simpleChat/generated/Chat.js",
            "Glacier2/simpleChat/browser/Client.js"],
        dest: "Glacier2/simpleChat/browser/"
    },
    "Ice/bidir":
    {
        srcs: ["node_modules/ice/lib/Ice.min.js",
               "Ice/bidir/generated/Callback.js",
               "Ice/bidir/browser/Client.js"],
        dest: "Ice/bidir/browser"
    },
    "Ice/hello":
    {
        srcs: [
            "node_modules/ice/lib/Ice.min.js",
            "Ice/hello/generated/Hello.js",
            "Ice/hello/browser/Client.js"],
        dest: "Ice/hello/browser"
    },
    "Ice/latency":
    {
        srcs: [
            "node_modules/ice/lib/Ice.min.js",
            "Ice/latency/generated/Latency.js",
            "Ice/latency/browser/Client.js"],
        dest: "Ice/latency/browser"
    },
    "Ice/minimal":
    {
        srcs: [
            "node_modules/ice/lib/Ice.min.js",
            "Ice/minimal/generated/Hello.js",
            "Ice/minimal/browser/Client.js"],
        dest: "Ice/minimal/browser/"
    },
    "Ice/throughput":
    {
        srcs: [
            "node_modules/ice/lib/Ice.min.js",
            "Ice/throughput/generated/Throughput.js",
            "Ice/throughput/browser/Client.js"],
        dest: "Ice/throughput/browser/"
    },
    "Manual/printer":
    {
        srcs: [
            "node_modules/ice/lib/Ice.min.js",
            "Manual/printer/generated/Printer.js",
            "Manual/simpleFileSystem/browser/Client.js"],
        dest: "Manual/printer/browser"
    },
    "Manual/simpleFileSystem":
    {
        srcs: [
            "node_modules/ice/lib/Ice.min.js",
            "Manual/simpleFileSystem/generated/Filesystem.js",
            "Manual/printer/browser/Client.js"],
        dest: "Manual/simpleFileSystem/browser"
    }
};

const demoTaskName = name => "demo_" + name.replace("/", "_");
const demoSlice2JsTask = name => demoTaskName(name) + ":slice2js";
const demoMinTask = name => demoTaskName(name) + ":minimize";
const demoBuildTask = name => demoTaskName(name) + ":build";
const demoCleanTask = name => demoTaskName(name) + ":clean";

for(const name of Object.keys(demos))
{
    const demo = demos[name];
    gulp.task(demoSlice2JsTask(name),
              cb =>
              {
                  const outdir = path.join(root, name, "generated");
                  pump([
                      gulp.src(path.join(root, name, "*.ice")),
                      slice2js({include: [path.join(root, name)], dest: outdir}),
                      gulp.dest(outdir)], cb);
              });

    gulp.task(demoMinTask(name),
              cb =>
              {
                  const outdir = path.join(root, demo.dest);
                  pump([gulp.src(demo.srcs),
                        newer(path.join(outdir, "Client.min.js")),
                        concat("Client.min.js"),
                        terser(),
                        gulp.dest(outdir),
                        gzip(),
                        gulp.dest(outdir)], cb);
              });

    gulp.task(demoCleanTask(name),
              async () =>
              {
                  const rmOpts = { recursive: true, force: true };
                  await Promise.all([
                      fsp.rm(path.join(root, name, ".depend"), rmOpts),
                      fsp.rm(path.join(root, name, "generated"), rmOpts),
                      fsp.rm(path.join(root, demo.dest, "Client.min.js"), rmOpts),
                      fsp.rm(path.join(root, demo.dest, "Client.min.js.gz"), rmOpts)
                  ]);
              });

    gulp.task(demoBuildTask(name),
              gulp.series(demoSlice2JsTask(name),
                          demoMinTask(name)));
}

gulp.task("build", gulp.parallel(Object.keys(demos).map(demoBuildTask)));

gulp.task("clean", gulp.parallel(Object.keys(demos).map(demoCleanTask)));

gulp.task("run", gulp.series("build",
                             async () =>
                             {
                                 httpServer();
                                 await open("http://localhost:8080/index.html");
                             }));

gulp.task("default", gulp.series("build"));
