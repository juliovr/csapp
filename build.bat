@echo off

set source_file=%1
set executable=%2

IF NOT EXIST bin mkdir bin
pushd bin

cl -Z7 ../%source_file% && %executable%

popd
