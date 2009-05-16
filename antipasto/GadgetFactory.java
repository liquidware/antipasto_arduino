package antipasto;

import org.w3c.dom.Element;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import processing.app.Base;

import antipasto.CoreFactory;
import antipasto.Gadget;
import antipasto.ModuleFactory;
import antipasto.Interfaces.*;
import antipasto.Util.*;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.DocumentBuilder;
import java.io.*;
import java.util.List;
import java.util.ArrayList;
import java.util.logging.Logger;
import java.util.logging.Level;


/**
 * @author omarayub
 *
 */
/**
 * @author omarayub
 *
 */
public class GadgetFactory {
    public Element createGadgetXML(IGadget gadget) throws Exception {
        Element root = null;
        try
        {
            Element modules;
            IModule[] moduleFiles = gadget.getModules();
            DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
            DocumentBuilder db = dbf.newDocumentBuilder();
            Document doc = db.newDocument();
            root = doc.createElement("SketchBook");
            modules = doc.createElement("Modules");

            for(int i = 0; i < moduleFiles.length; i++)
            {
                modules.appendChild(doc.importNode(moduleFiles[i].getConfiguration(), true));
            }
            Element nameElem = doc.createElement("Name");
            nameElem.setAttribute("value", gadget.getName());

            Element versionElement = XMLVersionCreator.CreateVersionElement(doc, gadget);

            root.appendChild(nameElem);
            root.appendChild(versionElement);
            root.appendChild(modules);
        }
        catch(Exception ex)
        {
            throw new Exception("Error creating the sketch book configuration");
        }
        return root;
    }

