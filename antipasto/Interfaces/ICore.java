package antipasto.Interfaces;


import java.io.File;


/**
 * Created by IntelliJ IDEA.
 * User: Omar
 */
public interface ICore extends IVersionable, IConfigurable{
    public void setFiles(File[] files);

    public File[] getFiles();

    public void setName(String name);

    public String getName();
}
