package antipasto;

import org.w3c.dom.Element;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

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
        if(gadgetFile.getName().endsWith(".pde") || gadgetFile.getName().endsWith(".gadget"))
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

    public File writeGadgetToFile(IGadget gadget, String outputDir) throws IOException {
        File outDir = new File(outputDir);
        if(!outDir.exists())
        {
            outDir.mkdir();
        }

        File tmpDir = new File(System.getProperty("java.io.tmpdir") + File.separator + gadget.getName() + ".tmp");
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


        File returnFile = Packer.packageFile(outDir.getPath() + File.separator + gadget.getName() + ".pde"
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
                            System.out.println("Module being loaded : " + outputDirectory + File.separator + moduleName);
                            System.out.println("Module location : " + moduleFile.getPath());
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

    public IGadget AddModuleToGadget(IGadget gadget, IModule module) throws Exception {
        if(gadget instanceof Gadget){
            if(module instanceof Module){
                int hasAlreadyCount = 0;
                for(int i = 0; i < gadget.getModules().length; i++){
                    if(gadget.getModules()[i].getName().equalsIgnoreCase(module.getName())){
                        hasAlreadyCount ++;
                    }
                }

                if(hasAlreadyCount > 1){
                    module.setName(module.getName() + "(" + hasAlreadyCount + ")");
                }
                //get the file copy it to the book directory, unpack it to a subfolder
                Gadget mBook = (Gadget) gadget;
                Module mGadget = (Module)module;
                File gadgetFile = mGadget.getPackedFile();
                File outDirectory = new File(mBook.getTempDirectory() + File.separator + module.getName());

                ModuleFactory factory = new ModuleFactory();
                try {
                    IModule copiedGadget = factory.loadModule(gadgetFile, outDirectory.getPath(), true);
                    copy(gadgetFile, new File(mBook.getTempDirectory()));

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

    private void copy(File src, File dir) throws IOException {
           File dst = new File(dir.getPath() + File.separator + src.getName());
           InputStream in = new FileInputStream(src);
           OutputStream out = new FileOutputStream(dst);

           // Transfer bytes from in to out
           byte[] buf = new byte[1024];
           int len;
           while ((len = in.read(buf)) > 0) {
               out.write(buf, 0, len);
           }
           in.close();
           out.close();
       }
}
