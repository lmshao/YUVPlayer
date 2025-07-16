wxWidgets static library directory
===

Download **wxWidgets-3.2.1.zip** from
 [https://github.com/wxWidgets/wxWidgets/releases/tag/v3.2.1](https://github.com/wxWidgets/wxWidgets/releases/tag/v3.2.1).

Decompress the package and open `wxWidgets-3.2.1\build\msw\wx_vc17.sln` with VS2022.

Compile both `Debug|x64` and `Release|x64` configurations, then move all the generated files from `wxWidgets-3.2.1\lib\vc_x64_lib\` to `YUVPlayer\wxWidgets\lib\vc_x64_lib\`.
