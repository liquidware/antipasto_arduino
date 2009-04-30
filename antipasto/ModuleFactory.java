package antipasto;

import org.w3c.dom.Element;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import antipasto.CoreFactory;
import antipasto.Module;
import antipasto.Interfaces.ICore;
import antipasto.Interfaces.IModule;
import antipasto.Interfaces.IVersionable;
import antipasto.ModuleRules.ModuleRulesParser;
import antipasto.Util.*;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.DocumentBuilder;
import java.io.File;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.util.List;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.logging.Logger;
import java.util.logging.Level;

public class ModuleFactory {
    public Element CreateModuleXML(IModule module) throws Exception {
        Element root = null;
        try
        {
            Element cores, sketch, nameElem, boardsElem, previewImgElem, targetElem;
            CoreFactory cFact = new CoreFactory();
            File[] files = module.getData();
            DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
            DocumentBuilder db = dbf.newDocumentBuilder();
            Document doc = db.newDocument();
            root = doc.createElement("Module");
            cores = doc.createElement("Cores");
            sketch = doc.createElement("Sketch");
            nameElem = doc.createElement("Name");
            boardsElem = doc.createElement("Boards");
            previewImgElem = doc.createElement("PreviewImage");
            Element dataElements = doc.createElement("Data");
            targetElem = doc.createElement("Target");

            String name = module.getName();

            for(int i=0; i<files.length; i++)
            {
                Element dataElem = doc.createElement("File");
                dataElem.setAttribute("value", files[i].getName());
                dataElements.appendChild(dataElem);
            }

            for(int i=0; i<module.getCores().length; i++)
            {
                ICore currentCore = module.getCores()[i];
                Element coreElem = (Element) doc.importNode(cFact.CreateCoreXML(currentCore), true);
                cores.appendChild(coreElem);
            }

            sketch.setAttribute("value", module.getSketchFile().getName());

            if(module instanceof Module)
            {
                previewImgElem.setAttribute("value", ((Module)module).getImageFile().getName());
            }

            boardsElem.setAttribute("value", module.getBoardsFile().getName());

            nameElem.setAttribute("value", name);

            targetElem.setAttribute("value", module.getTarget());

            root.appendChild(targetElem);
            root.appendChild(nameElem);
            root.appendChild(sketch);
            root.appendChild(cores);
            root.appendChild(boardsElem);
        }
        catch(Exception ex)
        {
            throw new Exception("Error creating the module configuration " + ex.getMessage());
        }
        return root;
    }

    public IModule CreateModule(File[] files, String name, String outputDirectory) throws Exception {
            IModule retModule = this.CreateModule(files, outputDirectory);
            retModule.setName(name);
            return retModule;
        }
    public IModule CreateModule(File[] files, String outputDirectory) throws Exception {
        File configFile = null;
        File boardsFile = null;
        File img = null;
        File rulesFile = null;
        for(int i = 0; i < files.length; i++)
        {
            if(files[i].getName().equalsIgnoreCase("config.xml")){
                configFile = files[i];
            }else if(files[i].getName().equalsIgnoreCase("boards.txt")){
                boardsFile = files[i];
            }else if(files[i].getName().equalsIgnoreCase("preview.jpeg") ||
            		files[i].getName().equalsIgnoreCase("preview.png") || 
            		files[i].getName().equalsIgnoreCase("preview.jpg")){
                img = files[i];
            }else if(files[i].getName().equalsIgnoreCase("rules.xml")){
            	rulesFile = files[i];
            }
        }

        if(configFile == null)
        {
            throw new Exception("No configuration file found");
        }

        File moduleFile = Packer.packageFile(outputDirectory + File.separator + "temp.module", files);

        Module retMod = (Module) this.CreateModule(configFile, boardsFile, img ,files, moduleFile, rulesFile ,outputDirectory, true);
        if(rulesFile != null){
        	ModuleRulesParser parser = new ModuleRulesParser(rulesFile);
        	retMod.setRules(parser.getModuleRules());
        }
        return retMod;
    }

