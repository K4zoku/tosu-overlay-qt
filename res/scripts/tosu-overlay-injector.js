(() => {
  let maxFps = __TOSU_OVERLAY_MAX_FPS__;
  let minFrameIntervalMs = maxFps > 0 ? 1000 / maxFps : 0;
  const nativeRequestAnimationFrame = window.requestAnimationFrame.bind(window);
  const pendingAnimationFrames = new Map();
  let nextAnimationFrameId = 0;
  let scheduledAnimationFrame = null;
  let lastTimestamp = null;

  const scheduleAnimationFramePump = () => {
    if (scheduledAnimationFrame !== null) {
      return;
    }

    scheduledAnimationFrame = nativeRequestAnimationFrame((timestamp) => {
      scheduledAnimationFrame = null;
      if (pendingAnimationFrames.size === 0) {
        return;
      }

      if (minFrameIntervalMs > 0 && lastTimestamp !== null && timestamp - lastTimestamp < minFrameIntervalMs) {
        scheduleAnimationFramePump();
        return;
      }

      lastTimestamp = timestamp;
      const callbacks = Array.from(pendingAnimationFrames.values());
      pendingAnimationFrames.clear();

      for (const callback of callbacks) {
        callback(timestamp);
      }

      if (pendingAnimationFrames.size > 0) {
        scheduleAnimationFramePump();
      }
    });
  };

  window.__tosuOverlaySetAnimationFrameRate = (fps) => {
    if (!Number.isFinite(fps) || fps <= 0) {
      return;
    }

    maxFps = fps;
    minFrameIntervalMs = 1000 / maxFps;
    lastTimestamp = null;
  };

  window.requestAnimationFrame = (callback) => {
    const animationFrameId = ++nextAnimationFrameId;
    pendingAnimationFrames.set(animationFrameId, callback);
    scheduleAnimationFramePump();
    return animationFrameId;
  };

  window.cancelAnimationFrame = (animationFrameId) => {
    pendingAnimationFrames.delete(animationFrameId);
  };

  const initializeChannel = () => {
    const script = document.createElement("script");
    script.onload = function () {
      let object;
      new QWebChannel(
        qt.webChannelTransport,
        (channel) => (object = channel.objects.object),
      );
      document.addEventListener("keydown", (event) =>
        object?.onKeyDown(event.key),
      );
    };
    script.src = "qrc:///qtwebchannel/qwebchannel.js";
    (document.head || document.documentElement).appendChild(script);
  };

  if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", initializeChannel, { once: true });
  } else {
    initializeChannel();
  }
})();
