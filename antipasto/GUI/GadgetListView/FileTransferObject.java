package antipasto.GUI.GadgetListView;

import java.io.File;
import java.util.EventObject;

public class FileTransferObject extends EventObject {
    private File _f;

    public FileTransferObject(Object source, File f){
        super(source);
        _f = f;
    }

    public File getFile(){
        return _f;
    }
}
