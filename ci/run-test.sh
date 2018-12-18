#!/usr/bin/env bash
#set -o xtrace

CI_DIR=$(cd "$(dirname "$0")" && pwd)
ROOT_DIR=$(cd "${CI_DIR}/.." && pwd)

HW="$(find "$ROOT_DIR/" -mindepth 1 -maxdepth 1 -type d -name 'hw*' | sort | tail -n 1 | sed -nE 's/.*\/hw([0-9]+)$/\1/p')"

if [ -z "${HW}" ]; then
  echo "! No hw found."
  exit 1
fi

# run test in labshell sysoHWx
export HW

echo " found latest hw (hw${HW}), running tests"
"${CI_DIR}/run-test-labshell.sh"
