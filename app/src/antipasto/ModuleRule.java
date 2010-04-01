package antipasto;

import antipasto.Interfaces.IModule;
import antipasto.Interfaces.IModuleRule;

public class ModuleRule implements IModuleRule {

    private String moduleName = "";
    private boolean hasMessage = false;
    private boolean needsContinueButton = true;
    private boolean needsRetryButton = true;
    private IModule module = null;
    private String message = "";

    public String getGadgetName() {
        return moduleName;
    }

    public void setGadgetName(String gadgetName) {
        this.moduleName = gadgetName;
    }

    public boolean isHasMessage() {
        return hasMessage;
    }

    public void setHasMessage(boolean hasMessage) {
        this.hasMessage = hasMessage;
    }

    public boolean isNeedsContinueButton() {
        return needsContinueButton;
    }

    public void setNeedsContinueButton(boolean needsContinueButton) {
        this.needsContinueButton = needsContinueButton;
    }

    public boolean isNeedsRetryButton() {
        return needsRetryButton;
    }

    public void setNeedsRetryButton(boolean needsRetryButton) {
        this.needsRetryButton = needsRetryButton;
    }

    public IModule getGadget() {
        return module;
    }


    public void setGadget(IModule gadget) {
        this.module = gadget;
    }

    public String getMessage(){
        return this.message;
    }

    public void setMessage(String message){
        this.message = message;
    }
}
