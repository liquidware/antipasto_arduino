package antipasto.Util;

import java.io.File;
import javax.swing.*;
import javax.swing.filechooser.*;
import antipasto.Util.*;

/* Gadget File Filter used for file choosing. */
public class AntipastoFileFilter extends FileFilter {

    //Accept all gadget files
    public boolean accept(File f) {
        if (f.isDirectory()) {
            return true;
        }

        String extension = Utils.getExtension(f);
        if (extension != null) {
            if (f.isDirectory()|| 
                extension.equals(Utils.gdt) || 
                extension.equals(Utils.pde)) {
                    return true;
            } else {
                return false;
            }
        }

        return false;
    }

    //The description of this filter
    public String getDescription() {
        return "All Antipasto Files (*.pde, *.gdt)";
    }
}


