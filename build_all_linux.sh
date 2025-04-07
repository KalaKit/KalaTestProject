#!/bin/bash

# Set root folder
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

cd "$ROOT_DIR" || exit

echo "Building debug..."
echo ""

bash "./build_linux.sh" cmake build

echo ""
echo "Building release..."
echo ""

bash "./build_linux.sh" cmake release

echo ""
echo "Successfully finished building!"

echo ""
read -r -p "Press enter to exit..."
echo ""
exit 0
