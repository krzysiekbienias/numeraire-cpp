#!/bin/bash

echo "🧹 Cleaning existing build..."
rm -rf xcode_build
rm -rf CMakeCache.txt
rm -rf CMakeFiles
rm -rf bin/*

echo "📁 Creating build directory..."
mkdir -p xcode_build
cd xcode_build

echo "🔧 Running CMake with Xcode generator..."
cmake .. -G Xcode

echo "🔨 Building all targets..."
cmake --build . --config Debug

echo "✅ Done. Binaries are in ./bin"
