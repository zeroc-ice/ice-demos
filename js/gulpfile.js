// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

var babel       = require("gulp-babel"),
    bower       = require("bower"),
    browserSync = require("browser-sync"),
    concat      = require("gulp-concat"),
    del         = require("del"),
    extreplace  = require("gulp-ext-replace"),
    fs          = require("fs"),
    gulp        = require("gulp"),
    gzip        = require("gulp-gzip"),
    iceBuilder  = require("gulp-ice-builder"),
    minifycss   = require("gulp-minify-css"),
    newer       = require("gulp-newer"),
    npm         = require("npm"),
    open        = require("gulp-open"),
    path        = require("path"),
    paths       = require("vinyl-paths"),
    uglify      = require("gulp-uglify"),
    HttpServer  = require("./bin/HttpServer");

//
// Check ICE_HOME environment variable. If this is set and points to a source
// distribution then prefer it over default packages.
//
var ICE_HOME;
if(process.env.ICE_HOME)
{
    ICE_HOME = fs.existsSync(path.join(process.env.ICE_HOME, "js", "package.json")) ? process.env.ICE_HOME : undefined;
}

function slice2js(options)
{
    var defaults = {};
    var opts = options || {};

    defaults.args = opts.args || [];
    defaults.dest = opts.dest;
    if(ICE_HOME)
    {
        defaults.exe = path.join(ICE_HOME, 'cpp', 'bin', process.platform == "win32" ? "slice2js.exe" : "slice2js");
        defaults.args = defaults.args.concat(["-I" + path.join(ICE_HOME, 'slice')]);
    }
    return iceBuilder.compile(defaults);
}

var common =
{
    "scripts":
    [
        "bower_components/foundation/js/vendor/modernizr.js",
        "bower_components/foundation/js/vendor/jquery.js",
        "bower_components/foundation/js/foundation.min.js",
        "bower_components/nouislider/distribute/jquery.nouislider.all.js",
        "bower_components/animo.js/animo.js",
        "bower_components/spin.js/spin.js",
        "bower_components/spin.js/jquery.spin.js",
        "bower_components/highlightjs/highlight.pack.js",
        "assets/icejs.js"
    ],
    "styles":
    [
        "bower_components/foundation/css/foundation.css",
        "bower_components/animo.js/animate-animo.css",
        "bower_components/highlightjs/styles/vs.css",
        "bower_components/nouislider/distribute/jquery.nouislider.min.css",
        "assets/icejs.css"
    ]
};

//
// Deploy Ice for JavaScript browser libraries from bower or from ICE_HOME depending
// on whenever or not ICE_HOME is set.
//
gulp.task("dist:libs", ["npm", "bower"],
    function()
    {
        var libs = ICE_HOME ? path.join(ICE_HOME, 'js', 'lib', '*') : 'bower_components/ice/lib/*';
        return gulp.src([libs])
            .pipe(newer("lib"))
            .pipe(gulp.dest("lib"));
    });

gulp.task("dist:libs-es5", ["npm", "bower"],
    function()
    {
        var libs = ICE_HOME ? path.join(ICE_HOME, 'js', 'lib', 'es5', '*') : 'bower_components/ice/lib/es5/*';
        return gulp.src([libs])
            .pipe(newer("lib/es5"))
            .pipe(gulp.dest("lib/es5"));
    });

gulp.task("dist:clean", [],
    function()
    {
        del(["lib/*", "lib/es5/*"]);
    });

