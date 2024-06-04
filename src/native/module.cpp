#include "module.h"
#include "sow.h"

extern "C" {

static void tjs_obj2cfg(JSContext *ctx, JSValue obj, cfg_serial *cfg) {
    /*JSValue parity = JS_GetPropertyStr(ctx, obj, "parity");
    if (!JS_IsUndefined(parity)){
        JS
        JSToCString(ctx, &cfg->parity, family);
    }
    JS_FreeValue(ctx, parity);

    JSValue socktype = JS_GetPropertyStr(ctx, obj, "socktype");
    if (!JS_IsUndefined(socktype))
        JS_ToInt32(ctx, &ai->ai_socktype, socktype);
    JS_FreeValue(ctx, socktype);

    JSValue protocol = JS_GetPropertyStr(ctx, obj, "protocol");
    if (!JS_IsUndefined(protocol))
        JS_ToInt32(ctx, &ai->ai_protocol, protocol);
    JS_FreeValue(ctx, protocol);
    */

    {
        JSValue value = JS_GetPropertyStr(ctx, obj, "vtime");
        uint32_t buffer;
        if (!JS_IsUndefined(value)){
            JS_ToUint32(ctx, &buffer, value);
            cfg->vtime = (uint8_t)buffer;
        }
        JS_FreeValue(ctx, value);
    }

    {
        JSValue value = JS_GetPropertyStr(ctx, obj, "vmin");
        uint32_t buffer;
        if (!JS_IsUndefined(value)){
            JS_ToUint32(ctx, &buffer, value);
            cfg->vtime = (uint8_t)buffer;
        }
        JS_FreeValue(ctx, value);
    }

    {
        JSValue value = JS_GetPropertyStr(ctx, obj, "ispeed");
        if (!JS_IsUndefined(value))
            JS_ToUint32(ctx, &cfg->ispeed, value);
        JS_FreeValue(ctx, value);
    }

    {
        JSValue value = JS_GetPropertyStr(ctx, obj, "ispeed");
        if (!JS_IsUndefined(value))
            JS_ToUint32(ctx, &cfg->ospeed, value);
        JS_FreeValue(ctx, value);
    }
}

static JSValue tjs___MODULE___configure(JSContext *ctx, JSValue this_val, int argc, JSValue *argv) {
    if (!JS_IsString(argv[0]))
    return JS_ThrowTypeError(ctx, "the file name must be a string");

    size_t len;
    const char *name = JS_ToCStringLen(ctx, &len, argv[0]);
    if (!name)
        return JS_EXCEPTION;

    if (!JS_IsObject(argv[1]))
    return JS_ThrowTypeError(ctx, "the configuration must be an object");

    cfg_serial cfg;

    tjs_obj2cfg(ctx,argv[1],&cfg);

    int fd = __MODULE___configure(name,cfg);
    if(fd==-1) return JS_ThrowPlainError(ctx, "Unable to open file [%s] as serial", name);
    return JS_NewInt32(ctx,fd);
}

static const JSCFunctionListEntry tjs___MODULE___funcs[] = {
    TJS_CFUNC_DEF("configure", 2, tjs___MODULE___configure),
};

void tjs__mod___MODULE___init(JSContext *ctx, JSValue ns) {
    JSRuntime *rt = JS_GetRuntime(ctx);

    JSValue extmod = JS_NewObjectProto(ctx, JS_NULL);
    JS_SetPropertyFunctionList(ctx, extmod, tjs___MODULE___funcs, countof(tjs___MODULE___funcs));
    JS_DefinePropertyValueStr(ctx, ns, "$__MODULE__", extmod, 0);
}

}