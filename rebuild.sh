#!/bin/bash
echo "🧹 Cleaning existing build..."
rm -rf xcode_build

echo "📁 Creating build directory..."
mkdir -p xcode_build

echo "🔧 Running CMake with Xcode generator and x86_64 arch..."
cmake -S . -B xcode_build \
    -G Xcode \
    -DCMAKE_OSX_DEPLOYMENT_TARGET=13.0 \
    -DCMAKE_OSX_ARCHITECTURES=x86_64

echo "✅ CMake configuration complete. You can now run:"
echo "   open xcode_build/*.xcodeproj"
