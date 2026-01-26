//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import crypto from "crypto";
import fs from "fs";
import hogan from "hogan.js";
import http from "http";
import httpProxy from "http-proxy";
import https from "https";
import path from "path";
import url from "url";
import { fileURLToPath } from "url";

const __dirname = path.dirname(fileURLToPath(import.meta.url));

function Init()
{
    const MimeTypes =
    {
        css: "text/css",
        html: "text/html",
        ico: "image/x-icon",
        js: "text/javascript",
    };

    const libraries = ["/lib/Ice.js",
                     "/lib/Glacier2.js",
                     "/lib/IceStorm.js",
                     "/lib/IceGrid.js"];

    const DemoData = {
        cssDeps: [
            "https://cdnjs.cloudflare.com/ajax/libs/foundation/5.5.3/css/foundation.min.css",
            "https://cdnjs.cloudflare.com/ajax/libs/highlight.js/9.12.0/styles/vs.min.css",
            "https://cdnjs.cloudflare.com/ajax/libs/noUiSlider/7.0.10/jquery.nouislider.min.css"
        ],
        jsDeps: [
            "https://cdnjs.cloudflare.com/ajax/libs/jquery/2.2.4/jquery.min.js",
            "https://cdnjs.cloudflare.com/ajax/libs/foundation/5.5.3/js/foundation.min.js",
            "https://cdnjs.cloudflare.com/ajax/libs/highlight.js/9.12.0/highlight.min.js",
            "https://cdnjs.cloudflare.com/ajax/libs/modernizr/2.8.3/modernizr.min.js",
            "https://cdnjs.cloudflare.com/ajax/libs/noUiSlider/7.0.10/jquery.nouislider.all.min.js",
            "https://cdnjs.cloudflare.com/ajax/libs/spin.js/2.3.2/spin.min.js",
            "https://cdnjs.cloudflare.com/ajax/libs/URI.js/1.18.12/URI.min.js",
            "/assets/jquery.spin.js"
        ]
    }

    class HttpServer {
        constructor(host, ports) {
            this._host = host;
            this._ports = ports;
            this._basePath = path.resolve(path.join(__dirname, ".."));
        }
        processRequest(req, res) {
            let filePath = path.resolve(path.join(this._basePath, req.url.pathname));

            if (req.url.pathname.match("/index\.html$")) {

                fs.stat(filePath,
                    function (err, stats) {
                        if (err) {
                            if (err.code === "ENOENT") {
                                res.writeHead(404);
                                res.end("404 Page Not Found");
                                console.log("HTTP/404 (Page Not Found)" + req.method + " " + req.url.pathname + " -> " + filePath);
                            }

                            else {
                                res.writeHead(500);
                                res.end("500 Internal Server Error");
                                console.log("HTTP/500 (Internal Server Error) " + req.method + " " + req.url.pathname + " -> " +
                                    filePath);
                            }
                        }

                        else {
                            let template = hogan.compile(fs.readFileSync(filePath, "utf8"));
                            res.writeHead(200, { "Content-Type": "text/html" });
                            res.end(template.render(DemoData));
                        }
                    });
                return;
            }

            let ext = path.extname(filePath).slice(1);

            //
            // When the browser ask for a .js or .css file and it has support for gzip content
            // check if a gzip version (.js.gz or .css.gz) of the file exists and use that instead.
            //
            if ((ext == "js" || ext == "css" || ext == "map") && req.headers["accept-encoding"].indexOf("gzip") !== -1) {
                fs.stat(filePath + ".gz",
                    function (err, stats) {
                        if (err || !stats.isFile()) {
                            fs.stat(filePath,
                                function (err, stats) {
                                    doRequest(err, stats, filePath);
                                });
                        }

                        else {
                            doRequest(err, stats, filePath + ".gz");
                        }
                    });
            }

            else {
                fs.stat(filePath,
                    function (err, stats) {
                        doRequest(err, stats, filePath);
                    });
            }

            const doRequest = function (err, stats, filePath) {
                if (err) {
                    if (err.code === "ENOENT") {
                        res.writeHead(404);
                        res.end("404 Page Not Found");
                        console.log("HTTP/404 (Page Not Found)" + req.method + " " + req.url.pathname + " -> " + filePath);
                    }

                    else {
                        res.writeHead(500);
                        res.end("500 Internal Server Error");
                        console.log("HTTP/500 (Internal Server Error) " + req.method + " " + req.url.pathname + " -> " +
                            filePath);
                    }
                }

                else {
                    if (!stats.isFile()) {
                        res.writeHead(403);
                        res.end("403 Forbiden");
                        console.log("HTTP/403 (Forbiden) " + req.method + " " + req.url.pathname + " -> " + filePath);
                    }

                    else {
                        //
                        // Create a md5 using the stats attributes
                        // to be used as Etag header.
                        //
                        const hash = crypto.createHash("md5");
                        hash.update(stats.ino.toString());
                        hash.update(stats.mtime.toString());
                        hash.update(stats.size.toString());

                        const headers = {
                            "Content-Type": MimeTypes[ext] || "text/plain",
                            "Content-Length": stats.size,
                            "Etag": hash.digest("hex")
                        };

                        if (path.extname(filePath).slice(1) == "gz") {
                            headers["Content-Encoding"] = "gzip";
                        }

                        //
                        // Check for conditional request header if-none-match.
                        //
                        let modified = true;
                        if (req.headers["if-none-match"] !== undefined) {
                            modified = req.headers["if-none-match"].split(" ").every(
                                function (element, index, array) {
                                    return element !== headers.Etag;
                                });
                        }

                        //
                        // Not Modified
                        //
                        if (!modified) {
                            res.writeHead(304, headers);
                            res.end();
                            console.log("HTTP/304 (Not Modified) " + req.method + " " + req.url.pathname + " -> " + filePath);
                        }

                        else {
                            res.writeHead(200, headers);
                            if (req.method === "HEAD") {
                                res.end();
                            }

                            else {
                                fs.createReadStream(filePath, { "bufferSize": 4 * 1024 }).pipe(res);
                            }
                            console.log("HTTP/200 (Ok) " + req.method + " " + req.url.pathname + " -> " + filePath);
                        }
                    }
                }
            };
        }
        start() {
            const options = {
                passphrase: "password",
                pfx: fs.readFileSync(path.join(__dirname, "../../../certs", "server.p12")),
            };

            const httpServer = http.createServer();
            const httpsServer = https.createServer(options);

            if (httpProxy) {
                proxyConfig.forEach(
                    function (conf) {
                        proxies[conf.resource] = {
                            server: httpProxy.createProxyServer({ target: conf.target, secure: false }),
                            protocol: conf.protocol
                        };
                    });
            }

            const self = this;
            [httpServer, httpsServer].forEach(function (server) {
                server.on("request", function (req, res) {
                    //
                    // Dummy data callback required so request end event is emitted.
                    //
                    const dataCB = function (data) {
                    };

                    const endCB = function () {
                        req.url = url.parse(req.url);
                        self.processRequest(req, res);
                    };

                    req.on("data", dataCB);
                    req.on("end", endCB);
                });
            });

            if (httpProxy) {
                const requestCB = function (protocols) {
                    return function (req, socket, head) {
                        const errCB = function (err) {
                            socket.end();
                        };
                        const proxy = proxies[req.url];
                        if (proxy && protocols.indexOf(proxy.protocol) !== -1) {
                            proxy.server.ws(req, socket, head, errCB);
                        }

                        else {
                            socket.end();
                        }
                    };
                };

                httpServer.on("upgrade", requestCB(["ws"]));
                httpsServer.on("upgrade", requestCB(["ws", "wss"]));
            }

            httpServer.listen(8080, this._host);
            httpsServer.listen(9090, this._host);
            console.log("listening on ports 8080 (http) and 9090 (https)...");
        }
    }


    //
    // Proxy configuration for the different demos.
    //
    const proxyConfig = [
        {resource: "/demows", target: "http://localhost:10002", protocol: "ws"},
        {resource: "/demowss", target: "https://localhost:10003", protocol: "wss"},
        {resource: "/chatws", target: "http://localhost:5063", protocol: "ws"},
        {resource: "/chatwss", target: "https://localhost:5064", protocol: "wss"}
    ];

    const proxies = {};


    new HttpServer("0.0.0.0", [8080, 9090]).start();
}

export default Init;
