
#!/bin/bash

set -e

echo "🧹 Cleaning previous Ninja build..."
rm -rf ninja_build

echo "📁 Creating build directory..."
mkdir -p ninja_build

echo "🔧 Running CMake with Ninja generator..."
cmake -S . -B ninja_build -G Ninja

echo "⚙️  Building project with Ninja..."
cmake --build ninja_build

echo "✅ Build complete. Running the binary..."
./ninja_build/clicker_plus_plus
