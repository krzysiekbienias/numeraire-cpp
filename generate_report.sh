#!/bin/bash

# Default base directory is ./src or passed as argument
BASE_DIR=${1:-"./src"}
REPORT_NAME="solved_problems_report.md"

# Check if base directory exists
if [[ ! -d "$BASE_DIR" ]]; then
    echo "Error: Directory '$BASE_DIR' does not exist."
    exit 1
fi

# Absolute paths
BASE_DIR_ABS=$(realpath "$BASE_DIR")
PARENT_DIR=$(dirname "$BASE_DIR_ABS")
OUTPUT_FILE="$PARENT_DIR/$REPORT_NAME"

# Start report
echo "# Solved Problems Report" > "$OUTPUT_FILE"
echo "" >> "$OUTPUT_FILE"

# Loop through each subfolder
for dir in "$BASE_DIR_ABS"/*/; do
    [ -d "$dir" ] || continue
    folder_name=$(basename "$dir")
    capitalized_folder_name=$(echo "$folder_name" | awk '{print toupper(substr($0,1,1)) tolower(substr($0,2))}')

    echo "" >> "$OUTPUT_FILE"
    echo "## $capitalized_folder_name" >> "$OUTPUT_FILE"
    echo "" >> "$OUTPUT_FILE"

    # Get .cpp files in this folder
    files=("$dir"/*.cpp)
    if [ ${#files[@]} -gt 0 ] && [ -f "${files[0]}" ]; then
        i=1
        for file in "${files[@]}"; do
            filename=$(basename "$file" .cpp)
            echo "$i. $filename" >> "$OUTPUT_FILE"
            echo "" >> "$OUTPUT_FILE"  # blank line for notes
            ((i++))
        done
    else
        echo "_No problems found in this section._" >> "$OUTPUT_FILE"
    fi
done

echo ""
echo "Markdown report saved to: $OUTPUT_FILE"
