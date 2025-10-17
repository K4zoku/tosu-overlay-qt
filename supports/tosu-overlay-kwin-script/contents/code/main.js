let cachedGeometry = { x: 0, y: 0, width: 0, height: 0 };

function sendGeometry(geometry) {
    if (isGeometryEqual(geometry, cachedGeometry)) {
        return;
    }
    cachedGeometry = geometry;
    callDBus("app.tosu.Overlay", "/", "app.tosu.Overlay.Ipc", "geometryChanged",
        geometry.x, geometry.y, geometry.width, geometry.height);
}

function isGeometryEqual(geometry1, geometry2) {
    return geometry1.x === geometry2.x &&
        geometry1.y === geometry2.y &&
        geometry1.width === geometry2.width &&
        geometry1.height === geometry2.height;
}

function isOsuWindow(window) {
    return window.resourceName === "osu!.exe" || window.resourceName === "osu!";
}

// Scan for currently opened osu window
for (let window of workspace.windowList()) {
    if (isOsuWindow()) {
        sendGeometry(window.clientGeometry);
        window.clientGeometryChanged.connect(() => sendGeometry(window.clientGeometry));
    }
}

// Connect event for new osu window added
workspace.windowAdded.connect(function(window) {
    if (isOsuWindow(window)) {
        sendGeometry(window.clientGeometry);
        window.clientGeometryChanged.connect(() => sendGeometry(window.clientGeometry));
    }
});

workspace.windowActivated.connect(function(window) {
    if (isOsuWindow(window)) {
        sendGeometry(window.clientGeometry);
    }
});