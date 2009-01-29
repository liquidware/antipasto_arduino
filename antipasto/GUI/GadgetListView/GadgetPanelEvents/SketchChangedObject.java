package antipasto.GUI.GadgetListView.GadgetPanelEvents;

import antipasto.Interfaces.IGadget;

public class SketchChangedObject {
    IGadget _book;
    public SketchChangedObject(IGadget book){
        _book = book;
    }

    public IGadget getSketchBook(){
        return _book;
    }
}
