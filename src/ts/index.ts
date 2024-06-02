
import { Type as t, Static } from "tjs:typebox"
import { program } from "tjs:commander"
enum parity_t { DISABLED = 0, ENABLED };
enum stop_t { ONE_BIT = 0, TWO_BITS };
enum bits_t { FIVE = 0, SIX, SEVEN, EIGHT };
enum hardware_flow_t { DISABLED = 0, ENABLED };
enum speeds { B0 = 0, B50, B75, B110, B134, B150, B200, B300, B600, B1200, B1800, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800 }

const serial_cfg_schema = t.Object({});
type serial_cfg_t = Static<typeof serial_cfg_schema>

export function configure(file: string, cfg: serial_cfg_t) { }

export function hello() {
    console.log('Hello world!')
}

export {
    serial_cfg_schema
}