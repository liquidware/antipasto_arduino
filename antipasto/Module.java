package antipasto;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import org.w3c.dom.Element;

import antipasto.Interfaces.ICore;
import antipasto.Interfaces.IModule;
import antipasto.Interfaces.IPackedFile;
import antipasto.Interfaces.ITemporary;
import antipasto.ModuleRules.ModuleRules;
import antipasto.ModuleRules.ModuleRulesParser;

import javax.swing.*;
import javax.imageio.*;

import antipasto.Util.*;


public class Module implements IModule, ITemporary, IPackedFile {

    private File[] _files;
    private ICore[] _cores;
    private String _name;
    private int _major;
    private int _minor;
    private int _revision;
    private File _sketchFile;
    private File _boardsFile;
    private BufferedImage _img;
    private File imgFile;
    private String tempDir;
    private File packedFile;
    private String _target;
    private File _rulesFile;
    private ModuleRules _rules;
	private File dataDirectory;

    public BufferedImage getImage() {
        return _img;
    }

    public void  setImage(File img) throws IOException{
        _img = ImageIO.read(img);
        imgFile = img;
    }

    public File getImageFile()
    {
        return imgFile;
    }

    public File[] getData() {
    	checkDataDirectory();
        return dataDirectory.listFiles();
    }
	
	public void checkDataDirectory(){
		System.out.println("checking data directory");
		dataDirectory = new File(this.getTempDirectory() + File.separator + "data");
		if(!dataDirectory.exists()){
    		dataDirectory.mkdir();
    	}
	}
	
	public void addFile(File file){
		checkDataDirectory();
		String newFilePath = this.dataDirectory.getPath() + File.separator + file.getName();
		try{
			FileCopy.copy(file.getPath(), newFilePath);
		}catch(Exception ex){
			System.out.println(ex.getMessage());
		}
	}

    public void setData(File[] _files) {
        this._files = _files;
    }

    public void setName(String name) {
        this._name = name;
    }

    public String getName() {
        return this._name;
    }

    public Element getConfiguration() throws Exception {
        return new ModuleFactory().CreateModuleXML(this);
    }

    public ICore[] getCores() {

        return this._cores;
    }

    public void setCores(ICore[] cores) {
        this._cores = cores;
    }

    public File getSketchFile() {
        return this._sketchFile;
    }

    public void setSketchFile(File sketchFile) {
        this._sketchFile = sketchFile;
    }

    public File getBoardsFile() {
        return this._boardsFile;
    }

    public void setBoardsFile(File boardsFile) {
        this._boardsFile = boardsFile;
    }

    public void copyCoreToDirectory(String parentDirectory) {
        CoreFactory fact = new CoreFactory();
        File coreDirectory = new File(parentDirectory);

        if(coreDirectory.isDirectory()){
            for(int i = 0; i < this.getCores().length; i++){
                ICore curCore = this.getCores()[i];
                File coreSub = new File(coreDirectory.getPath() + File.separator + curCore.getName());
                if(!coreSub.exists()){
                    coreSub.mkdir();
                }
                try {
                    fact.CreateCore(fact.WriteCoreToFile(coreSub.getPath(), curCore.getName(),
                            ".core", curCore), coreSub.getPath());
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public String getTarget() {
        return _target;  
    }

    public void setTarget(String target) {
        _target = target;
    }

    public int getMajorNumber() {
        return this._major;
    }

    public int getMinorNumber() {
        return this._minor;
    }

    public int getRevisionNumber() {
        return this._revision;
    }

    public void setMajorNumber(int value) {
        this._major = value;
    }

    public void setMinorNumber(int value) {
        this._minor = value;
    }

    public void setRevisionNumber(int value) {
        this._revision = value;
    }

    protected ImageIcon createImageIcon(String path,
                                               String description) {
            return new ImageIcon(path, description);
    }

    public void setTempDirectory(String directory) {
        this.tempDir = directory;
    }

    public String getTempDirectory() {
        return this.tempDir;
    }

    public void setPackedFile(File file) {
        packedFile = file;
    }

    public File getPackedFile() {
        if(packedFile == null){
            ModuleFactory gf = new ModuleFactory();
            try {
                gf.CreateModule(new File(this.getTempDirectory()).listFiles(), this.getName(), System.getProperty("java.io.tmpdir"));
            } catch (Exception e) {
                e.printStackTrace();  
            }
        }
        return packedFile;
    }
    public void setRules(ModuleRules rules){
    	this._rules = rules;
    }
    
    public ModuleRules getRules(){
    	return this._rules;
    }
    
    public void setRulesFile(File r){
    	this._rulesFile = r;
    	if(this._rulesFile != null){
	    	ModuleRulesParser parser = new ModuleRulesParser(r);
	    	this.setRules(parser.getModuleRules());
    	}
    }
    
    public File getRulesFile(){
    	return this._rulesFile;
    }
}
