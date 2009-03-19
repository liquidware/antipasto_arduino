package antipasto.Util;

import java.util.jar.JarEntry;
import java.util.jar.JarOutputStream;
import java.io.*;

public class Packer {
     public static File packageFile(String outPath, File[] packageFiles) throws IOException
    {
        File outFile = new File(outPath);
        

        BufferedOutputStream bo = new BufferedOutputStream(new FileOutputStream(outPath));
        JarOutputStream jo = new JarOutputStream(bo);

        for(int i =0; i < packageFiles.length; i++)
        {
            if(packageFiles[i] != null){
                File f = packageFiles[i];
                String act = f.getPath();
                BufferedInputStream bi = new BufferedInputStream(new FileInputStream(act));

                JarEntry je = new JarEntry(act);
                String comment = f.getName();
              
                System.out.println("parent for file " + f.getName() + " is " + f.getParent());
                if(f.getParent().equalsIgnoreCase("data")){
                	comment = "data" + File.separator + f.getName();
                }
                je.setComment(comment);
                jo.putNextEntry(je);

                byte[] buf = new byte[1024];
                int anz;

                while ((anz = bi.read(buf)) != -1) {
                  jo.write(buf, 0, anz);
                }

                bi.close();
            }
        }
        jo.close();
        bo.close();

        return outFile;
    }
}
