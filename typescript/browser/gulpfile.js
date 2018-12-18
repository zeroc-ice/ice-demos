// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

/* eslint no-sync: "off" */
/* eslint no-process-env: "off" */
/* eslint no-process-exit: "off" */
/* eslint no-shadow: "off" */

const del = require("del");
const fs = require("fs");
const gulp = require("gulp");
const iceBuilder = require("gulp-ice-builder");
const open = require("gulp-open");
const path = require("path");
const paths = require("vinyl-paths");
const pump = require("pump");
const rollup = require("rollup");
const tsc = require("gulp-typescript");
const httpServer = require("./bin/HttpServer");

const root = path.resolve(".");
//
// Check ICE_HOME environment variable. If this is set and points to a source
// distribution then prefer it over default packages.
//
const iceHome = process.env.ICE_HOME && fs.existsSync(path.join(process.env.ICE_HOME, "js", "package.json")) ?
    process.env.ICE_HOME : undefined;

function parseArg(argv, key)
{
    for(let i = 0; i < argv.length; ++i)
    {
        const e = argv[i];
        if(e == key)
        {
            return argv[i + 1];
        }
        else if(e.indexOf(key + "=") === 0)
        {
            return e.substr(key.length + 1);
        }
    }
}

const platform = parseArg(process.argv, "--cppPlatform") || process.env.CPP_PLATFORM;
const configuration = parseArg(process.argv, "--cppConfiguration") || process.env.CPP_CONFIGURATION;

function slice2js(options)
{
    if(iceHome)
    {
        if(process.platform == "win32")
        {
            if(!platform || (platform != "Win32" && platform != "x64"))
            {
                console.log("Error: CPP_PLATFORM environment variable must be set to " +
                            "`Win32' or `x64', in order to locate slice2js.exe");
                process.exit(1);
            }

            if(!configuration || (configuration != "Debug" && configuration != "Release"))
            {
                console.log("Error: CPP_CONFIGURATION environment variable must be set to " +
                            "`Debug' or `Release', in order to locate slice2js.exe");
                process.exit(1);
            }
            options.iceToolsPath = path.join(iceHome, 'cpp', 'bin', platform, configuration);
        }
        options.iceHome = iceHome;
    }
    return iceBuilder(options);
}

//
// If ICE_HOME is set we install Ice for JavaScript module from ICE_HOME
// that is required for running JavaScript NodeJS demos.
//
if(iceHome)
{
    //
    // BUGFIX: do not use npm.commands.install to install local packages
    // https://npm.community/t/npm-install-for-package-with-local-dependency-fails/754
    //
    gulp.task("npm",
              cb =>
              {
                  const moduleDir = path.join(root, "node_modules", "ice");

                  function install(err)
                  {
                      if(err)
                      {
                          cb(err);
                      }
                      else
                      {
                          fs.symlink(path.join(iceHome, 'js'), path.join(root, "node_modules", "ice"), "dir",
                                     err =>
                                     {
                                         cb(err);
                                     });
                      }
                  }

                  fs.stat(moduleDir,
                          err =>
                          {
                              if(err)
                              {
                                  if(err.code == "ENOENT")
                                  {
                                      install();
                                  }
                                  else
                                  {
                                      cb(err);
                                  }
                              }
                              else
                              {
                                  del(moduleDir).then(() => install());
                              }
                          });
              });
}
else
{
    gulp.task("npm",
              cb =>
              {
                  cb();
              });
}

const demos = [
    "Chat",
    "Glacier2/simpleChat",
    "Ice/bidir",
    "Ice/hello",
    "Ice/latency",
    "Ice/minimal",
    "Ice/throughput",
    "Manual/printer",
    "Manual/simpleFilesystem"];

const demoTaskName = name => "demo_" + name.replace(/\//g, "_");

const demoSliceJsTask = name => demoTaskName(name) + ":slice2js";
const demoTsCompileTask = name => demoTaskName(name) + ":tscompile";

const demoBundleTask = name => demoTaskName(name) + ":bundle";
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
                        args: ["--typescript"]
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
                        lib: ["dom", "es2017"],
                        target: "es2017",
                        module: "es6",
                        noImplicitAny: true,
                        noUnusedLocals: true,
                        moduleResolution: "node",
                        types: ["@types/jquery"]
                    }),
                gulp.dest(path.join(root, demo))
            ], cb);
        });

    gulp.task(demoBundleTask(demo),
        async () =>
        {
            const bundle = await rollup.rollup(
                {
                    input: path.join(demo, "Client.js"),
                    external: ["ice"]
                });

            return bundle.write(
                {
                    file: path.join(demo, "main.js"),
                    format: "iife",
                    globals:
                    {
                        ice: "ice"
                    }
                });
        });

    gulp.task(demoBuildTask(demo),
        gulp.series(demoSliceJsTask(demo), demoTsCompileTask(demo), demoBundleTask(demo)));

    gulp.task(demoCleanTask(demo),
              cb =>
              {
                  pump([gulp.src([path.join(demo, "**/*.js"),
                                  path.join(demo, "**/*.d.ts"),
                                  path.join(demo, "**/*.map")]),
                        paths(del)], cb);
              });
}

gulp.task("build", gulp.series("npm", demos.map(demoBuildTask)));
gulp.task("clean", gulp.series(demos.map(demoCleanTask)));

gulp.task("default", gulp.series("build"));

gulp.task("run",
          () =>
          {
              httpServer();
              return gulp.src(__filename).pipe(open({uri: "http://127.0.0.1:8080/index.html"}));
          });
