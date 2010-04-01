package antipasto.ModulePacker;

import java.io.File;
import java.util.StringTokenizer;

import antipasto.Module;
import antipasto.ModuleFactory;

public class ModulePackerCommandLine {
    public static void main(String args[])
    {
    	/*
    	 * Arg 1: Source Directory
    	 * Arg 2: Destination Directory
    	 * Arg 3: Name 
    	 * Arg 4: TargetBoard
    	 * Arg 5: Version
    	 */
        if(args.length < 5)
        {
            System.out.println("Error with args");
        }
        else
        {
            String sourceDirectory = args[0];
            String destinationDirectory = args[1];
            String name = args[2];
            String targetBoard = args[3];
            StringTokenizer tokenizedVersion = new StringTokenizer(args[4], ".");

            String maj = tokenizedVersion.nextToken();
            String min = tokenizedVersion.nextToken();
            String rev = tokenizedVersion.nextToken();

            File[] files = new File(sourceDirectory).listFiles();

            File rulesFile = null;
            
            for(int i = 0; i < files.length; i++){
            	if(files[i].getName().equalsIgnoreCase("rules.xml")){
            		rulesFile = files[i];
            	}
            }
            
            Module module = new Module();

            ModuleFactory fact = new ModuleFactory();
            try {
                module = (Module) fact.CreateModule(files);
                module.setName(name);
                module.getSketchFile().renameTo(new File(module.getName()));
                
                module.setMajorNumber(Integer.parseInt(maj));
                module.setMinorNumber(Integer.parseInt(min));
                module.setRevisionNumber(Integer.parseInt(rev));
                module.setTarget(targetBoard);
                module.setRulesFile(rulesFile);
            } catch (Exception e) {
                e.printStackTrace();  //To change body of catch statement use File | Settings | File Templates.
            }
            try {
                fact.WriteModuleToFile(module, destinationDirectory);
            } catch (Exception e) {
                e.printStackTrace();  //To change body of catch statement use File | Settings | File Templates.
            }
        }
    }
}
