#!/bin/bash

# Running clang over files
find ./include -regex '.*\.\(hpp\)' -exec clang-format -i {} -style LLVM \;
find ./src -regex '.*\.\(cpp\)' -exec clang-format -i {} -style LLVM \;
find ./tests -regex '.*\.\(cc\)' -exec clang-format -i {} -style LLVM \;
