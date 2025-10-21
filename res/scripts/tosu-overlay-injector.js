(() => {
  let script = document.createElement('script');
  script.onload = function() {
    let object = null;
    new QWebChannel(qt.webChannelTransport, (channel) => object = channel.objects.object);
    document.addEventListener('keydown', (event) => object?.onKeyDown(event.key));
  };
  script.src = 'qrc:///qtwebchannel/qwebchannel.js';
  document.body.appendChild(script);
})();