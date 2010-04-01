package antipasto.GUI.GadgetListView.GadgetPanelEvents;

import java.io.File;
import java.util.EventObject;

import javax.swing.JMenuItem;

public class ActiveBoardObject extends EventObject {

    private String referenceURL;
    private String boardLongName;
    private String boardShortName;
    private JMenuItem boardMenuItem;
    private String corePath;
    private String librariesPath;
    private String examplesPath;

    public ActiveBoardObject(Object source,
                             String boardShortName,
                             String boardLongName,
                             JMenuItem boardMenuItem,
                             String corePath,
                             String librariesPath,
                             String examplesPath,
                             String referenceURL) {
        super(source);

        this.boardShortName = boardShortName;
        this.boardLongName = boardLongName;
        this.referenceURL = referenceURL;
        this.boardMenuItem = boardMenuItem;
        this.corePath = corePath;
        this.librariesPath = librariesPath;
        this.examplesPath = examplesPath;
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

    public String getLibrariesFolder() {
        return librariesPath;
    }

    public String getExamplesFolder() {
        return examplesPath;
    }

}
