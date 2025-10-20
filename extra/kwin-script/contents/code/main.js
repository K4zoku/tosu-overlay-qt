function sendGeometry(geometry) {
    callDBus("app.tosu.Overlay", "/", "app.tosu.Overlay.Ipc", "geometryChanged",
        geometry.x, geometry.y, geometry.width, geometry.height);
}

function connectWindowGeometryChanged(window) {
    sendGeometry(window.clientGeometry);

    window.clientGeometryChanged.connect(function(oldGeometry) {
        sendGeometry(window.clientGeometry);
    });
}

workspace.windowAdded.connect(function(window) {
    if (window.resourceName === "osu!.exe" || window.resourceName === "osu!") {
        connectWindowGeometryChanged(window);
    }
});

workspace.windowActivated.connect(function(window) {
    if (window.resourceName === "osu!.exe" || window.resourceName === "osu!") {
        sendGeometry(window.clientGeometry);
    }
});

for (let window of workspace.windowList()) {
    if (window.resourceName === "osu!.exe" || window.resourceName === "osu!") {
        connectWindowGeometryChanged(window);
    }
}