
import { Type as t, type Static } from "tjs:typebox"

const speeds_t = t.Union([
    t.Integer(),
    t.Literal('B0'), t.Literal('B50'), t.Literal('B75'), t.Literal('B110'), t.Literal('B134'), t.Literal('B150'), t.Literal('B200'), t.Literal('B300'),
    t.Literal('B600'), t.Literal('B1200'), t.Literal('B1800'), t.Literal('B2400'), t.Literal('B9600'), t.Literal('B19200'), t.Literal('B38400'), t.Literal('B57600'),
    t.Literal('B115200'), t.Literal('B230400'), t.Literal('B460800')], { default: 'B9600' });

const serial_cfg_schema = t.Object({
    parity: t.Union([t.Literal("disabled"), t.Literal("enabled")], { default: 'disabled' }),
    stop: t.Union([t.Literal(1), t.Literal(2)], { default: 1 }),
    hardware_flow: t.Union([t.Literal("disabled"), t.Literal("enabled")], { default: 'enabled' }),
    vtime: t.Integer({ default: 0 }),
    vmin: t.Integer({ default: 0 }),
    ispeed: speeds_t,
    ospeed: speeds_t
});
type serial_cfg_t = Static<typeof serial_cfg_schema>

export function configure(file: string, cfg: serial_cfg_t) { }


export {
    serial_cfg_schema
}