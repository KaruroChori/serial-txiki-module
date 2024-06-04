import {open} from "tjs:serial"
import assert from 'tjs:assert';

const fd = open("/dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0")
console.log(fd)
assert.eq(1,2)