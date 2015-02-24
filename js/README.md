## Requirements
Compiling and running the Ice for JavaScript demos requires the following:
- [Node.js](https://nodejs.org)
- Dependencies for [node-gyp](https://github.com/TooTallNate/node-gyp) (Required to build the slice2js compiler).

## Running the scripts
Be sure to run `npm install` first to get any required dependencies.
```bash
$ npm run gulp:build // Build the demos
$ npm run gulp:watch // Run demo web server; watch for files changes and reload
$ npm run gulp:clean // Clean the demos
```

## Running the demos
Running a demo requires running a corresponding (C++, Java, C#) server. For more information refer to the [documentation](https://doc.zeroc.com/display/Ice36/Sample+Programs).