    public IGadget loadGadget(File gadgetFile, String outputDirectory) {
        if(gadgetFile.getName().endsWith(".pde") || gadgetFile.getName().endsWith(".gdt"))
        {
            File[] unpackedFiles = UnPacker.UnPack(gadgetFile, new File(outputDirectory));
            File configFile = findFileByName("config.xml", unpackedFiles);
            try {
                Gadget gadget = (Gadget)this.generate(getFirstElementOfModuleFile(configFile), unpackedFiles, outputDirectory);
                gadget.setPackedFile(gadgetFile);
                //IModule[] modules = gadget.getGadgets();
                return gadget;
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return null;
    }
    
    public File copyGadget(IGadget src, String outputDirectory, String name) throws IOException{
    	IModule[] gadgetModules = src.getModules();
    	
    	
    	ModuleFactory fact = new ModuleFactory();
    	
    	IModule[] newGadgetModules = new IModule[gadgetModules.length];
    	
    	File modTempDir = Base.createTempFolder("moduleTemp");
    	
    	for(int i = 0; i < gadgetModules.length; i++){
    		File tempDir = Base.createTempFolder("slack" + i);
    		File newFile = new File(tempDir.getPath() + File.separator + ((IPackedFile)gadgetModules[i]).getPackedFile().getName());
    		try {
				Base.copyFile(((IPackedFile)gadgetModules[i]).getPackedFile(), newFile);
				try {
					newGadgetModules[i] = fact.loadModule(newFile, modTempDir.getPath() + File.separator + ((IPackedFile)gadgetModules[i]).getPackedFile().getName()
															, false);
				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
    	}
    	return this.CreateGadgetFile(name, outputDirectory, newGadgetModules);
    }

    /*
     * Writes gadget back to the original file
     * */
    public File writeGadgetToFile(IGadget gadget, File originalFile){
        File outDir = new File(originalFile.getPath());
        if(!outDir.exists())
        {
            outDir.mkdir();
        }

        File tmpDir = Base.createTempFolder(originalFile.getName());
        tmpDir.mkdir();

        ModuleFactory fact = new ModuleFactory();

        for(int i=0; i < gadget.getModules().length; i++)
        {
            try {
            	//NOTE: We may need to add a hook for the module rules
                fact.WriteModuleToFile(gadget.getModules()[i], tmpDir.getPath());
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        try {
            XMLWriter.Writer(gadget.getConfiguration(), "config.xml", tmpDir.getPath());
        } catch (Exception e) {
            e.printStackTrace();
        }


        File returnFile = null;
		try {
			returnFile = Packer.packageFile(originalFile.getPath()
			                                    , tmpDir.listFiles());
		} catch (IOException e) {
			e.printStackTrace();
		}

        File[] tmpFiles = tmpDir.listFiles();
        for(int i =0; i < tmpFiles.length; i++)
        {
            tmpFiles[i].delete();
        }  

        tmpDir.delete();
        return returnFile;
    }
    
    /*
     * Outputs a gadget to the specified directory
     * */
    public File writeGadgetToFile(IGadget gadget, String outputDir) throws IOException {
        File outDir = new File(outputDir);
        if(!outDir.exists())
        {
            outDir.mkdir();
        }

        File tmpDir = Base.createTempFolder(gadget.getName());
        tmpDir.mkdir();

        ModuleFactory fact = new ModuleFactory();

        for(int i=0; i < gadget.getModules().length; i++)
        {
            try {
            	//NOTE: We may need to add a hook for the module rules
                fact.WriteModuleToFile(gadget.getModules()[i], tmpDir.getPath());
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        try {
            XMLWriter.Writer(gadget.getConfiguration(), "config.xml", tmpDir.getPath());
        } catch (Exception e) {
            e.printStackTrace();
        }


        File returnFile = Packer.packageFile(outDir.getPath() + File.separator + gadget.getName() + ".gdt"
                                            , tmpDir.listFiles());

        File[] tmpFiles = tmpDir.listFiles();
        for(int i =0; i < tmpFiles.length; i++)
        {
            tmpFiles[i].delete();
        }

        tmpDir.delete();
        return returnFile;
    }

   

    private File findFileByName(String name, File[] files)
    {
        for(int i = 0; i < files.length; i++)
        {
            if(files[i].getName().equalsIgnoreCase(name))
            {
                return files[i];
            }
        }
        return null;
    }

    public File CreateGadgetFile(String name, String outputDirectory, IModule[] modules) throws IOException {
        IGadget gadget = new Gadget();

        gadget.setModule(modules);
        gadget.setName(name);
        gadget.setMajorNumber(1);
        gadget.setMinorNumber(0);
        gadget.setRevisionNumber(0);
        ((ITemporary)gadget).setTempDirectory(outputDirectory);

        return this.writeGadgetToFile(gadget, outputDirectory);
    }

    private IGadget generate(Element config, File[] files, String outputDirectory)
    {
        int length = config.getChildNodes().getLength();
        NodeList children = config.getChildNodes();
        int maj, min, rev;
        maj = 1;
        min = 0;
        rev = 0;

        String name = "";
        List gadgets = new ArrayList();

        for(int i =0; i < length; i++)
        {
            if(children.item(i).getNodeName().equalsIgnoreCase("name"))
            {
                name = children.item(i).getAttributes().item(0).getNodeValue();
            }else if(children.item(i).getNodeName().equalsIgnoreCase("modules"))
            {
                NodeList moduleElements = children.item(i).getChildNodes();
                ModuleFactory gadgetFactory = new ModuleFactory();
                for(int x = 0; x < moduleElements.getLength(); x++)
                {
                    if(moduleElements.item(x).getNodeName().equalsIgnoreCase("module"))
                    {
                        String moduleName = getModuleElementName((Element) moduleElements.item(x));
                        File moduleFile = this.findFileByName(moduleName + IModule.moduleExtension , files);

                        try {
                            gadgets.add(gadgetFactory.loadModule(moduleFile,outputDirectory + File.separator + moduleName , true));
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                }
            }else if(children.item(i).getNodeName().equalsIgnoreCase("version"))
            {
                try {
                    IVersionable version = XMLVersionReader.Load((Element) children.item(i));
                    maj = version.getMajorNumber();
                    min = version.getMajorNumber();
                    rev = version.getRevisionNumber();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }

        Gadget book = new Gadget();
        IModule[] gadArr = new IModule[gadgets.size()];

        for(int i = 0; i < gadgets.size(); i++)
        {
            gadArr[i] = (IModule) gadgets.get(i);
        }

        book.setModule(gadArr);
        book.setName(name);
        book.setMajorNumber(maj);
        book.setMinorNumber(min);
        book.setRevisionNumber(rev);
        book.setTempDirectory(outputDirectory);

        return book;
    }

    
    /**
     * Gets the name of the module from the element 
     * @param gadget
     * @return
     */
    private String getModuleElementName(Element gadget)
    {
        for(int i = 0; i < gadget.getChildNodes().getLength(); i++)
        {
            if(gadget.getChildNodes().item(i).getNodeName().equalsIgnoreCase("name"))
            {
                return gadget.getChildNodes().item(i).getAttributes().item(0).getNodeValue();
            }
        }
        return "";
    }

    
    /**
     * Gets the first element of the module configuration file
     * @param xmlConfig
     * @return Xml element
     * @throws Exception
     */
    private Element getFirstElementOfModuleFile(File xmlConfig) throws Exception {
        try {
            DocumentBuilderFactory docFact = DocumentBuilderFactory.newInstance();
            DocumentBuilder docBuilder = docFact.newDocumentBuilder();
            Document doc = docBuilder.parse(xmlConfig);
            return (Element) doc.getFirstChild();
        } catch (SAXException ex) {
            Logger.getLogger(CoreFactory.class.getName()).log(Level.SEVERE, null, ex);
        } catch (IOException ex) {
            Logger.getLogger(CoreFactory.class.getName()).log(Level.SEVERE, null, ex);
        }

        throw new Exception("Error getting the base element of the project");
    }

    /**
     * Adds a module to the gadget by copying the original module file to the currently expanded
     * gadget directory
     * @param gadget
     * @param module
     * @return the new gadget
     * @throws Exception
     */
    public IGadget AddModuleToGadget(IGadget gadget, IModule module) throws Exception {
        if(gadget instanceof IGadget){
            if(module instanceof Module){
                int hasAlreadyCount = 0;
                for(int i = 0; i < gadget.getModules().length; i++){
                    if(gadget.getModules()[i].getName().equalsIgnoreCase(module.getName())){
                        hasAlreadyCount ++;
                    }
                }
                
                //Count the number of modules and increment the name
                String moduleName = module.getName();
                if(hasAlreadyCount > 1){
                    moduleName = module.getName() + "(" + hasAlreadyCount + ")";
                }
                //copy the module to the base directory of the gadget
                //get the file copy it to the book directory, unpack it to a subfolder
                Gadget gadgetBase = (Gadget) gadget;
                Module moduleOriginal = (Module)module;
                File moduleFileOriginal = moduleOriginal.getPackedFile();
                File outDirectory = new File(gadgetBase.getTempDirectory() + File.separator + module.getName());
                
                if(!outDirectory.exists()){
                	outDirectory.mkdir();
                }
                
                File moduleFileNew = new File(outDirectory + File.separator + moduleName + ".module");
                Base.copyFile(moduleFileOriginal, moduleFileNew);
                
                ModuleFactory factory = new ModuleFactory();
                try {
                    IModule copiedGadget = factory.loadModule(moduleFileNew, outDirectory.getPath(), true);
                    
                    IModule[] oldGadgets = gadget.getModules();
                    IModule[] newGadgets = new IModule[oldGadgets.length + 1];

                    for(int i=0; i<oldGadgets.length; i++)
                    {
                        newGadgets[i] = oldGadgets[i];
                    }
                    
                    newGadgets[newGadgets.length - 1] = copiedGadget;
                    gadget.setModule(newGadgets);
                    
                    return gadget;
                } catch (Exception e) {
                    System.out.println("Error adding gadget to sketch");
                    e.printStackTrace();
                }
            }
        }
        throw new Exception("Error creating gadget");
    }
}
