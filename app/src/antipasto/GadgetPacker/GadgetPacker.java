package antipasto.GadgetPacker;

import java.io.File;
import java.io.IOException;

import antipasto.GadgetFactory;
import antipasto.ModuleFactory;
import antipasto.Interfaces.IModule;


public class GadgetPacker {
    public static void main(String args[])
    {
        if(args.length < 3)
            {
                System.out.println("Need to specify at least one gadget file");
            }
            else
            {
                String outputDirectory = args[0];
                String name = args[1];

                GadgetFactory sbFact = new GadgetFactory();
                ModuleFactory gFact = new ModuleFactory();

                IModule[] gadgetArr = new IModule[args.length - 2];

                for(int i = 2; i < args.length; i++)
                {
                    File f = new File(args[i]);
                    try {
                        gadgetArr[i-2] = gFact.loadModule(f, System.getProperty("java.io.tmpdir"),true);
                    } catch (Exception e) {
                        e.printStackTrace();  //To change body of catch statement use File | Settings | File Templates.
                    }
                }
                try {
                    sbFact.CreateGadgetFile(name, outputDirectory, gadgetArr);
                } catch (IOException e) {
                    e.printStackTrace();  //To change body of catch statement use File | Settings | File Templates.
                }
            }
    }
}
