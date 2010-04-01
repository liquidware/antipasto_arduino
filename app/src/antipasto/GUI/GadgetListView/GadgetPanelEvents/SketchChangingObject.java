package antipasto.GUI.GadgetListView.GadgetPanelEvents;


import java.io.File;

import antipasto.Interfaces.IGadget;

public class SketchChangingObject {
    public IGadget old;
    public File newFile;

    public SketchChangingObject(IGadget oldSketchBook, File newSketchFile){
        old = oldSketchBook;
        newFile = newSketchFile;
    }
}
