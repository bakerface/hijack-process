# hijack-process
[![npm version](https://badge.fury.io/js/hijack-process.svg)](http://badge.fury.io/js/hijack-process)
[![downloads](http://img.shields.io/npm/dm/hijack-process.svg)](https://www.npmjs.com/package/hijack-process)

This package can be used to inspect and manipulate the memory of running
processes. **Please note that most platforms restrict virtual memory access to
priviliged users.**

### Table of Contents
  - [#](#hijack-id) **hijack**(*id*) - hijack a process


### Documentation
<a name="hijack-id" href="#hijack-id">#</a> **hijack**(*id*)

Attaches to the process with the specified *id*.

``` javascript
var hijack = require('hijack-process');

var thisProcess = hijack(process.pid);
// => { handle: 1234 }
```
