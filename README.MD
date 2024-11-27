Live2D Motion3.json Fix

A small tool to recalculate count of Curve, Segment and Point, and correct them in motion3.json.
Intend to fix Cubism Model with problem like [THIS](https://github.com/Konata09/Live2dOnWeb/issues/6)

Complie it with c++17 enabled, put the executable under your Model folder and run.
It will create motion3.json.bak and fix original files.

The calculate method is from Framework/src/motion/cubismmotion.ts in Cubism SDK for WEB.

You can open an issue if there is any problem.
