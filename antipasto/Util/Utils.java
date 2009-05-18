package antipasto.Util;

import java.io.File;
import javax.swing.ImageIcon;

/* Some common utilities for gadgets. */
public class Utils {
    public final static String gdt = "gdt";
    public final static String module = "module";

    /*
     * Get the extension of a file.
     */
    public static String getExtension(File f) {
        String ext = null;
        String s = f.getName();
        int i = s.lastIndexOf('.');

        if (i > 0 &&  i < s.length() - 1) {
            ext = s.substring(i+1).toLowerCase();
        }
        return ext;
    }

}
