import {open} from "tjs:serial"
import assert from 'tjs:assert';

//TODO: We should have a virtual serial device as performing this test in hardware is virtually impossible.

/*
const fd = open("/dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0")
console.log(fd)
assert.eq(1,2)
*/