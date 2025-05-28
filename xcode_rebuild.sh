#!/bin/bash

echo "🧹 Cleaning existing build..."
rm -rf xcode_build

echo "📁 Creating build directory..."
mkdir -p xcode_build

echo "🔧 Running CMake with Xcode generator..."
cmake -S . -B xcode_build -G Xcode -DCMAKE_OSX_DEPLOYMENT_TARGET=15.3

echo "✅ CMake configuration complete. You can now run:"
echo "   open xcode_build/*.xcodeproj"

