
import { Type as t, type Static, Value } from "tjs:typebox"


const speeds_t = t.Union([
    t.Integer(),
    t.Literal('B0'), t.Literal('B50'), t.Literal('B75'), t.Literal('B110'), t.Literal('B134'), t.Literal('B150'), t.Literal('B200'), t.Literal('B300'),
    t.Literal('B600'), t.Literal('B1200'), t.Literal('B1800'), t.Literal('B2400'), t.Literal('B9600'), t.Literal('B19200'), t.Literal('B38400'), t.Literal('B57600'),
    t.Literal('B115200'), t.Literal('B230400'), t.Literal('B460800')], { default: 'B9600' });

const serial_cfg_schema = t.Object({
    parity: t.Optional(t.Union([t.Literal("disabled"), t.Literal("enabled")], { default: 'disabled' })),
    bits: t.Optional(t.Union([t.Literal(5), t.Literal(6), t.Literal(7), t.Literal(8)], { default: 8 })),
    stop: t.Optional(t.Union([t.Literal(1), t.Literal(2)], { default: 1 })),
    hardware_flow: t.Optional(t.Union([t.Literal("disabled"), t.Literal("enabled")], { default: 'enabled' })),
    vtime: t.Optional(t.Integer({ default: 0 })),
    vmin: t.Optional(t.Integer({ default: 0 })),
    ispeed: t.Optional(speeds_t),
    ospeed: t.Optional(speeds_t)
}, { additionalProperties: false });
type serial_cfg_t = Static<typeof serial_cfg_schema>

const core = globalThis[Symbol.for('tjs.internal.core')];

/**
 * Open a serial device and lock it.
 * @param file the path for the file mapped serial.
 * @param cfg the configuration of the serial.
 * @returns a normal file handle. Just use it as you normally would.
 */
export function open(file: string, cfg?: serial_cfg_t) : tjs.FileHandle {
    const pcfg = Value.Default(serial_cfg_schema, cfg)
    //@ts-ignore
    const fd = core.$__MODULE__.configure(file, {
        ...cfg,
        ispeed: (typeof cfg.ispeed === "number") ? cfg.ispeed : Number.parseInt(cfg.ispeed.substring(1)),
        ospeed: (typeof cfg.ospeed === "number") ? cfg.ospeed : Number.parseInt(cfg.ospeed.substring(1))
    })
    if(fd!==-1){
        return core.newStdioFile(file,fd);
    }
    else throw new Error(`Unable to use serial connection with [${file}]`);
}

/**
 * 
 * @returns The list of available files mapping serial devices.
 */
export async function list() : Promise<string[]>{
    const names = []
    const dirIter = await tjs.readdir('/dev/serial/by-id/');
    for await (const item of dirIter) {
        names.push(`/dev/serial/by-id/${item.name}`);
    }
    return names;
}

export {
    serial_cfg_schema, type serial_cfg_t,
}