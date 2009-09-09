package antipasto.GUI.GadgetListView.GadgetPanelEvents;

import java.io.File;
import java.util.EventObject;

import javax.swing.JMenuItem;

public class ActiveBoardObject extends EventObject {

    private String referenceURL;
    private String boardLongName;
    private String boardShortName;
    private JMenuItem boardMenuItem;

    public ActiveBoardObject(Object source, 
                             String boardShortName, 
                             String boardLongName, 
                             String referenceURL,
                             JMenuItem boardMenuItem) {
        super(source);
        
        this.boardShortName = boardShortName;
        this.boardLongName = boardLongName;
        this.referenceURL = referenceURL;
        this.boardMenuItem = boardMenuItem;
    }

    public String getReferenceURL() {
        return referenceURL;
    }

    public String getName() {
        return getBoardLongName();
    }

    public String getBoardLongName() {
        return boardLongName;
    }

    public String getBoardShortName() {
        return boardShortName;
    }

    public JMenuItem getBoardMenuItem() {
        return boardMenuItem;
    }
}
