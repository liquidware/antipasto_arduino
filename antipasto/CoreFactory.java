package antipasto;

import org.w3c.dom.Element;
import org.w3c.dom.NodeList;
import org.w3c.dom.Document;
import org.xml.sax.SAXException;

import antipasto.Core;
import antipasto.Interfaces.ICompiler;
import antipasto.Interfaces.ICore;
import antipasto.Interfaces.ITemporary;
import antipasto.Interfaces.IVersionable;
import antipasto.Util.FileCopy;
import antipasto.Util.UnPacker;
import antipasto.Util.XMLVersionCreator;
import antipasto.Util.XMLVersionReader;


import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.ParserConfigurationException;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.ArrayList;
import java.util.Arrays;

public class CoreFactory {
    public ICore CreateCore(File[] files) throws Exception {
        for(int i = 0; i < files.length; i++ )
        {
            if(files[i].getName().equalsIgnoreCase("config.xml"))
            {
               return this.CreateCore(this.getBaseElementOfCore(files[i]), files);
            }
        }
        Core c = new Core();
        c.setFiles(files);
        c.setName(files[0].getName());
        c.setMajorNumber(1);
        c.setMinorNumber(0);
        c.setRevisionNumber(0);

        return c;
    }

    public ICore CreateCore(Element core, File[] files) throws Exception
    {
        String name = "";

        System.out.println(core);
        int majorVersion = 0;
        int minorVersion = 0;
        int revision = 0;

        File[] outputFiles = new File[files.length];

        if(core.getNodeName().equalsIgnoreCase("core"))
        {
            NodeList nodes = core.getChildNodes();

            for(int i=0; i < nodes.getLength(); i++)
            {
                if(nodes.item(i).getNodeName().equalsIgnoreCase("Name"))
                {
                    Element elem = (Element) nodes.item(i);
                    name = elem.getAttribute("value");
                }else if(nodes.item(i).getNodeName().equalsIgnoreCase("Files"))
                {
                    NodeList fileNodes = nodes.item(i).getChildNodes();
                    for(int x = 0; x < fileNodes.getLength(); x++)
                    {
                        File f;
                        String fileName = ((Element)fileNodes.item(i)).getAttribute("value");
                        if((f = this.CheckFile(fileName, files)) != null)
                        {
                            try
                            {
                                outputFiles[x] = (f);
                            }
                            catch(Exception ex)
                            {

                            }
                        }
                        else
                        {
                            throw new FileNotFoundException("Couldn't find the file specified");
                        }
                        //System.out.println(fileName);
                    }
                }else if(nodes.item(i).getNodeName().equalsIgnoreCase("Compilers"))
                {
                    //Future
                    /*NodeList compilerNodes = nodes.item(i).getChildNodes();
                    for(int x = 0; x < compilerNodes.getLength(); x++)
                    {
                        ICompiler compiler = compilerFact.CreateCompiler((Element)compilerNodes.item(x));
                        compilers.add(compiler);
                    } */
                }else if(nodes.item(i).getNodeName().equalsIgnoreCase("Version"))
                {
                    IVersionable version = XMLVersionReader.Load((Element) nodes.item(i));
                    majorVersion = version.getMajorNumber();
                    minorVersion = version.getMinorNumber();
                    revision = version.getRevisionNumber();
                }
            }

            return this.CreateCore(name, null, files, majorVersion, minorVersion, revision);
        }
        else
        {
            throw new Exception("Core must be defined at the root");
        }
    }

