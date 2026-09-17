#!/usr/bin/env bash
set -euo pipefail

readonly sdk_root="${ANDROID_SDK_ROOT:-$HOME/AndroidSDK}"
readonly adb="$sdk_root/platform-tools/adb"
readonly package="org.robosturm.commander_wars"
readonly activity="$package.CommanderWarsActivity"
readonly apk="$PWD/build_android/android-build/build/outputs/apk/debug/android-build-debug.apk"
readonly local_server="$sdk_root/ndk/30.0.16248370/toolchains/llvm/prebuilt/linux-x86_64/lib/clang/21/lib/linux/arm/lldb-server"
readonly remote_server="files/cw-lldb-server"
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
"$adb" get-state >/dev/null
"$adb" install -r -t "$apk" >/dev/null
"$adb" push "$local_server" /data/local/tmp/cw-lldb-server >/dev/null
"$adb" shell run-as "$package" pkill cw-lldb-server >/dev/null 2>&1 || true
"$adb" shell run-as "$package" cp /data/local/tmp/cw-lldb-server "$remote_server"
"$adb" shell run-as "$package" chmod 700 "$remote_server"
"$adb" shell am force-stop "$package"
"$adb" shell am start -W -n "$package/$activity" >/dev/null

pid=$("$adb" shell pidof -s "$package" | tr -d '\r')
if [[ -z "$pid" ]]; then
    echo "Android app did not start." >&2
    exit 1
fi

"$adb" forward --remove "tcp:$port" >/dev/null 2>&1 || true
"$adb" forward "tcp:$port" "tcp:$port" >/dev/null
"$adb" shell run-as "$package" "$remote_server" g "0.0.0.0:$port" --attach "$pid" &
server_pid=$!
sleep 0.5
echo "LLDB server ready on localhost:$port"
wait "$server_pid"