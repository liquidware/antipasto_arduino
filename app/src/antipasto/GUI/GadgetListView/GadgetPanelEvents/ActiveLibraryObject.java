package antipasto.GUI.GadgetListView.GadgetPanelEvents;

import java.io.File;
import java.util.EventObject;

import javax.swing.JMenuItem;

public class ActiveLibraryObject extends EventObject {

    private String referenceURL;
    private String libraryLongName;
    private String libraryShortName;
    private JMenuItem libraryMenuItem;

    public ActiveLibraryObject(Object source,
                               String libraryShortName,
                               String libraryLongName,
                               String referenceURL,
                               JMenuItem libraryMenuItem) {
        super(source);

        this.libraryShortName = libraryShortName;
        this.libraryLongName = libraryLongName;
        this.referenceURL = referenceURL;
        this.libraryMenuItem = libraryMenuItem;
    }

    public String getReferenceURL() {
        return referenceURL;
    }

    public String getName() {
        return getLibraryLongName();
    }

    public String getLibraryLongName() {
        return libraryLongName;
    }

    public String getLibraryShortName() {
        return libraryShortName;
    }

    public JMenuItem getLibraryMenuItem() {
        return libraryMenuItem;
    }
}
