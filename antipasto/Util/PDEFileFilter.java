package antipasto.Util;

import java.io.File;
import javax.swing.*;
import javax.swing.filechooser.*;
import antipasto.Util.*;

/* Gadget File Filter used for file choosing. */
public class PDEFileFilter extends FileFilter {

    //Accept all gadget files
    public boolean accept(File f) {
        if (f.isDirectory()) {
            return true;
        }

        String extension = Utils.getExtension(f);
        if (extension != null) {
            if (extension.equals(Utils.pde)) {
                    return true;
            } else {
                return false;
            }
        } else if (f.isDirectory()) {
        	return true;
        }

        return false;
    }

    //The description of this filter
    public String getDescription() {
        return "PDE Files (*.pde)";
    }
}


