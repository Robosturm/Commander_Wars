set -eu

# Default to a 60 seconds interval between printing messages.
PERIOD=${1:-60}

nexttime=$PERIOD
msg=
count=0

# Reset timer (SECONDS is a special Bash variable).
SECONDS=0

while true; do
    # Periodically report the last read line.
    timeleft=$((nexttime-SECONDS))
    while [ $timeleft -le 0 ]; do
        ((nexttime+=PERIOD))
        ((timeleft+=PERIOD))
        printf "[progress] %3d %s\n" $SECONDS "${msg:-(no output)}"
        msg=
    done

    if read -r -t $timeleft line; then
        # Save line for later.
        ((count+=1))
        msg="Line $count: $line"
        continue
    elif [ $? -le 128 ]; then
        # EOF (as opposed to a timeout)
        [ -z "$msg" ] || printf "[progress] %3d %s\n" $SECONDS "$msg"
        printf "[progress] %3d done (read %d lines).\n" $SECONDS $count
        break
    fi
done