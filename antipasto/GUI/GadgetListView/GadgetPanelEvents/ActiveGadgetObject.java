package antipasto.GUI.GadgetListView.GadgetPanelEvents;

import java.util.EventObject;
import java.io.File;

public class ActiveGadgetObject extends EventObject {

    private File sketch;
    private File boards;

    public ActiveGadgetObject(Object source, File sketchFile, File boardsFile) {
        super(source);
        sketch = sketchFile;
        boards = boardsFile;
    }

    public File getSketch(){
        return sketch;
    }

    public File getBoards(){
        return boards;
    }
}
