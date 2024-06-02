#!/bin/env bun
import { $ } from "bun"

await $`rm -rf ./dist`

await $`mkdir -p ./dist/src`
await $`mkdir -p ./dist/tests`
await $`mkdir -p ./dist/examples`
await $`mkdir -p ./dist/benchmarks`
await $`mkdir -p ./dist/bundle`

//JS
try {
  const t = await Bun.build({
    entrypoints: ['./src/ts/index.ts'],
    outdir: './dist/bundle/',
    external: ["tjs:*"],
    target: "bun",
  })
  await $`mv ./dist/bundle/index.js ./dist/bundle/[module].js`
}
catch (e) { console.error(e) }

//D.TS
await $`./node_modules/.bin/dts-bundle-generator -o ./dist/bundle/[module].d.ts ./src/ts/index.ts --no-check --export-referenced-types=false`

const old = await Bun.file("./dist/bundle/[module].d.ts").text()
await Bun.write("./dist/bundle/[module].d.ts", `
/**
 * Typebox Txiki module interface
 *
 * @module tjs:__MODULE__
 */
declare module 'tjs:__MODULE__' {
  ${old.replaceAll('declare', '')}
}

`)

//Additional files
await $`cp ./LICENCE ./dist/LICENCE`
await $`cp ./module.json ./dist/module.json`

/*
//C entrypoint
await Bun.write("./dist/src/module.c", `#include "module.h"
#include " ../../utils.h"

void tjs__mod___MODULE___init(JSContext *ctx, JSValue ns) {
}`)

//H entrypoint
await Bun.write("./dist/src/module.h", `#pragma once

#include "../../private.h"
#include "../../utils.h"

#ifdef __cplusplus
extern "C" {
#endif
void tjs__mod___MODULE___init(JSContext *ctx, JSValue ns);
#ifdef __cplusplus
}
#endif`)
*/

//Rest of native
await $`if [ -d ./src/native/ ]; then cp -r ./src/native/* ./dist/src; fi`

//Tests & benchmarks
await $`if [ -d ./tests/ ]; then cp -r ./tests/* ./dist/tests; fi`
await $`if [ -d ./benchmarks/ ]; then cp -r ./benchmarks/* ./dist/benchmarks; fi`
await $`if [ -d ./examples/ ]; then cp -r ./examples/* ./dist/examples; fi`
