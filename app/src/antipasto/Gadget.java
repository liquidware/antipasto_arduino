package antipasto;

import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

import antipasto.Interfaces.*;

import java.io.File;

public class Gadget implements IGadget, ITemporary, IPackedFile {
    private int _majorVersion = 0;
    private int _minorVersion = 0;
    private int _revisonVersion = 0;
    private String _name = "";
    private IModule[] _gadgets;
    private String temporaryDir;
    private File packedFile;
    private int iterator = 0;
    private boolean hasRules;
    private IModuleRule[] gadgetRules;

    private IModuleRule prevRule;
    

    public void setModule(IModule[] gadgets) {
        _gadgets = gadgets;
    }

    public IModule[] getModules() {
        return _gadgets;
    }

    public String getName() {
        return _name;  //To change body of implemented methods use File | Settings | File Templates.
    }

    public void setName(String name) {
        _name = name;
    }

    public void beginBuildGadgets() {
        this.iterator = -1;
    }

    public IModule getNextGadget() {
        iterator++;
        if(this.iterator >= this._gadgets.length){
            return null;
        }else{
            return this._gadgets[iterator];
        }
    }

    private IModule findGadgetByName(String name){
        for(int i=0; i < this._gadgets.length; i++){
            if(this._gadgets[i].getName().equalsIgnoreCase(name)){
                return this._gadgets[i];
            }
        }
        return null;
    }

    public int getMajorNumber() {
        return this._majorVersion;
    }

    public int getMinorNumber() {
        return this._minorVersion;
    }

    public int getRevisionNumber() {
        return this._revisonVersion;
    }

    public void setMajorNumber(int value) {
        this._majorVersion = value;
    }

    public void setMinorNumber(int value) {
        this._minorVersion = value;
    }

    public void setRevisionNumber(int value) {
        this._revisonVersion = value;
    }

    public Element getConfiguration() throws Exception {
        GadgetFactory fact = new GadgetFactory();
        return fact.createGadgetXML(this);
    }

    public void setTempDirectory(String directory) {
        this.temporaryDir = directory;
    }

    public String getTempDirectory() {
        return this.temporaryDir;
    }

    public void setPackedFile(File file) {
        packedFile = file;
    }

    public File getPackedFile() {
        return packedFile;  
    }

    public IModuleRule getRuleForGadget(IModule gadget){
        for(int i =0; i < this.gadgetRules.length; i++){
            if(this.gadgetRules[i].getGadget().getName().equalsIgnoreCase(gadget.getName())){
                return this.gadgetRules[i];
            }
        }
        return null;
    }

    public void loadRulesFile(Element rules){
        NodeList gadgetRules = rules.getElementsByTagName("Gadget");
        IModuleRule[] gadgetRuleObjects = new IModuleRule[gadgetRules.getLength()];

        this.hasRules = true;

        for(int i = 0; i < gadgetRules.getLength(); i++){
            Element gadgetRule = (Element) gadgetRules.item(i);
            String name = gadgetRule.getAttribute("Name");
            String message = gadgetRule.getAttribute("Message");
            boolean hasMessage = gadgetRule.getAttribute("hasMessage").equalsIgnoreCase("true");
            boolean needsContinueButton = gadgetRule.getAttribute("needsContinueButton").equalsIgnoreCase("true");
            boolean needsRetryButton = gadgetRule.getAttribute("needsRetryButton").equalsIgnoreCase("true");

            IModule gadget = this.findGadgetByName(name);

            IModuleRule gadgetRuleObj = new ModuleRule();            
            gadgetRuleObj.setNeedsRetryButton(needsRetryButton);
            gadgetRuleObj.setNeedsContinueButton(needsContinueButton);
            gadgetRuleObj.setHasMessage(hasMessage);
            gadgetRuleObj.setGadget(gadget);
            gadgetRuleObj.setMessage(message);

            gadgetRuleObjects[i] = gadgetRuleObj;
        }

        this.gadgetRules = gadgetRuleObjects;
        this.orderGadgetsByRules();
    }

    private void orderGadgetsByRules(){
        IModule[] reordered = new IModule[this._gadgets.length];
        IModule[] original = this._gadgets;
        int index = 0;
        for(int i = 0; i < gadgetRules.length; i++){
            IModule gadget = gadgetRules[i].getGadget();
            for(int x = 0; x < this._gadgets.length; x++){
                if(gadget.getName().equals(this._gadgets[x].getName())){
                    this._gadgets[x] = null;
                    break;
                }
            }
            reordered[index] = gadget;
            index++;
        }

        for(int i = 0; i < this._gadgets.length; i++){
            if(this._gadgets != null){
                reordered[index] = this._gadgets[i];
                index++;
            }
        }
        this._gadgets = reordered;
    }
}
