module.exports = {
    "extends": [
        "../.eslintrc.js"
    ],
    "env": {
        "browser": true,
        "es6": true,
        "node": false
    },
    "globals": {
        "Ice": true,
        "Demo": true,
        "$": true,
        "URI": true,
        "hljs": true
    },
    "rules":{
        "no-implicit-globals": "off"
    }
}
