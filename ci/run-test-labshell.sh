#!/usr/bin/env labshell
#!/usr/bin/env bash
#set -o xtrace

ROOT_DIR=$(cd "$(dirname "$0")/.." && pwd)

HW="$(find "$ROOT_DIR/" -mindepth 1 -maxdepth 1 -type d -name 'hw*' | sort | tail -n 1 | sed -nE 's/.*\/hw([0-9]+)$/\1/p')"
HW_FOLDER="${ROOT_DIR}/hw${HW}"
HW_SCRIPT="${HW_FOLDER}/hw${HW}.sh"
echo " found latest hw (${HW}) in '${HW_FOLDER}' with scripts '${HW_SCRIPT}'"

# skip hw < 1
if [ -z "$HW" ] || [ "$HW" -lt 1 ]; then
  echo '+ No tests for hw < 1, skipping'
  exit 0
fi

# kill qemu
if pgrep qemu-system-; then
  echo '! qemu is running, please stop qemu before running this test.'
  exit 1
fi

function run() {
  echo " run '$@'..."
  if ! "$@"; then
    echo "! '$@' failed."
    exit -1
  fi
}

# cd into hw folder
cd "${HW_FOLDER}"

# build
echo " test 'Build all artifacts' function..."
run "${HW_SCRIPT}"
run "${ROOT_DIR}/ci/check-files.py" "${ROOT_DIR}/ci/artifacts/hw${HW}.txt"

# run qemu
echo " test 'qemu*' script functions..."
if [ "$HW" -eq 1 ]; then
  run "${HW_SCRIPT}" qemu_sysinfo 0</dev/null &
  sleep 30
  echo " verify qemu is running..."
  run pgrep qemu-system-x86
  run pkill qemu-system-x86
  
  run "${HW_SCRIPT}" qemu_busybox 0</dev/null &
  sleep 30
  echo " verify qemu is running..."
  run pgrep qemu-system-x86
else
  run "${HW_SCRIPT}" qemu 0</dev/null &
  sleep 30

  echo " verify qemu is running..."
  if [ "$HW" -lt 3 ]; then
    run pgrep qemu-system-x86
  else
    run pgrep qemu-system-aar
  fi
fi

# kernel modules
if [ "$HW" -gt 3 ]; then
  echo " test 'kernel modules' functions..."
  run "${HW_SCRIPT}" modules
fi

# ssh_cmd
if [ "$HW" -gt 1 ]; then
  echo " test 'ssh_cmd' function..."
  run "${HW_SCRIPT}" ssh_cmd "echo 'Hello, World'"
fi

echo " kill qemu..."
if [ "$HW" -lt 3 ]; then
  run pkill qemu-system-x86
else
  run pkill qemu-system-aar
fi

# clean
echo " test 'clean' function..."
run "${HW_SCRIPT}" clean
CLEAN_LOG="$(git clean -df --dry-run)"
if [ "$CLEAN_LOG" != "" ]; then
  echo "clean failed:"
  echo "$CLEAN_LOG"
  exit -6
fi
