PROJECT_NAME = tosu-overlay
PROJECT_VER  = 1.0.0

KWINPKG_FILE = $(PROJECT_NAME)-$(PROJECT_VER).kwinscript

clean: 
	@rm -f "$(KWINPKG_FILE)"

package: $(KWINPKG_FILE)

install: package
	kpackagetool6 --type=KWin/Script -u $(KWINPKG_FILE) || kpackagetool6 --type=KWin/Script -i $(KWINPKG_FILE) && \
	kpackagetool6 --type=KWin/Script -s $(PROJECT_NAME)
	
uninstall:
	kpackagetool6 --type=KWin/Script -r $(PROJECT_NAME)

$(KWINPKG_FILE): $(KWINPKG_DIR)
	@7z a -tzip $(KWINPKG_FILE) ./contents/ ./metadata.json
