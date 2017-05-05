# hijack-process
[![npm version](https://badge.fury.io/js/hijack-process.svg)](http://badge.fury.io/js/hijack-process)
[![downloads](http://img.shields.io/npm/dm/hijack-process.svg)](https://www.npmjs.com/package/hijack-process)

This package can be used to inspect and manipulate the memory of running
processes. **Please note that most platforms restrict virtual memory access to
priviliged users.**

### Table of Contents
  - [**hijack**](#hijack-id)(*id*) - hijack a process
  - *process*.[**read**](#process-read)(*address*, *size*) - read a byte array

### Documentation
<a name="hijack-id" href="#hijack-id">#</a> **hijack**(*id*)

Attaches to the process with the specified *id*.

``` javascript
var hijack = require('hijack-process');

var thisProcess = hijack(process.pid);
// => { handle: 1234 }
```

<a name="process-read" href="#process-read">#</a> *process*.**read**(*address*, *size*)

Read *size* bytes from the specified *address*.

``` javascript
var hijack = require('hijack-process');

var thisProcess = hijack(process.pid);
var bytes = thisProcess.read(0xDEADBEEF, 8);
// => [ 0, 1, 2, 3, 4, 5, 6, 7 ]
```
