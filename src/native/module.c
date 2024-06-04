#include "module.h"
#include "../../utils.h"

static JSValue tjs___MODULE___configure(JSContext *ctx, JSValue this_val, int argc, JSValue *argv) {
    printf("Banananaaaaaaaaaaaaa");
    return JS_UNDEFINED;
}

static const JSCFunctionListEntry tjs___MODULE___funcs[] = {
    TJS_CFUNC_DEF("$__MODULE__configure", 2, tjs___MODULE___configure),
};

void tjs__mod___MODULE___init(JSContext *ctx, JSValue ns) {
    JSRuntime *rt = JS_GetRuntime(ctx);

    JS_SetPropertyFunctionList(ctx, ns, tjs___MODULE___funcs, countof(tjs___MODULE___funcs));
}