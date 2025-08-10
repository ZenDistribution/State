#!/bin/bash

find ./binaries -regex '.*\.\(hpp\)' -exec clang-format -i {} -style LLVM \;
find ./include -regex '.*\.\(hpp\)' -exec clang-format -i {} -style LLVM \;
find ./src -regex '.*\.\(cpp\)' -exec clang-format -i {} -style LLVM \;
find ./tests -regex '.*\.\(cc\)' -exec clang-format -i {} -style LLVM \;
