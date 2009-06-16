package antipasto.Util;

import java.io.File;
import javax.swing.*;
import javax.swing.filechooser.*;
import antipasto.Util.*;

/* Gadget File Filter used for file choosing. */
public class ScriptFileFilter extends FileFilter {

    //Accept all script files
    public boolean accept(File f) {
        if (f.isDirectory()) {
            return true;
        }

        String extension = Utils.getExtension(f);
        if (extension != null) {
            if (extension.equals(Utils.rb) || f.isDirectory()) {
                    return true;
            } else {
                return false;
            }
        }

        return false;
    }

    //The description of this filter
    public String getDescription() {
        return "Script Files (*.rb)";
    }
}


