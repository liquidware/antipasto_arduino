package antipasto.Plugins.Api;

import java.io.*;
import processing.app.*;
import org.apache.log4j.Logger;

public class EditorApi {

    private Logger __logger = Logger.getLogger(EditorApi.class);

    public EditorApi() {
    }

    public void exit() {
        //Base.handleQuit();
        __logger.info("exiting");
    }

    public void addFileToImagepanel(File f) {
        try {
            Base.getActiveEditor().getSketch().addFile(f);
        } catch (Exception ne) {
            __logger.error("ERROR ADDING FILE: " + f.getPath());
        }
    }

    public void burnImages() {
        __logger.info("burnImages");
        Base.getActiveEditor().imageListPanel.transfer();
    }

    public void setCode(String thecode) {
        __logger.info("setCode");
        Base.getActiveEditor().getTextArea().setText(thecode);
        Base.getActiveEditor().getSketch().setModified(true);
        Base.getActiveEditor().handleSave(true);
    }

    public void burnCode() {
        __logger.info("burnCode");
        Base.getActiveEditor().handleExport(false);
    }

    public String version() {
        return "0.1";
    }
}
