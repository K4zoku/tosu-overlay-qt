
function send-geometry() {
    local x=$1 y=$2 w=$3 h=$4
    dbus-send --dest=app.tosu.Overlay / app.tosu.Overlay.Ipc.geometryChanged int32:$x int32:$y int32:$w int32:$h
}

while true; do
    # [TODO] run something to get window geometry
    # send-geometry $x $y $w $h
    sleep 1
done