//
// If ICE_HOME is set we install Ice for JavaScript module from ICE_HOME
// that is required for running JavaScript NodeJS demos.
//
gulp.task("npm", [],
    function(cb)
    {
        npm.load({loglevel: 'silent'}, function(err, npm) {
            if(ICE_HOME)
            {
                npm.commands.install([path.join(ICE_HOME, 'js')], function(err, data)
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

gulp.task("bower", ["npm"],
    function(cb)
    {
        bower.commands.install().on("end", function(){ cb(); });
    });

gulp.task("common:js", ["bower"],
    function()
    {
        return gulp.src(common.scripts)
            .pipe(newer("assets/common.min.js"))
            .pipe(concat("common.min.js"))
            .pipe(uglify())
            .pipe(gulp.dest("assets"))
            .pipe(gzip())
            .pipe(gulp.dest("assets"));
    });

gulp.task("common:js:watch", ["common:js"],
    function()
    {
        gulp.watch(common.scripts, function(e){
            gulp.start("common:js", function(){
                browserSync.reload(e.path);
            });
        });
    });

gulp.task("common:css", ["bower"],
    function()
    {
        return gulp.src(common.styles)
            .pipe(newer("assets/common.css"))
            .pipe(concat("common.css"))
            .pipe(minifycss())
            .pipe(gulp.dest("assets"))
            .pipe(gzip())
            .pipe(gulp.dest("assets"));
    });

gulp.task("common:css:watch", ["common:css"],
    function()
    {
        gulp.watch(common.styles, function(e){
            gulp.start("common:css", function(){
                browserSync.reload(e.path);
            });
        });
    });

gulp.task("common:clean", [],
    function()
    {
        del(["assets/common.css", "assets/common.min.js"]);
    });

var demos = [
    "Ice/hello",
    "Ice/throughput",
    "Ice/minimal",
    "Ice/latency",
    "Ice/bidir",
    "Glacier2/simpleChat",
    "Chat"];

function demoTaskName(name) { return "demo_" + name.replace("/", "_"); }
function demoES5IceTask(name) { return demoTaskName(name) + ":ice-es5"; }
function demoBabelTask(name) { return demoTaskName(name) + ":babel"; }
function demoBrowserBabelTask(name) { return demoTaskName(name) + "-browser:babel"; }
function demoBuildTask(name) { return demoTaskName(name) + ":build"; }
function demoWatchTask(name) { return demoTaskName(name) + ":watch"; }
function demoDependCleanTask(name) { return demoTaskName(name) + "-depend:clean"; }
function demoCleanTask(name) { return demoTaskName(name) + ":clean"; }
function demoBabelCleanTask(name) { return demoTaskName(name) + ":babel-clean"; }
function demoGeneratedFile(file){ return path.join(path.basename(file, ".ice") + ".js"); }

demos.forEach(
    function(name)
    {
        gulp.task(demoTaskName(name), ['dist:libs', 'dist:libs-es5'],
            function()
            {
                return gulp.src(path.join(name, "*.ice"))
                    .pipe(slice2js({args: ["-I" + name], dest: name}))
                    .pipe(gulp.dest(name));
            });

        gulp.task(demoBabelTask(name), [demoTaskName(name)],
            function()
            {
                return gulp.src(path.join(name, "*.js"))
                    .pipe(babel({compact:false}))
                    .pipe(gulp.dest(path.join(name, "es5")));
            });

        const depends = [demoBabelTask(name)];

        if(fs.existsSync(path.join(name, "browser")))
        {
            gulp.task(demoBrowserBabelTask(name), [demoTaskName(name)],
                function()
                {
                    return gulp.src(path.join(name, "browser", "*.js"))
                        .pipe(babel({compact:false}))
                        .pipe(gulp.dest(path.join(name, "browser", "es5")));
                });
            depends.push(demoBrowserBabelTask(name));
        }

        gulp.task(demoES5IceTask(name), [demoBabelTask(name)],
            function()
            {
                return gulp.src(['node_modules/ice/src/es5/**/*']).pipe(
                    gulp.dest(path.join(name, "es5", "node_modules")));
            });
        depends.push(demoES5IceTask(name));

        gulp.task(demoBuildTask(name), depends, function(){});

        gulp.task(demoWatchTask(name), [demoBuildTask(name)],
            function()
            {
                gulp.watch(path.join(name, "*.ice"), [demoTaskName(name)]);

                gulp.watch([path.join(name, "*.js"),
                            path.join(name, "browser", "*.js"),
                            path.join(name, "*.html")],
                    function(e){
                        browserSync.reload(e.path);
                    });
            });

        gulp.task(demoBabelCleanTask(name), [],
            function()
            {
                gulp.src([path.join(name, "es5", "*.js"),
                         path.join(name, "es5", "node_modules"),
                          path.join(name, "browser", "es5", "*.js")])
                    .pipe(paths(del));
            })

        gulp.task(demoDependCleanTask(name), [],
            function()
            {
                gulp.src(path.join(name, ".depend")).pipe(paths(del));
            });

        gulp.task(demoCleanTask(name), [demoDependCleanTask(name), demoBabelCleanTask(name)],
            function()
            {
                gulp.src(path.join(name, "*.ice"))
                    .pipe(extreplace(".js"))
                    .pipe(paths(del));
            });
    });

var minDemos =
{
    "Ice/minimal":
    {
        srcs: [
            "lib/Ice.min.js",
            "Ice/minimal/Hello.js",
            "Ice/minimal/browser/Client.js"],
        dest: "Ice/minimal/browser/"
    },
    "Chat":
    {
        srcs: [
            "lib/Ice.min.js",
            "lib/Glacier2.min.js",
            "Chat/Chat.js",
            "Chat/ChatSession.js",
            "Chat/Client.js"],
        dest: "Chat"
    }
};

function minDemoTaskName(name) { return demoTaskName(name) + ":min"; }
function minDemoWatchTaskName(name) { return minDemoTaskName(name) + ":watch"; }
function minDemoCleanTaskName(name) { return minDemoTaskName(name) + ":clean"; }

Object.keys(minDemos).forEach(
    function(name)
    {
        var demo = minDemos[name];

        gulp.task(minDemoTaskName(name), [demoTaskName(name), "dist:libs", "common:css", "common:js"],
            function()
            {
                return gulp.src(demo.srcs)
                    .pipe(newer(path.join(demo.dest, "Client.min.js")))
                    .pipe(concat("Client.min.js"))
                    //.pipe(uglify()) TODO no es6 support in uglify
                    .pipe(gulp.dest(demo.dest))
                    .pipe(gzip())
                    .pipe(gulp.dest(demo.dest));
            });

        gulp.task(minDemoWatchTaskName(name), [minDemoTaskName(name)],
            function()
            {
                gulp.watch(demo.srcs, [minDemoTaskName(name)]);
            });

        gulp.task(minDemoCleanTaskName(name), [],
            function()
            {
                del([path.join(demo.dest, "Client.min.js"),
                     path.join(demo.dest, "Client.min.js.gz")]);
            });
    });

gulp.task("demo:build", demos.map(demoBuildTask).concat(Object.keys(minDemos).map(minDemoTaskName)));
gulp.task("demo:watch", demos.map(demoWatchTask).concat(Object.keys(minDemos).map(minDemoWatchTaskName)));
gulp.task("demo:clean", demos.map(demoCleanTask).concat(Object.keys(minDemos).map(minDemoCleanTaskName)));

gulp.task("build", ["demo:build"]);

gulp.task("watch", ["demo:build", "dist:libs", "demo:watch", "common:css:watch", "common:js:watch"],
    function()
    {
        browserSync();
        HttpServer();
        return gulp.src("./index.html").pipe(open("", {url: "http://127.0.0.1:8080/index.html"}));
    });
gulp.task("clean", ["demo:clean", "common:clean", "dist:clean"]);
gulp.task("default", ["build"]);
