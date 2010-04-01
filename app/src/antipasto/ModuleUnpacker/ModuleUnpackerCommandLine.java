package antipasto.ModuleUnpacker;

import java.io.File;

import antipasto.ModuleFactory;


public class ModuleUnpackerCommandLine {
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

            ModuleFactory fact = new ModuleFactory();
            File sourceSketch = new File(sourceFile);
            File destinationDirectory = new File(outputPath);

            if(sourceSketch.exists())
            {
                if(!destinationDirectory.exists())
                {
                    destinationDirectory.mkdir();
                }
                try {
                    //TODO FIGURE OUT WTF IS GOING ON HERE
                    fact.loadModule(sourceSketch, destinationDirectory.getPath(), true);
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
