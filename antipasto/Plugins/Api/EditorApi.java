package antipasto.Plugins.Api;

import java.io.*;
import processing.app.*;
import org.apache.log4j.Logger;

public class EditorApi {

    private Logger __logger = Logger.getLogger(EditorApi.class);

    public EditorApi() {
    }

    public void exit() {
        Base.editor.handleQuitInternal();
        __logger.info("exiting");
    }

    public void addFileToImagepanel(File f) {
        try {
            Base.editor.sketch.addFile(f);
        } catch (Exception ne) {
            __logger.error("ERROR ADDING FILE: " + f.getPath());
        }
    }

    public void burnImages() {
        __logger.info("burnImages");
        Base.editor.setImageListVisable(Base.editor.gadgetPanel.getActiveModule());
        Base.editor.imageListPanel.transfer();
    }

    public void setCode(String thecode) {
        __logger.info("setCode");
        Base.editor.textarea.setText(thecode);
        Base.editor.sketch.setModified(true);
        Base.editor.handleSave(true);
    }

    public void burnCode() {
        __logger.info("burnCode");
        Base.editor.handleExport();
    }

    public String version() {
        return "0.1";
    }
}
