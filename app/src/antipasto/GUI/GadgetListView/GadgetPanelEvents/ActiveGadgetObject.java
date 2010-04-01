package antipasto.GUI.GadgetListView.GadgetPanelEvents;

import java.util.EventObject;
import java.io.File;

import antipasto.Interfaces.IModule;

public class ActiveGadgetObject extends EventObject {

    private File sketch;
    private File boards;
    private IModule module;

    public ActiveGadgetObject(Object source, File sketchFile, File boardsFile, IModule module) {
        super(source);
        sketch = sketchFile;
        boards = boardsFile;
        this.module = module;
    }

    public File getSketch(){
        return sketch;
    }

    public File getBoards(){
        return boards;
    }
    
    public IModule getModule(){
    	return module;
    }
}