   public IModule loadModule(File module, String outputDirectory, boolean checkDepends) throws Exception {
       String outputDir = outputDirectory;
       File dir = new File(outputDir);

       if(dir.exists())
       {
           boolean isDir = dir.isDirectory();
           if(!isDir)
           {
               throw new Exception("Directory specified is not a directory");
           }
       }
       else
       {
           dir.mkdir();
       }
       File[] moduleFiles = UnPacker.UnPack(module, dir);

       File configFile = null;
       File boardsFile = null;
       File previewImage = null;
       File rulesFile = null;

       for(int i = 0; i < moduleFiles.length; i++)
       {
           if(moduleFiles[i].getName().equalsIgnoreCase("config.xml"))
           {
               configFile = moduleFiles[i];
           }else if(moduleFiles[i].getName().equalsIgnoreCase("boards.txt"))
           {
               boardsFile = moduleFiles[i];
           }else if(moduleFiles[i].getName().equalsIgnoreCase("preview.gif") ||
        		    moduleFiles[i].getName().equalsIgnoreCase("preview.png") ||
        		    moduleFiles[i].getName().equalsIgnoreCase("preview.jpeg")||
        		    moduleFiles[i].getName().equalsIgnoreCase("preview.jpg"))
           {
               previewImage = moduleFiles[i];
           }else if(moduleFiles[i].getName().equalsIgnoreCase("rules.xml")){
        	   rulesFile = moduleFiles[i];
           }else if(moduleFiles[i].getName().equalsIgnoreCase("data.jar")){
        	   //we've found the data folder
        	   File dataFolder = new File(dir.getPath() + File.separator + "data");
        	   if(!dataFolder.exists()){
        		   dataFolder.mkdir();
        	   }
        	   UnPacker.UnPack(moduleFiles[i], dataFolder);
           }
       }
       return this.CreateModule(configFile, boardsFile, previewImage, moduleFiles, module, rulesFile ,outputDirectory, true);
   }

    public File WriteModuleToFile(IModule module, String outputDirectory) throws Exception {
        File outDir = new File(outputDirectory);

        if(!outDir.exists())
        {
            outDir.mkdir();
        }
        if(module.getConfiguration() != null)
        {
            int tot = 0;
            File imageFile = null;
            if(module.getBoardsFile() != null)
            {
                tot++;
            }
            if(module.getImage() != null)
            {
                if(module instanceof Module)
                {
                    tot++;
                    imageFile = ((Module)module).getImageFile();
                }
            }
            if(module.getConfiguration() != null)
            {
                tot++;
            }
            if(module.getSketchFile() != null)
            {
                tot++;
            }

            File xml = XMLWriter.Writer(module.getConfiguration(), "config.xml", outputDirectory);
            CoreFactory fact = new CoreFactory();
            File[] cores = new File[module.getCores().length];
            for(int i = 0; i < module.getCores().length; i++)
            {
                cores[i] = fact.WriteCoreToFile(outputDirectory, module.getCores()[i].getName(), ".core", module.getCores()[i]);
            }
            int rulesFileCount = 0;
            if(module.getRulesFile()!= null){
            	rulesFileCount = 1;
            }
            int moduleDataCount = 0;		//don't be alarmed, were only using this
            								//for array init.
            if(module.getData().length > 0) moduleDataCount = 1;
            
            File[] packerFiles = new File[moduleDataCount + tot + cores.length + rulesFileCount];
            File packedData;
            int x = 0;
            if(moduleDataCount > 0){
            	packedData = Packer.packageFile(outputDirectory + File.separator + "data.jar", module.getData());
            	packerFiles[x] = packedData;
            	x++;
            }
            
           
            packerFiles[x] = xml;
            packerFiles[++x] = module.getSketchFile();
            packerFiles[++x] = module.getBoardsFile();
            if(imageFile != null)
            {
                packerFiles[++x] = imageFile;
            }
            if(module.getRulesFile() != null){
            	packerFiles[++x] = module.getRulesFile();
            }
            for(int i=0; i < cores.length; i++)
            {
                packerFiles[++x] = cores[i];                
            }
           return Packer.packageFile(outputDirectory + File.separator + module.getName() + ".module", packerFiles);
        }
        return null;
    }

