// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

const del         = require("del"),
      fs          = require("fs"),
      gulp        = require("gulp"),
      iceBuilder  = require("gulp-ice-builder"),
      newer       = require("gulp-newer"),
      npm         = require("npm"),
      open        = require("gulp-open"),
      path        = require("path"),
      paths       = require("vinyl-paths"),
      pump        = require("pump"),
      resolve     = require("rollup-plugin-node-resolve"),
      rollup      = require("rollup"),
      tsc         = require("gulp-typescript"),
      extreplace  = require("gulp-ext-replace"),
      HttpServer  = require("./bin/HttpServer");

//
// Check ICE_HOME environment variable. If this is set and points to a source
// distribution then prefer it over default packages.
//
let iceHome;
if(process.env.ICE_HOME)
{
    iceHome = fs.existsSync(path.join(process.env.ICE_HOME, "js", "package.json")) ? process.env.ICE_HOME : undefined;
}

function parseArg(argv, key)
{
    for(var i = 0; i < argv.length; ++i)
    {
        var e = argv[i];
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

var platform = parseArg(process.argv, "--cppPlatform") || process.env.CPP_PLATFORM;
var configuration = parseArg(process.argv, "--cppConfiguration") || process.env.CPP_CONFIGURATION;

function slice2js(options)
{
    var defaults = {};
    var opts = options || {};

    defaults.args = opts.args || [];
    defaults.include = opts.include || []
    defaults.dest = opts.dest;
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
            defaults.iceToolsPath = path.join(iceHome, 'cpp', 'bin', platform, configuration);
        }
        defaults.iceHome = iceHome;
    }
    return iceBuilder.compile(defaults);
}

//
// If ICE_HOME is set we install Ice for JavaScript module from ICE_HOME
// that is required for running JavaScript NodeJS demos.
//
gulp.task("npm", [],
          cb =>
          {
              npm.load({loglevel: 'silent', progress: false},
                       (err, npm) =>
                       {
                           if(iceHome)
                           {
                               npm.commands.install([path.join(iceHome, 'js')],
                                                    (err, data) =>
                                                    {
                                                        cb(err);
                                                    });
                           }
                           else
                           {
                               cb();
                           }
                       });
          });

const browserDemos = [
    "Chat",
    "Glacier2/simpleChat",
    "Ice/bidir",
    "Ice/hello",
    "Ice/latency",
    "Ice/minimal",
    "Ice/throughput",
    "Manual/printer",
    "Manual/simpleFileSystem"];

const demoTaskName = name => "demo_" + name.replace(/\//g, "_");

const demoSliceJsTask = name => demoTaskName(name) + ":slice-compile-js";
const demoSliceJsCleanTask = name => demoTaskName(name) + ":slice-compile-js:clean";
const demoSliceTsTask = name => demoTaskName(name) + ":slice-compile-ts";
const demoSliceTsCleanTask = name => demoTaskName(name) + ":slice-compile-ts:clean";

const demoBuildTask = name => demoTaskName(name) + ":build";
const demoBundleTask = name => demoTaskName(name) + ":bundle";
const demoCleanTask = name => demoTaskName(name) + ":clean";

for(let demo of browserDemos)
{
    gulp.task(demoSliceJsTask(demo), [],
              cb =>
              {
                  pump([
                      gulp.src(path.join(demo, "*.ice")),
                      slice2js({include:[demo], dest:path.join(demo, "node_modules", "demo")}),
                      gulp.dest(path.join(demo, "node_modules", "demo"))
                  ], cb);
              });

    gulp.task(demoSliceTsTask(demo), [],
              cb =>
              {
                  pump([
                      gulp.src(path.join(demo, "*.ice")),
                      slice2js({
                        include:[demo],
                        dest:path.join(demo, "node_modules", "demo"),
                        args:["--typescript"]}),
                      gulp.dest(path.join(demo, "node_modules", "demo"))
                  ], cb);
              });

    gulp.task(demoBuildTask(demo), ["npm", demoSliceJsTask(demo), demoSliceTsTask(demo)],
              cb =>
              {
                  pump([
                      gulp.src(path.join(demo, "*.ts")),
                      tsc(
                          {
                              lib: ["dom", "es2017"],
                              target: "es2017",
                              module: "es6",
                              noImplicitAny:true,
                              noUnusedLocals:true,
                              types: ["@types/jquery"]
                          }),
                      gulp.dest(demo)
                  ], cb);
              });

    gulp.task(demoBundleTask(demo), [demoBuildTask(demo)],
              cb => {
                  return rollup.rollup({
                      input: path.join(demo, "Client.js"),
                      external: ["ice"],
                      plugins: [
                          resolve({
                              // pass custom options to the resolve plugin
                              customResolveOptions: {
                                  moduleDirectory: 'node_modules'
                              }
                          })]
                  }).then(bundle => {
                      return bundle.write({
                          file: path.join(demo, "main.js"),
                          format: "iife",
                          globals: {
                              ice: "ice"
                          }
                      });
                  });
              });

    gulp.task(demoSliceTsCleanTask(demo), [],
              cb =>
              {
                  pump([gulp.src(path.join(demo, "*.ice")), extreplace(".d.ts"), paths(del)], cb);
              });

    gulp.task(demoSliceJsCleanTask(demo), [],
              cb =>
              {
                  pump([gulp.src(path.join(demo, "*.ice")), extreplace(".js"), paths(del)], cb);
              });

    gulp.task(demoCleanTask(demo), [demoSliceJsCleanTask(demo), demoSliceTsCleanTask(demo)],
              cb =>
              {
                  pump([gulp.src(path.join(demo, "**/*.js")), paths(del)], cb);
              });
}

gulp.task("demo:build", browserDemos.map(demoBuildTask));
gulp.task("demo:clean", browserDemos.map(demoCleanTask));

gulp.task("build", ["demo:build"]);

gulp.task("clean", ["demo:clean", "dist:clean"]);
gulp.task("default", ["build"]);

gulp.task("run", ["build"],
    function(){
        HttpServer();
        return gulp.src("").pipe(open({uri: "http://127.0.0.1:8080/index.html"}));
    });
