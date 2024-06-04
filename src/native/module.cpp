#include "module.h"
#include "sow.h"

using namespace __MODULE__;

extern "C" {

//Most checks are skipped since the json schema is validated before running this.
//So structure is assumed to be enforced.
static void tjs_obj2cfg(JSContext *ctx, JSValue obj, cfg_serial *cfg) {
    {
        JSValue value = JS_GetPropertyStr(ctx, obj, "parity");
        const char* buffer;
        if (!JS_IsUndefined(value)){
            buffer = JS_ToCString(ctx, value);
            if(strcmp("enabled",buffer)==0) cfg->parity = feature_t::ENABLED;
            else if(strcmp("disabled",buffer)==0) cfg->parity = feature_t::DISABLED;
            JS_FreeCString(ctx, buffer);
        }
        JS_FreeValue(ctx, value);
    }

    {
        JSValue value = JS_GetPropertyStr(ctx, obj, "bits");
        uint32_t buffer;
        if (!JS_IsUndefined(value)){
            JS_ToUint32(ctx, &buffer, value);
            cfg->bits = static_cast<bits_t>((uint8_t)buffer-5);
        }
        JS_FreeValue(ctx, value);
    }


    {
        JSValue value = JS_GetPropertyStr(ctx, obj, "stop");
        uint32_t buffer;
        if (!JS_IsUndefined(value)){
            JS_ToUint32(ctx, &buffer, value);
            cfg->stop = static_cast<stop_t>((uint8_t)buffer-1);
        }
        JS_FreeValue(ctx, value);
    }

    {
        JSValue value = JS_GetPropertyStr(ctx, obj, "hardware_flow");
        const char* buffer;
        if (!JS_IsUndefined(value)){
            buffer = JS_ToCString(ctx, value);
            if(strcmp("enabled",buffer)==0) cfg->hardware_flow = feature_t::ENABLED;
            else if(strcmp("disabled",buffer)==0) cfg->hardware_flow = feature_t::DISABLED;
            JS_FreeCString(ctx, buffer);
        }
        JS_FreeValue(ctx, value);
    }

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
            cfg->vmin = (uint8_t)buffer;
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
        JSValue value = JS_GetPropertyStr(ctx, obj, "ospeed");
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

    int fd = configure(name,cfg);
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