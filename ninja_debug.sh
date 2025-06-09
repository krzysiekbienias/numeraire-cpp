#!/bin/bash

set -e

echo "🧹 Cleaning previous Ninja build..."
rm -rf ninja_build

echo "📁 Creating build directory..."
mkdir -p ninja_build

echo "🔧 Running CMake with Debug flags and Ninja generator..."
cmake -S . -B ninja_build -G Ninja -DCMAKE_BUILD_TYPE=Debug

echo "⚙️  Building project with Ninja..."
cmake --build ninja_build

echo "🐞 Launching LLDB..."
lldb ./ninja_build/clicker_plus_plus
