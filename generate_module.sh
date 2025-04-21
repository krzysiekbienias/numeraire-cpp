#!/bin/bash

# Check for required arguments
if [ "$#" -ne 2 ]; then
  echo "❌ Usage: ./generate_module.sh <folder> <name>"
  echo "   Example: ./generate_module.sh arrays max_element"
  exit 1
fi

FOLDER=$1
NAME=$2

# File paths
CPP_FILE="src/${FOLDER}/${NAME}.cpp"
HEADER_FILE="include/${FOLDER}/${NAME}.hpp"
TEST_FILE="unit_tests/${FOLDER}/test_${NAME}.cpp"

# Create directories
mkdir -p "src/${FOLDER}"
mkdir -p "include/${FOLDER}"
mkdir -p "unit_tests/${FOLDER}"

# Create .cpp file
cat > "$CPP_FILE" <<EOF
#include "${NAME}.hpp"

// Implement your ${NAME} logic here.
EOF

# Create .hpp file
cat > "$HEADER_FILE" <<EOF
#pragma once

// Declare your ${NAME} interface here.
EOF

# Create test file
cat > "$TEST_FILE" <<EOF
#include "${NAME}.hpp"
#include <gtest/gtest.h>

TEST(${NAME^}Test, SampleTest) {
    // TODO: Write test for ${NAME}
    EXPECT_TRUE(true);
}
EOF

echo "✅ Created:"
echo " - $CPP_FILE"
echo " - $HEADER_FILE"
echo " - $TEST_FILE"
