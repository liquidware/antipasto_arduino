package antipasto.CoreUnpacker;
//import org.apache.xpath.operations.*;



import java.lang.String;
import java.io.File;

import antipasto.CoreFactory;

public class CoreUnpackerCommandLine {
    public static void main(String args[])
    {
        if(args.length < 2)
        {
            System.out.println("Incorrect number of command line arguements");
        }
        else
        {
            String sourceFile = args[0];
            String outputPath = args[1];

            CoreFactory fact = new CoreFactory();
            File sourceCore = new File(sourceFile);
            File destinationDirectory = new File(outputPath);

            if(sourceCore.exists())
            {
                if(!destinationDirectory.exists())
                {
                    destinationDirectory.mkdir();
                }
                try {
                    fact.UnpackCore(sourceCore, outputPath);
                } catch (Exception e) {
                    e.printStackTrace();  //To change body of catch statement use File | Settings | File Templates.
                }
            }
            else
            {
                System.out.println("Core file does not exist");
            }
        }
    }
}
