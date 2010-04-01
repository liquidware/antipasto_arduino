package antipasto.GadgetUnpacker;
import java.io.File;

import antipasto.GadgetFactory;


public class GadgetUnpacker {
    public static void main(String args[])
    {
        if(args.length < 2)
        {
            System.out.println("You must specify both an input and an output directory");
        }
        else
        {
            File skFile = new File(args[0]);
            String outputDirectory = args[1];

            File outDir = new File(outputDirectory);
            if(!outDir.exists())
            {
                outDir.mkdir();
            }

            GadgetFactory sbFactory = new GadgetFactory();

            sbFactory.loadGadget(skFile, outputDirectory);
        }
    }
}
