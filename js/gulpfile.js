// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

/* eslint no-sync: "off" */
/* eslint no-process-env: "off" */
/* eslint no-process-exit: "off" */
/* eslint no-shadow: "off" */

const babel = require("gulp-babel");
const concat = require("gulp-concat");
const del = require("del");
const fs = require("fs");
const gulp = require("gulp");
const gzip = require("gulp-gzip");
const iceBuilder = require("gulp-ice-builder");
const newer = require("gulp-newer");
const open = require("gulp-open");
const path = require("path");
const paths = require("vinyl-paths");
const pump = require("pump");
const terser = require("gulp-terser");
const httpServer = require("./bin/HttpServer");

const root = path.resolve(path.join('__dirname', '..'));
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
    const defaults = {};
    const opts = options || {};

    defaults.args = opts.args || [];
    defaults.include = opts.include || [];
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
    return iceBuilder(defaults);
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

gulp.task("common:es5",
          cb =>
          {
              pump([gulp.src([path.join(root, "assets", "common.js")]),
                    babel({compact: false}),
                    gulp.dest(path.join(root, "assets", "es5"))], cb);
          });

gulp.task("common:es5:clean",
          cb =>
          {
              pump([gulp.src([path.join(root, "assets", "es5")], {allowEmpty: true}),
                    paths(del)], cb);
          });

const demos =
{
    "Chat":
    {
        srcs: [
            "node_modules/ice/lib/Ice.min.js",
            "node_modules/ice/lib/Glacier2.min.js",
            "Chat/generated/Chat.js",
            "Chat/generated/ChatSession.js",
            "Chat/Client.js"],
        dest: "Chat"
    },
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

const demoBabelTask = name => demoTaskName(name) + ":babel";
const demoGeneratedBabelTask = name => demoTaskName(name) + "-generated:babel";
const demoBrowserBabelTask = name => demoTaskName(name) + "-browser:babel";
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

    gulp.task(demoGeneratedBabelTask(name),
              cb =>
              {
                  pump([
                      gulp.src([path.join(root, name, "generated", "*.js")]),
                      babel({compact: false}),
                      gulp.dest(path.join(root, name, "es5", "generated"))], cb);
              });

    gulp.task(demoBabelTask(name),
              cb =>
              {
                  pump([
                      gulp.src([path.join(root, name, "*.js")]),
                      babel({compact: false}),
                      gulp.dest(path.join(root, name, "es5"))], cb);
              });

    if(fs.existsSync(path.join(name, "browser")))
    {
        gulp.task(demoBrowserBabelTask(name),
                  cb =>
                  {
                      pump([
                          gulp.src(path.join(root, name, "browser", "*.js")),
                          babel({compact: false}),
                          gulp.dest(path.join(root, name, "es5", "browser"))], cb);
                  });
    }
    else
    {
        gulp.task(demoBrowserBabelTask(name),
                  cb =>
                  {
                      cb();
                  });
    }

    gulp.task(demoCleanTask(name),
              cb =>
              {
                  pump([gulp.src([path.join(root, name, ".depend"),
                                  path.join(root, name, "es5"),
                                  path.join(root, name, "browser", "es5"),
                                  path.join(root, name, "generated")], {allowEmpty: true}),
                        gulp.src([path.join(root, demo.dest, "Client.min.js"),
                                  path.join(root, demo.dest, "Client.min.js.gz")], {allowEmpty: true}),
                        paths(del)], cb);
              });

    gulp.task(demoBuildTask(name),
              gulp.series(demoSlice2JsTask(name),
                          demoMinTask(name),
                          demoGeneratedBabelTask(name),
                          demoBabelTask(name),
                          demoBrowserBabelTask(name)));
}

gulp.task("build", gulp.series("npm", "common:es5", gulp.parallel(Object.keys(demos).map(demoBuildTask))));

gulp.task("clean", gulp.parallel("common:es5:clean", Object.keys(demos).map(demoCleanTask)));

gulp.task("run", gulp.series("build",
                             () =>
                             {
                                 httpServer();
                                 return gulp.src(__filename).pipe(open({uri: "http://127.0.0.1:8080/index.html"}));
                             }));

gulp.task("default", gulp.series("build"));
