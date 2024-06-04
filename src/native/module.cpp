#include "module.h"
#include "sow.h"

extern "C" {

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