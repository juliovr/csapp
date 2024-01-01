@echo off

set source_file=%1
set executable=%2

IF NOT EXIST bin mkdir bin
pushd bin

cl -Zi -O2 -MD /D "TBB_USE_THREADING_TOOLS" /DEBUG ../%source_file% && %executable%

popd