    public IModule CreateModule(File[] files)
    {
        File[] dataFiles = null;
        ArrayList cores = new ArrayList();
        File sketch = null;
        File boards = null;
        String name = null;
        File img = null;
        ArrayList packedFiles = new ArrayList();
        File rulesFile = null;
        
        CoreFactory cf = new CoreFactory();

        String directory = files[0].getPath().replaceAll(files[0].getName(), "");

        if(!(new File(directory + File.separator + "cores").exists()))
        {
            new File(directory + File.separator + "cores").mkdir();
        }

        if(!(new File(directory + File.separator + "data").exists()))
        {
            new File(directory + File.separator + "data").mkdir();
        }

        File dir = new File(directory);
        for(int i = 0; i < files.length; i++)
        {
            if(files[i].isDirectory())
            {
                if(files[i].getName().equalsIgnoreCase("data"))
                {
                    dataFiles = files[i].listFiles();
                }
                else if(files[i].getName().equalsIgnoreCase("cores"))
                {
                    File[] coreDirectories = files[i].listFiles();
                    for(int x=0; x<coreDirectories.length; x++)
                    {
                        if(coreDirectories[x].isDirectory())
                        {
                            try {
                                ICore coreObj = cf.CreateCore(coreDirectories[x].listFiles());
                                if(!checkIfCoreAlreadyExists(cores, coreObj))
                                {
                                    cores.add(coreObj);
                                }
                            } catch (Exception e) {
                                e.printStackTrace();  
                            }
                        }
                    }
                }
            }
            else if(files[i].getName().toLowerCase().endsWith(".pde"))
            {
                sketch = files[i];
                name = files[i].getName().substring(0, files[i].getName().length());
            }
            else if(files[i].getName().toLowerCase().endsWith(".core"))
            {
                ICore coreObj = cf.CreateCore(files[i], directory + "cores" +File.separator + files[i].getName().replaceAll(".core", ""));
                if(!this.checkIfCoreAlreadyExists(cores, coreObj))
                {
                    cores.add(coreObj);
                }
            }
            else if(files[i].getName().equalsIgnoreCase("boards.txt"))
            {
                boards = files[i];
            }
            else if(files[i].getName().equalsIgnoreCase("preview.gif") || 
            		files[i].getName().equalsIgnoreCase("preview.png") ||
            		files[i].getName().equalsIgnoreCase("preview.jpeg") || 
            		files[i].getName().equalsIgnoreCase("preview.jpg"))
            {
                img = files[i];
            }
            else if(files[i].getName().endsWith(".module"))
            {
                 packedFiles.add(files[i]);
            }
            else if(files[i].getName().endsWith("rules.xml")){
            	rulesFile = files[i];
            }
        }

        Module retModule = new Module();
        retModule.setData(dataFiles);
        retModule.setName(name);
        retModule.setSketchFile(sketch);
        retModule.setBoardsFile(boards);
        retModule.setRulesFile(rulesFile);
        try {
			retModule.setImage(img);
		} catch (IOException e) {
			e.printStackTrace();
		}
        retModule.setTempDirectory(files[0].getPath().replaceAll(files[0].getName(), ""));
        for(int i = 0; i < packedFiles.size(); i++){
            if(((File)(packedFiles.get(i))).getName().equalsIgnoreCase(name + ".module")){
                retModule.setPackedFile((File)packedFiles.get(i));
            }
        }

        ICore[] coreArr = new ICore[cores.size()];

        for(int i = 0; i < cores.size(); i++)
        {
            coreArr[i] = (ICore) cores.get(i);
        }

        retModule.setCores(coreArr);
        return retModule;
    }

