package antipasto.GUI.GadgetListView.GadgetPanelEvents;

import java.util.EventObject;
import java.io.File;

public class ActiveBoardObject extends EventObject {

    private String referenceURL;
    private String name;

    public ActiveBoardObject(Object source, String name, String referenceURL) {
        super(source);
        this.referenceURL = referenceURL;
        this.name = name;
    }

    public String getReferenceURL() {
        return referenceURL;
    }

    public String getName() {
        return name;
    }
}