    public ICore CreateCore(File f, String outputDirectory)
    {
        //TODO: 1)UnPack 2)Parse Config 3)Create Compiler Objects 4)Create Source Objects
        try {
            File outputDir = new File(outputDirectory);
            if(!outputDir.exists())
            {
                outputDir.mkdir();
            }
            File[] files = UnPacker.UnPack(f, new File(outputDirectory));

            for(int i =0; i < files.length; i++)
            {
                File cf = files[i];
                if(cf.getName().equalsIgnoreCase("config.xml"))
                {
                    Element config = this.getBaseElementOfCore(cf);
                    files[i] = null;                               //Get rid of the XML file
                    return this.CreateCore(config, files);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        }

        return null;
    }

    private ICore CreateCore(ICore core)
    {
        return this.CreateCore(core.getName(), null, core.getFiles(), core.getMajorNumber(), core.getMinorNumber(), core.getRevisionNumber());
    }

    public Element CreateCoreXML(ICore core) throws Exception
    {
        try {
            DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
            DocumentBuilder db = dbf.newDocumentBuilder();
            Document doc = db.newDocument();
            Element root = doc.createElement("Core");
            Element nameNode = (Element) root.appendChild(doc.createElement("Name"));
            nameNode.setAttribute("value", core.getName());
            Element compilerOutput = (Element) root.appendChild(doc.createElement("Compilers"));
            /*for(int i=0; i < core.getCompilers().size(); i++)
            {
                compilerOutput.appendChild(fact.CreateCompilerXMLElement(core.getCompilers().get(i)));
            } */
            Element fileOutput = doc.createElement("Files");
            for(int i=0; i < core.getFiles().length; i++)
            {
                try
                {
                    Element elem = doc.createElement("File");
                    elem.setAttribute("value", core.getFiles()[i].getName());
                    fileOutput.appendChild(elem);
                }
                catch(Exception ex)
                {
                    System.out.println(ex.getMessage());
                    System.out.println(fileOutput);
                }
            }
            root.appendChild(fileOutput);
            Element version = XMLVersionCreator.CreateVersionElement(doc, (IVersionable)core);
            
            root.appendChild(version);
            return root;
        } catch (ParserConfigurationException ex) {
            Logger.getLogger(CoreFactory.class.getName()).log(Level.SEVERE, null, ex);
            throw new Exception("Error creating compiler XML Element");
        }
    }

    private File CheckFile(String fileName, File[] files)
    {
        for(int i = 0; i < files.length; i++)
        {
            File f = files[i];
            if(f != null && f.getName().equalsIgnoreCase(fileName))
                return f;
        }

        return null;
    }

    private ICore CreateCore(String name, ICompiler[] compilers,  File[] files, int majorVersion, int minorVersion, int revision)
    {
        Core retCore = new Core();
        retCore.setFiles(files);
        retCore.setName(name);

        retCore.setMajorNumber(majorVersion);
        retCore.setMinorNumber(minorVersion);
        retCore.setRevisionNumber(revision);

        return retCore;
    }

    public File WriteCoreToFile(String directory, String fileName, String extension, ICore core) throws Exception
    {
        try {
            String tmpDirectoryPath = System.getProperty("java.io.tmpdir") + File.separator + core.getName();
            File tmpDir = new File(tmpDirectoryPath);
            if(!tmpDir.exists()){
                tmpDir.mkdir();
            }

            //Copy all of the core files then create a config.xml in the directory
            for(int i=0; i<core.getFiles().length; i++){
                if(core.getFiles()[i] != null && !core.getFiles()[i].getName().endsWith("config.xml")){
                    FileCopy.copy(core.getFiles()[i].getPath(), 
                            tmpDir.getPath() + File.separator + core.getFiles()[i].getName());
                }
            }

            if(!tmpDir.exists() && !tmpDir.mkdir())
            {
            	System.out.println(tmpDirectoryPath);
                throw new Exception("Couldn't create temporary directory");
            }

            File dir = new File(directory);
            if(!dir.exists()){
                dir.mkdir();
            }
            
            File xmlFile = antipasto.Util.XMLWriter.Writer(this.CreateCoreXML(core), "config.xml", tmpDir.getPath());
            boolean hasConfig = false;
            int configIndex = 0;

            File retFile = antipasto.Util.Packer.packageFile(directory + File.separator + fileName + extension, tmpDir.listFiles());
            return retFile;
        } catch (IOException ex) {
            Logger.getLogger(CoreFactory.class.getName()).log(Level.SEVERE, null, ex);
        }

        return null;
    }

    public ICore UnpackCore(File coreFile, String outputDirectory) throws Exception
    {
    	try {
	    	File[] files = UnPacker.UnPack(coreFile, new File(outputDirectory + File.separator + coreFile.getName().replaceAll(".core", "")));

	        File configFile = this.findFileByName(files, "config.xml");

	        Element config = this.getBaseElementOfCore(configFile);

			return this.CreateCore(config, files);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		throw new Exception("Error creating the core package.");
    }

    private File findFileByName(ArrayList files, String name) throws FileNotFoundException {
        File[] fileArray = new File[files.size()];

        for(int i = 0; i < files.size(); i++)
        {
            fileArray[i] = (File) files.get(i);
        }
        return  this.findFileByName(fileArray, name);
    }
    
    private File findFileByName(File[] files, String name) throws FileNotFoundException
    {
    	for(int i=0; i < files.length; i++)
    	{
            File f = files[i];
            //System.out.println(name.toLowerCase());
    		if(f.getName().toLowerCase().endsWith(name.toLowerCase()))
    		{
    			return f;
    		}
    	}
    	throw new FileNotFoundException();
    }

    private Element getBaseElementOfCore(File configFile) throws Exception
    {
        try {
            DocumentBuilderFactory docFact = DocumentBuilderFactory.newInstance();
            DocumentBuilder docBuilder = docFact.newDocumentBuilder();
            Document doc = docBuilder.parse(configFile);
            return (Element) doc.getFirstChild();
        } catch (SAXException ex) {
            Logger.getLogger(CoreFactory.class.getName()).log(Level.SEVERE, null, ex);
        } catch (IOException ex) {
            Logger.getLogger(CoreFactory.class.getName()).log(Level.SEVERE, null, ex);
        }

        throw new Exception("Error getting the base element of the project");
    }
}
