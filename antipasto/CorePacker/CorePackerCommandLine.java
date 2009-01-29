package antipasto.CorePacker;

import java.io.File;
import java.util.StringTokenizer;

import antipasto.Core;
import antipasto.CoreFactory;

public class CorePackerCommandLine {
    public static void main(String args[])
    {
        if(args.length < 4)
        {
            System.out.println("Error with args");
        }
        else
        {
            String sourceDirectory = args[0];
            String destinationDirectory = args[1];
            String name = args[2];
            StringTokenizer tokenizedVersion = new StringTokenizer(args[3], ".");

            String maj = tokenizedVersion.nextToken();
            String min = tokenizedVersion.nextToken();
            String rev = tokenizedVersion.nextToken();

            File[] files = new File(sourceDirectory).listFiles();

            Core c = new Core();
            

            c.setFiles(files);
            c.setName(name);

            c.setMajorNumber(Integer.parseInt(maj));
            c.setMinorNumber(Integer.parseInt(min));
            c.setRevisionNumber(Integer.parseInt(rev));

            CoreFactory fact = new CoreFactory();
            try {
                fact.WriteCoreToFile(new File(destinationDirectory).getCanonicalPath(), name, ".core",  c);
            } catch (Exception e) {
                e.printStackTrace();  
            }
        }
    }
}
