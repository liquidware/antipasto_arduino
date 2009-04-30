package antipasto.Util;

import java.io.*;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;
import java.util.Enumeration;

public class UnPacker {
    public static File[] UnPack(File packedFile, File unpackDirectory)
    {
        File f = unpackDirectory;
        if(!f.exists())
        {
            f.mkdir();
        }


        String URI = packedFile.getAbsolutePath();
        File[] files = null;
        try
        {
            JarFile jf = new JarFile(URI);
            Enumeration entries = jf.entries();
            Enumeration getCount = jf.entries();

            int count = 0;

            while(getCount.hasMoreElements())
            {
                count++;
                getCount.nextElement();
            }

            files = new File[count];

            int i = 0;

            while(entries.hasMoreElements())
            {
                JarEntry je = (JarEntry) entries.nextElement();
            	System.out.println(je.getComment());
                BufferedInputStream stream = new BufferedInputStream(jf.getInputStream(je));
                WriteFile(f.getPath(), je.getComment(), stream);
                files[i] = new File(f.getPath() +  File.separator + je.getComment());
                i++;
            }
        }
        catch(IOException ex)
        {
            ex.printStackTrace();
        }

        return files;
    }

    private static void WriteFile(String outputDirectory, String fileName, BufferedInputStream stream) throws IOException
    {
        byte[] buf = new byte[1024];
        int length;

        BufferedOutputStream bo = new BufferedOutputStream(new FileOutputStream( outputDirectory + File.separator + fileName));

        while((length = stream.read(buf)) != -1)
        {
            bo.write(buf, 0, length);
        }
        bo.flush();
        bo.close();
    }
}
