function sendGeometry(geometry) {
    callDBus("app.tosu.Overlay", "/", "app.tosu.Overlay.Ipc", "geometryChanged",
        geometry.x, geometry.y, geometry.width, geometry.height);
}

function connectWindowGeometryChanged(window) {
    sendGeometry(window.clientGeometry);

    window.clientGeometryChanged.connect(function(oldGeometry) {
        let data = {
            event: "geometry-changed",
            name: window.resourceName,
            geometry: window.clientGeometry,
            oldGeometry: oldGeometry
        };
        console.log("[Overlay]", JSON.stringify(data, null, 2));
        sendGeometry(window.clientGeometry);
    });
}

workspace.windowAdded.connect(function(window) {
    if (window.resourceName === "osu!.exe" || window.resourceName === "osu!") {
        console.log("[Overlay] Found newly added osu! window");
        let data = {
            event: "window-added",
            name: window.resourceName,
            geometry: window.clientGeometry
        };
        console.log("[Overlay]", JSON.stringify(data, null, 2));
        connectWindowGeometryChanged(window);
    }
});

workspace.windowActivated.connect(function(window) {
    if (window.resourceName === "osu!.exe" || window.resourceName === "osu!") {
        console.log("[Overlay] Found activated osu! window");
        let data = {
            event: "window-activated",
            name: window.resourceName,
            geometry: window.clientGeometry
        };
        console.log("[Overlay]", JSON.stringify(data, null, 2));
        sendGeometry(window.clientGeometry);
    }
});

for (let window of workspace.windowList()) {
    if (window.resourceName === "osu!.exe" || window.resourceName === "osu!") {
        console.log("[Overlay] Found osu! window");
        connectWindowGeometryChanged(window);
    }
}