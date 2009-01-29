package antipasto;


import org.w3c.dom.Element;

import antipasto.Interfaces.ICore;

import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;

public class Core implements ICore {

    private ArrayList files = new ArrayList();
    private String name;
    
    private int majorVersion = 0;
    private int minorVersion = 0;
    private int revisonVersion = 0;

    public void setFiles(File[] files) {
        this.files = new ArrayList(Arrays.asList(files));
    }

    public File[] getFiles() {
        File[] retFiles = new File[files.size()];
        for (int i = 0; i < files.size(); i++) {
            retFiles[i] = (File) files.get(i);
        }
        return retFiles;
    }

    public void setFiles(ArrayList files) {
        this.files = files;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

   public Element getConfiguration() {
        CoreFactory fact = new CoreFactory();
        Element elem = null;
        try {
            return fact.CreateCoreXML(this);
        } catch (Exception e) {
            e.printStackTrace();  //To change body of catch statement use File | Settings | File Templates.
        }
        return elem;
    }

    public int getMajorNumber() {
        return this.majorVersion;
    }

    public int getMinorNumber() {
        return this.minorVersion;
    }

    public int getRevisionNumber() {
        return this.revisonVersion;
    }

    public void setMajorNumber(int value) {
        this.majorVersion = value;
    }

    public void setMinorNumber(int value) {
        this.minorVersion = value;
    }

    public void setRevisionNumber(int value) {
        this.revisonVersion = value;
    }
}