    public IModule CreateModule(File xmlConfig, File boardsTxt, File img ,File[] files, File moduleFile, File rulesFile, String outputDirectory, boolean checkDepends) throws Exception {
        Element root = this.getFirstElementOfModuleFile(xmlConfig);
        NodeList children = root.getChildNodes();
        CoreFactory coreFact = new CoreFactory();
        IVersionable version = null;
        File sketchFile = null;
        File previewImage = img;
        File _rulesFile = rulesFile;

        File outputDir = new File(outputDirectory);
        File coresDirectory = new File(outputDirectory + "/cores");
        File dataDirectory = new File(outputDirectory + "/data");

        this.createDirectoryIfItDoesntExist(outputDir);
        this.createDirectoryIfItDoesntExist(coresDirectory);
        this.createDirectoryIfItDoesntExist(dataDirectory);

        String name = root.getElementsByTagName("Name").item(0).getAttributes().item(0).getNodeValue();
        String target = root.getElementsByTagName("Target").item(0).getAttributes().item(0).getNodeValue();

        List cores = new ArrayList();
        List fileList = new ArrayList();

        for(int i=0; i < children.getLength(); i++){
            if(children.item(i).getNodeName().equalsIgnoreCase("cores")){
                NodeList coreNodes = ((Element)children.item(i)).getElementsByTagName("Core");
                for(int x = 0; x < coreNodes.getLength(); x++){
                    System.out.println( coreNodes.getLength() + coreNodes.item(0).getNodeName());
                    Element coreRoot = (Element) coreNodes.item(x);
                    
                    NodeList coreElements = coreRoot.getChildNodes();
                    String coreName = coreRoot.getElementsByTagName("Name").item(0).getAttributes().getNamedItem("value").getNodeValue();
                    File coreFile = (this.findFileByName(coreName + ".core", files));

                    ICore core = coreFact.UnpackCore(coreFile, coresDirectory.getPath());

                    if(checkDepends)
                    {
                        for(int z = 0; z < coreElements.getLength(); z++ )
                        {
                            if(coreElements.item(z).getNodeName().equalsIgnoreCase("version"))
                            {
                                //Version has to be greater than the previous
                                version = XMLVersionReader.Load((Element) coreElements.item(z));
                                if(!VersionCompatiablity.IsSameOrNewer(version, core))
                                {
                                    throw new Exception("Versions dont match for core:" + coreName);
                                }
                            }
                        }
                    }
                    if(!checkIfCoreAlreadyExists(cores, core))
                    {
                        cores.add(core);
                    }
                }
            }
            else if(children.item(i).getNodeName().equalsIgnoreCase("data")){
                NodeList fileElements = children.item(i).getChildNodes();
                File[] dataFiles = new File(files[0].getPath() + File.separator + "data").listFiles();

                for(int x = 0; x < fileElements.getLength(); x++ )
                {
                    Element fileRoot = (Element) children.item(x);
                    String fileName = fileRoot.getElementsByTagName("Name").item(0).getAttributes().item(0).getNodeValue();
                    fileList.add(this.findFileByName(fileName, dataFiles));
                }
            }
            else if(children.item(i).getNodeName().equalsIgnoreCase("version")){
                version = XMLVersionReader.Load((Element) children.item(i));
            }
            else if(children.item(i).getNodeName().equalsIgnoreCase("sketch")){
                Element elem = (Element) children.item(i);
                sketchFile = this.findFileByName(elem.getAttributeNode("value").getNodeValue(), files);
            }
            else if(children.item(i).getNodeName().equalsIgnoreCase("message")){
            	Element elem = (Element)children.item(i);
            	NodeList elems = elem.getChildNodes();
            	
            	for(int x=0; x < elems.getLength(); x++){
            		
            	}
            }
        }


        ICore[] coreArray = new ICore[cores.size()];
        File[] fileArray = new File[fileList.size()];

        for(int i =0; i < cores.size(); i++)
        {
            coreArray[i] = (ICore) cores.get(i);
        }

        for(int i=0; i<fileList.size(); i++)
        {
            fileArray[i] = (File)fileList.get(i);
        }

        Module module = new Module();

        module.setCores(coreArray);
        module.setData(fileArray);
        module.setName(name);
        module.setSketchFile(sketchFile);
        module.setBoardsFile(boardsTxt);
        module.setImage(img);
        module.setTempDirectory(outputDirectory);
        module.setPackedFile(moduleFile);
        module.setTarget(target);
        module.setRulesFile(rulesFile);

      
        if(version == null)
        {
            module.setMajorNumber(1);
            module.setMinorNumber(0);
            module.setRevisionNumber(0);
        }
        else
        {
            module.setMajorNumber(version.getMajorNumber());
            module.setMinorNumber(version.getMinorNumber());
            module.setRevisionNumber(version.getRevisionNumber());
        }

          return module;
    }

    private File findFileByName(String name, File[] files) throws FileNotFoundException {
        for(int i = 0; i < files.length; i++)
        {
            if(files[i].getName().equalsIgnoreCase(name))
            {
                return files[i];
            }
        }
        throw new FileNotFoundException("File not found in the array");
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

    private boolean checkIfCoreAlreadyExists(List cores, ICore core)
    {
        for(int i = 0; i <cores.size(); i++)
        {
            ICore coreCompare = (ICore) cores.get(i);

            if((coreCompare != null) && core.getName().equalsIgnoreCase(coreCompare.getName()))
            {
                if(core.getMajorNumber() == coreCompare.getMajorNumber() &&
                   core.getMinorNumber() == coreCompare.getMinorNumber() &&
                   core.getRevisionNumber() == coreCompare.getRevisionNumber())
                {
                    return true;
                }
            }
        }
        return false;
    }

    private void createDirectoryIfItDoesntExist(File dir)
    {
        if(!dir.exists())
        {
            dir.mkdir();
        }
    }

}
