#!/usr/bin/env bash
set -euo pipefail

readonly sdk_root="${ANDROID_SDK_ROOT:-$HOME/AndroidSDK}"
readonly adb="$sdk_root/platform-tools/adb"
readonly package="org.robosturm.commander_wars"
readonly activity="$package.CommanderWarsActivity"
readonly apk="$PWD/build_android/android-build/build/outputs/apk/debug/android-build-debug.apk"
readonly local_server="$sdk_root/ndk/30.0.16248370/toolchains/llvm/prebuilt/linux-x86_64/lib/clang/21/lib/linux/arm/lldb-server"
readonly remote_server="./files/cw-lldb-server"
readonly port=5039

cleanup() {
    "$adb" forward --remove "tcp:$port" >/dev/null 2>&1 || true
}
trap cleanup EXIT INT TERM

if [[ ! -f "$apk" ]]; then
    echo "APK not found. Run CMake: Build with the Android workspace configuration first." >&2
    exit 1
fi
if [[ ! -x "$local_server" ]]; then
    echo "Android LLDB server not found at $local_server" >&2
    exit 1
fi

echo "Starting Android LLDB server"
"$adb" wait-for-device
"$adb" get-state >/dev/null
if [[ "${CW_FORCE_ANDROID_INSTALL:-0}" == "1" ]] || ! "$adb" shell pm path "$package" >/dev/null 2>&1; then
    "$adb" shell pm trim-caches 512M >/dev/null 2>&1 || true
    if ! "$adb" install --no-incremental -r -t "$apk"; then
        echo "APK installation failed. Check available storage on the Android device." >&2
        exit 1
    fi
else
    echo "Using the already installed debug APK (set CW_FORCE_ANDROID_INSTALL=1 to reinstall)."
fi
"$adb" push "$local_server" /data/local/tmp/cw-lldb-server >/dev/null
"$adb" shell run-as "$package" pkill cw-lldb-server >/dev/null 2>&1 || true
"$adb" shell run-as "$package" mkdir -p files
"$adb" shell run-as "$package" cp /data/local/tmp/cw-lldb-server files/cw-lldb-server
"$adb" shell run-as "$package" chmod 700 files/cw-lldb-server
if ! "$adb" shell run-as "$package" test -x files/cw-lldb-server; then
    echo "The LLDB server was not created in the app sandbox." >&2
    exit 1
fi
"$adb" shell am force-stop "$package"
"$adb" shell am start -n "$package/$activity" >/dev/null

pid=""
for attempt in {1..20}; do
    pid=$("$adb" shell pidof -s "$package" | tr -d '\r')
    [[ -n "$pid" ]] && break
done
if [[ -z "$pid" ]]; then
    echo "Android app did not start." >&2
    exit 1
fi

"$adb" forward --remove "tcp:$port" >/dev/null 2>&1 || true
"$adb" forward "tcp:$port" "tcp:$port" >/dev/null
"$adb" shell run-as "$package" "$remote_server" g "0.0.0.0:$port" --attach "$pid" &
server_pid=$!
for attempt in {1..20}; do
    if (echo >/dev/tcp/127.0.0.1/$port) 2>/dev/null; then
        echo "LLDB server ready on localhost:$port"
        break
    fi
    if ! kill -0 "$server_pid" 2>/dev/null; then
        echo "The device LLDB server exited before opening port $port." >&2
        exit 1
    fi
    read -r -t 0.25 || true
done
wait "$server_pid"