package antipasto.Interfaces;

import org.w3c.dom.Element;

import antipasto.ModuleRules.ModuleRules;

import javax.swing.*;

import java.awt.image.BufferedImage;
import java.io.File;

public interface IModule extends IVersionable, IConfigurable {
    //preview image needs to be specified
    public BufferedImage getImage();

    public File[] getData();
    public void setData(File[] files);
	
	public void addFile(File file);

    public void setName(String name);
    public String getName();

    public Element getConfiguration() throws Exception;

    public ICore[] getCores();
    public void setCores(ICore[] cores);

    public File getSketchFile();
    public void setSketchFile(File sketchFile);

    public File getBoardsFile();
    public void setBoardsFile(File boardsFile);

    public void copyCoreToDirectory(String parentDirectory);

    public String getTarget();
    public void setTarget(String target);
    
    public ModuleRules getRules();
    public File getRulesFile();
    
    public static String moduleExtension = ".module";
}
